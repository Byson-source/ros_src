# FIXME This works only for few robots. If you want, convert this into C++.
# REVIEW rosのcallbackはpythonの場合、並列して行える!C＋＋は無理？？
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
import glob


import message_filters

import cv2
import time

path = "/home/ayumi/Documents/tohoku_uni/CLOVERs/images/"

all_rgb = "/home/ayumi/Documents/tohoku_uni/CLOVERs/images/all_rgb/"

img_number = 1
img_number2 = 2
already_loop = 1

# NOTE index;img detection中に入ってきた画像を格納しておく
stock = {}
# NOTE loop nodeには連番した画像のみを読ませたい。そのために、連番になっていない画像を一時的に避難させておく。
temp_stock = {}
start2end = {"end": 0}
# index:img
bridge = CvBridge()

condition = {"cb1 storing": 1,
             "cb2 storing": 1}


def callback(rgb, id):
    global img_number, stock, condition

    cv2_img = bridge.imgmsg_to_cv2(rgb, "bgr8")
# FIXME Maybe should change to (640,480)
    cv2_img = cv2.resize(cv2_img, dsize=(640, 480))
    # rospy.logerr(img_number)

    # REVIEW 再開
    if already_loop == 1:
        condition["cb1 storing"] = 1
        cv2.imwrite(path + "rgb/"+str(img_number) + ".jpg", cv2_img)
        cv2.imwrite(all_rgb + str(img_number) + ".jpg", cv2_img)
        # print("image storing;"+str(img_number))
        img_number += 2
        condition["cb1 storing"] = 0

    # REVIEW 中止
    else:
        condition["cb1 storing"] = -1
        # rospy.loginfo("CB1 stocking img "+str(img_number))
        cv2.imwrite(all_rgb + str(img_number) + ".jpg", cv2_img)
        stock[str(img_number)] = cv2_img

        img_number += 2


def callback2(rgb, id):
    global img_number2, stock, condition
    # rospy.logerr(img_number2)

    cv2_img = bridge.imgmsg_to_cv2(rgb, "bgr8")
# FIXME Maybe should change to (640,480)
    cv2_img = cv2.resize(cv2_img, dsize=(640, 480))
    # 再開
    if already_loop == 1:
        condition["cb2 storing"] = 1
        cv2.imwrite(path + "rgb/"+str(img_number2) + ".jpg", cv2_img)
        cv2.imwrite(all_rgb + str(img_number2) + ".jpg", cv2_img)
        # print("image storing;"+str(img_number2))
        img_number2 += 2
        condition["cb2 storing"] = 0
    # 中止
    else:
        condition["cb2 storing"] = -1
        # rospy.loginfo("CB2 stocking img "+str(img_number2))
        cv2.imwrite(all_rgb + str(img_number2) + ".jpg", cv2_img)
        stock[str(img_number2)] = cv2_img

        img_number2 += 2


def wait_until(condition_):
    while (condition["cb1 storing"] == condition_ or condition["cb2 storing"] == condition_):
        pass


def switch_CB(loop):
    # loop終了
    global already_loop
    already_loop = 1

    # NOTE Erase detected images
    # rospy.loginfo("file start is "+str(start2end["start"]))
    # rospy.loginfo("file end is "+str(start2end["end"]))
    for file_num in range(start2end["start"], start2end["end"]+1):
        os.remove(path + "rgb/"+str(file_num) + ".jpg")

    # NOTE Now store the images which were stored during loop closure detection

    if ("start" in start2end):
        # NOTE wait until both cb begins its work and now stock is fully stored
        wait_until(-1)
        # REVIEW 辞書のforループの回し方
        for index, img in stock.items():
            cv2.imwrite(path + "rgb/"+str(index) + ".jpg", img)

        for index, img in temp_stock.items():
            cv2.imwrite(path + "rgb/"+str(index) + ".jpg", img)

        stock.clear()
        temp_stock.clear()


def setup():
    shutil.rmtree(path)

    os.mkdir(path)
    os.mkdir(all_rgb)
    os.mkdir(path+"/rgb/")
    os.mkdir(path+"/sorted_rgb/")
    os.mkdir(path+"/feature_match/")
    os.mkdir(path+"/test/")
    os.mkdir(path+"/depth/")


def commandCB(event):

    # loop検知開始
    global already_loop, start2end

    wait_until(1)
    already_loop = 0
    wait_until(1)
    l = [img_number, img_number2]
    l_no = max(l)-2

    # NOTE 連続していないものをtemp_stockに避難させておく.

    # wait_until(1)
    # NOTE lはこれから作る予定の画像index
    for num in range(10**4):
        if (os.path.exists(path+"rgb/"+str(l_no-1)+".jpg")):
            break
        # NOTE なんかエラーが出るけど、うまく読み込めてはいるっぽい
        temp_stock[l_no] = cv2.imread(path+"rgb/"+str(l_no)+".jpg")
        os.remove(path+"rgb/"+str(l_no)+".jpg")
        l_no -= 2

    # NOTE loop closure nodeに画像が保存し終わったことを伝える
    msg.data = 1
    # NOTE ここで、loop nodeを起動
    files = glob.glob(path+"rgb/*")

    command.publish(msg)
    # loop nodeから検知終了の合図を待つ

    # NOTE detectの対象ファイルをまとめる [start:int,end:int]
    start2end["start"] = start2end["end"]+1
    start2end["end"] = l_no


if __name__ == '__main__':
    setup()
    rospy.init_node('image_listener', anonymous=True)

    rgb_topic = "/robot1/camera/color/image_raw"
    rgb_topic2 = "/robot2/camera/color/image_raw"

    ID_topic = "/robot1/rtabmap/info"
    ID_topic2 = "/robot2/rtabmap/info"

    rgb_sub = message_filters.Subscriber(rgb_topic, Image)
    rgb_sub2 = message_filters.Subscriber(rgb_topic2, Image)

    myid_sub = message_filters.Subscriber(ID_topic, Info)
    myid_sub2 = message_filters.Subscriber(ID_topic2, Info)

    button = rospy.Subscriber("loop", Int32, switch_CB)

    msg = Int32()
    command = rospy.Publisher("loop_start", Int32, queue_size=1)
    # NOTE  loop nodeが起動する頃には画像の保存が終了している必要がある

    # ts = message_filters.TimeSynchronizer([rgb_sub, myid_sub], 10)
    ts = message_filters.ApproximateTimeSynchronizer(
        [rgb_sub, myid_sub], 10, 0.1, allow_headerless=False)
    ts.registerCallback(callback)

    ts2 = message_filters.ApproximateTimeSynchronizer(
        [rgb_sub2, myid_sub2], 10, 0.1, allow_headerless=False)
    ts2.registerCallback(callback2)
    # rospy.Subscriber(image_topic, Image, rgb_callback)

    # NOTE 5秒に一度detection nodeを呼び込む
    while img_number < 8:
        pass
    rospy.Timer(rospy.Duration(5), commandCB)

    # REVIEW rospy spinが出ている時点でrate.sleepを用いて定時でpublishすることは不可能になる.
    rospy.spin()
