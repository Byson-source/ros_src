# TODO You need to get the feedback from loop_closure node that tells when to start and to restart again.
# TODO lop検出中に来た画像を保留しておくこと。
# FIXME This works only for few robots. If you want, convert this into C++.
#! /usr/bin/python
import rospy

import os
import shutil
# ROS Image message
from sensor_msgs.msg import Image
# ROS Image message -> OpenCV2 image converter
from cv_bridge import CvBridge, CvBridgeError
# OpenCV2 for saving an image
from rtabmap_ros.msg import Info
from std_msgs.msg import String

from std_msgs.msg import Int32

import message_filters

import cv2
import time

path = "/home/ayumi/Documents/tohoku_uni/CLOVERs/images/"

all_rgb = "/home/ayumi/Documents/tohoku_uni/CLOVERs/images/all_rgb/"

total_num = 2

img_number = 1
img_number2=2

already_loop=1

stock={}
start2end={"end":0}
# start ,end,previous_end

# index:img

# Instantiate CvBridge
bridge = CvBridge()

def callback(rgb, id):
    global img_number,img_number2,already_loop,stock,start2end

    cv2_img = bridge.imgmsg_to_cv2(rgb, "bgr8")
# FIXME Maybe should change to (640,480)
    cv2_img = cv2.resize(cv2_img, dsize=(512, 384))
    if already_loop==1:
        cv2.imwrite(path + "rgb/"+str(img_number) + ".jpg", cv2_img)
        cv2.imwrite(all_rgb + str(img_number) + ".jpg", cv2_img)

        img_number += total_num
    else:
        start2end["start"]=start2end["end"]+1
        if img_number>img_number2:
            start2end["end"]=img_number
        else:
            start2end["end"]=img_number2

        stock[str(img_number)]=cv2_img


def callback2(rgb, id):
    global img_number2, already_loop,stock

    cv2_img = bridge.imgmsg_to_cv2(rgb, "bgr8")
# FIXME Maybe should change to (640,480)
    cv2_img = cv2.resize(cv2_img, dsize=(512, 384))

    if already_loop==1:
        cv2.imwrite(path + "rgb/"+str(img_number2) + ".jpg", cv2_img)
        cv2.imwrite(all_rgb + str(img_number2) + ".jpg", cv2_img)

        img_number2 += total_num
    else:
        stock[str(img_number2)]=cv2_img

    

# NOTE wait until detection finishes
def loop_CB(loop):
    global already_loop,stock,start2end
    # 再開
    if already_loop==0:
        for file_num in range(start2end["start"],start2end["end"]):
            os.remove(path + "rgb/"+str(file_num) + ".jpg")
            
        for index,img in stock:
            cv2.imwrite(path + "rgb/"+str(index) + ".jpg", img)
        already_loop=1

    else:
    # 中止
        already_loop=0



if __name__ == '__main__':

    shutil.rmtree(path)

    os.mkdir(path)
    os.mkdir(all_rgb)
    os.mkdir(path+"/rgb/")

    rospy.init_node('image_listener', anonymous=True)

    # NOTE Input topics as many as you want
    rgb_topic = "/robot1/camera/rgb/image_raw"
    rgb_topic2="/robot2/camera/rgb/image_raw"

    ID_topic = "/robot1/rtabmap/info"
    # FIXME Fix to make it more easy to use for any-number usage
    ID_topic2 = "/robot2/rtabmap/info"
    # # To store the images from camera

    # Remove all image

    rgb_sub = message_filters.Subscriber(rgb_topic, Image)
    rgb_sub2 = message_filters.Subscriber(rgb_topic2, Image)

    myid_sub = message_filters.Subscriber(ID_topic, Info)
    myid_sub2 = message_filters.Subscriber(ID_topic2, Info)

    detect_note=rospy.Subscriber("loop",String,loop_CB)


    # ts = message_filters.TimeSynchronizer([rgb_sub, myid_sub], 10)
    ts = message_filters.ApproximateTimeSynchronizer(
        [rgb_sub, myid_sub], 10, 0.1, allow_headerless=False)
    ts.registerCallback(callback)

    ts2 = message_filters.ApproximateTimeSynchronizer(
        [rgb_sub2, myid_sub2], 10, 0.1, allow_headerless=False)
    ts2.registerCallback(callback2)
    # rospy.Subscriber(image_topic, Image, rgb_callback)
    rospy.spin()
