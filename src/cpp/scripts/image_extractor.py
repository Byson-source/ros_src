# FIXME This works only for few robots. If you want, convert this into C++.
# TODO 連番が崩れるときがあるので、loop nodeに許可を与えるときに工夫する
#! /usr/bin/python
import rospy

import os
import shutil
# ROS Image message
from sensor_msgs.msg import Image
# ROS Image message -> OpenCV2 image converter
from cv_bridge import CvBridge
# OpenCV2 for saving an image
from rtabmap_ros.msg import Info
from std_msgs.msg import Int32


import message_filters

import cv2
import time

path = "/home/ayumi/Documents/tohoku_uni/CLOVERs/images/"

all_rgb = "/home/ayumi/Documents/tohoku_uni/CLOVERs/images/all_rgb/"

img_number = 1
img_number2 = 2
already_loop = 1

#NOTE callback1's condition,2's condition
condition = {"cb1 storing":1,
            "cb1 stocking":0}

# NOTE index;img detection中に入ってきた画像を格納しておく
stock = {}
# NOTE loop nodeには連番した画像のみを読ませたい。そのために、連番になっていない画像を一時的に避難させておく。
temp_stock={}
start2end = {"end": 0}
# index:img
bridge = CvBridge()


def callback(rgb, id):
    global img_number, img_number2, already_loop, stock, start2end, condition

    cv2_img = bridge.imgmsg_to_cv2(rgb, "bgr8")
# FIXME Maybe should change to (640,480)
    cv2_img = cv2.resize(cv2_img, dsize=(512, 384))

    # 再開
    if already_loop == 1:

        if condition["cb1 storing"] == 0:

            condition["cb1 erasing"] = 1
            # NOTE Erase detected images
            # NOTE start2endが正しければ問題ないはず。。。

            for file_num in range(start2end["start"], start2end["end"]):
                os.remove(path + "rgb/"+str(file_num) + ".jpg")

            condition["cb1 erasing"] = 0

        # NOTE Now store the images which were stored during loop closure detection

            condition["cb1 recovering"] = 1
            if ("start" in start2end):
                # NOTE wait until cb2 begins its work and now stock is fully stored
                while (condition["cb2 storing"] == 0):
                    pass

                for index, img in stock:
                    cv2.imwrite(path + "rgb/"+str(index) + ".jpg", img)

                for index,img in temp_stock:
                    cv2.imwrite(path + "rgb/"+str(index) + ".jpg", img)

            condition["cb1 recovering"] = 0

        # NOTE ↓normal
        condition["cb1 storing"] = 1
        cv2.imwrite(path + "rgb/"+str(img_number) + ".jpg", cv2_img)
        cv2.imwrite(all_rgb + str(img_number) + ".jpg", cv2_img)
        print("image storing;"+str(img_number))

        img_number += 2

        stock.clear()

    # 中止
    else:
        condition["cb1 storing"] = 0

        if(condition["cb1 stocking"] == 0):
            # NOTE callback2が画像を保存するのをやめるまで待つ
            while(condition["cb2 storing"] == 1):
                pass
            
            # TODO 連続していないものをtemp_stockに避難させておく.
            l=[img_number,img_number2]
            l_no=max(l)-2
            # NOTE lはこれかあ作る予定の画像のindex
            print("Max image num is "+str(l_no))
            while True:
                if (os.path.exists(path+"rgb/"+str(l_no-1)+".jpg")):
                    break
                temp_stock[l_no]=cv2.imread(path+"rgb/"+str(l_no)+".jpg")
                # NOTE FileNotFoundError
                os.remove(path+"rgb/"+str(l_no)+".jpg")
                l_no-=2
            # NOTE これで連番になった
            # condition["seq_end"]=l_no

                # NOTE loop closure nodeに画像が保存し終わったことを伝える
            msg=Int32()
            msg.data=1

            # NOTE detectの対象ファイルをまとめる [start:int,end:int]
            print("confirm is publishing...")
            confirm.publish(msg)

            start2end["start"] = start2end["end"]+1
            if img_number > img_number2:
                start2end["end"] = img_number
            else:
                start2end["end"] = img_number2

        
        condition["cb1 stocking"]=1
        stock[str(img_number)] = cv2_img


def callback2(rgb, id):
    global img_number2, already_loop, stock, condition

    cv2_img = bridge.imgmsg_to_cv2(rgb, "bgr8")
# FIXME Maybe should change to (640,480)
    cv2_img = cv2.resize(cv2_img, dsize=(512, 384))
    # 再開
    if already_loop == 1:
        condition["cb2 storing"] = 1
        cv2.imwrite(path + "rgb/"+str(img_number2) + ".jpg", cv2_img)
        cv2.imwrite(all_rgb + str(img_number2) + ".jpg", cv2_img)
        print("image storing;"+str(img_number2))

        img_number2 += 2
    # 中止
    else:
        condition["cb2 storing"] = 0
        stock[str(img_number2)] = cv2_img


def loop_CB(loop):
    global already_loop
    if already_loop == 0:
        # 再開
        already_loop = 1
    else:
        # 中止
        already_loop = 0


def setup():
    shutil.rmtree(path)
    dir_info=Int32()
    dir_info.data=1

# NOTE Inform loop node that dir is deleted

    os.mkdir(path)
    os.mkdir(all_rgb)
    os.mkdir(path+"/rgb/")


if __name__ == '__main__':


    rospy.init_node('image_listener', anonymous=True)

    setup()

    rgb_topic = "/robot1/camera/rgb/image_raw"
    rgb_topic2 = "/robot2/camera/rgb/image_raw"

    ID_topic = "/robot1/rtabmap/info"
    # FIXME Fix to make it more easy to use for any-number usage
    ID_topic2 = "/robot2/rtabmap/info"
    # # To store the images from camera

    # Remove all image

    rgb_sub = message_filters.Subscriber(rgb_topic, Image)
    rgb_sub2 = message_filters.Subscriber(rgb_topic2, Image)

    myid_sub = message_filters.Subscriber(ID_topic, Info)
    myid_sub2 = message_filters.Subscriber(ID_topic2, Info)

    button = rospy.Subscriber("loop", Int32, loop_CB)
    # NOTE listen that loop node's setup is done
    confirm=rospy.Publisher("stop_storing",Int32,queue_size=1)

    # ts = message_filters.TimeSynchronizer([rgb_sub, myid_sub], 10)
    ts = message_filters.ApproximateTimeSynchronizer(
        [rgb_sub, myid_sub], 10, 0.1, allow_headerless=False)
    ts.registerCallback(callback)

    ts2 = message_filters.ApproximateTimeSynchronizer(
        [rgb_sub2, myid_sub2], 10, 0.1, allow_headerless=False)
    ts2.registerCallback(callback2)
    # rospy.Subscriber(image_topic, Image, rgb_callback)
    rospy.spin()
