# FIXME This works only for few robots. If you want, convert this into C++.
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
# cb1 storing, cb1 stocking, cb2 storing 
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
    global img_number, stock, start2end, condition

    cv2_img = bridge.imgmsg_to_cv2(rgb, "bgr8")
# FIXME Maybe should change to (640,480)
    cv2_img = cv2.resize(cv2_img, dsize=(512, 384))

    # REVIEW 再開
    # FIXME このままだとalready_loopがパブリッシュされた瞬間に以下の処理を実行できない
    if already_loop == 1:
        condition["cb1 storing"] = 1
        cv2.imwrite(path + "rgb/"+str(img_number) + ".jpg", cv2_img)
        cv2.imwrite(all_rgb + str(img_number) + ".jpg", cv2_img)
        print("image storing;"+str(img_number))

        img_number += 2

        condition["cb1 storing"] = 0

        stock.clear()

    # REVIEW 中止
    else:
        stock[str(img_number)] = cv2_img



def callback2(rgb, id):
    global img_number2, stock, condition

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
        condition["cb2 storing"] = 0
    # 中止
    else:
        stock[str(img_number2)] = cv2_img


def loop_CB(loop):
    global already_loop
    if already_loop == 0:
        # 再開
        already_loop = 1
        condition["cb1 stocking"]=0
        # NOTE 初回を除く
        if condition["cb1 storing"] == 0:
    
            # NOTE Erase detected images
            # FIXME can't remove
            for file_num in range(start2end["start"], start2end["end"]):
                os.remove(path + "rgb/"+str(file_num) + ".jpg")

            # NOTE Now store the images which were stored during loop closure detection

            if ("start" in start2end):
                # NOTE wait until cb2 begins its work and now stock is fully stored
                while (condition["cb2 storing"] == 0):
                    pass

                for index, img in stock:
                    cv2.imwrite(path + "rgb/"+str(index) + ".jpg", img)

                for index,img in temp_stock:
                    cv2.imwrite(path + "rgb/"+str(index) + ".jpg", img)

    else:
        # 中止
        already_loop = 0

        condition["cb1 storing"] = 0

        # NOTE callback2が画像を保存するのをやめるまで待つ
        while(condition["cb2 storing"] == 1):
            pass
        
        # NOTE 連続していないものをtemp_stockに避難させておく.
        l=[img_number,img_number2]
        l_no=max(l)-2
        # NOTE lはこれから作る予定の画像index
        print("Max image num is "+str(l_no))
        while True:
            if (os.path.exists(path+"rgb/"+str(l_no-1)+".jpg")):
                break
            temp_stock[l_no]=cv2.imread(path+"rgb/"+str(l_no)+".jpg")
            os.remove(path+"rgb/"+str(l_no)+".jpg")
            l_no-=2

        # NOTE loop closure nodeに画像が保存し終わったことを伝える
        msg.data=1
        confirm.publish(msg)

        # NOTE detectの対象ファイルをまとめる [start:int,end:int]
        start2end["start"] = start2end["end"]+1
        if img_number > img_number2:
            start2end["end"] = img_number
        else:
            start2end["end"] = img_number2

        
        condition["cb1 stocking"]=1


def setup():
    shutil.rmtree(path)

    os.mkdir(path)
    os.mkdir(all_rgb)
    os.mkdir(path+"/rgb/")

def timerCB(event):
    turn_on=Int32()
    turn_on.data=1
    switch.publish(turn_on)


if __name__ == '__main__':


    rospy.init_node('image_listener', anonymous=True)

    rgb_topic = "/robot1/camera/rgb/image_raw"
    rgb_topic2 = "/robot2/camera/rgb/image_raw"

    ID_topic = "/robot1/rtabmap/info"
    ID_topic2 = "/robot2/rtabmap/info"
    # # To store the images from camera

    # Remove all image

    rgb_sub = message_filters.Subscriber(rgb_topic, Image)
    rgb_sub2 = message_filters.Subscriber(rgb_topic2, Image)

    myid_sub = message_filters.Subscriber(ID_topic, Info)
    myid_sub2 = message_filters.Subscriber(ID_topic2, Info)

    button = rospy.Subscriber("loop", Int32, loop_CB)

    msg=Int32()
    confirm=rospy.Publisher("stop_storing",Int32,queue_size=1)
    #NOTE  loop nodeが起動する頃には画像の保存が終了している必要がある

    # ts = message_filters.TimeSynchronizer([rgb_sub, myid_sub], 10)
    ts = message_filters.ApproximateTimeSynchronizer(
        [rgb_sub, myid_sub], 10, 0.1, allow_headerless=False)
    ts.registerCallback(callback)

    ts2 = message_filters.ApproximateTimeSynchronizer(
        [rgb_sub2, myid_sub2], 10, 0.1, allow_headerless=False)
    ts2.registerCallback(callback2)
    # rospy.Subscriber(image_topic, Image, rgb_callback)
    switch=rospy.Publisher("command",Int32,queue_size=10)
    setup()

    # NOTE 5秒に一度detection nodeを呼び込む
    while img_number<10:
        pass
    rospy.Timer(rospy.Duration(5),timerCB)


    # REVIEW rospy spinが出ている時点でrate.sleepを用いて定時でpublishすることは不可能になる.
    rospy.spin()
