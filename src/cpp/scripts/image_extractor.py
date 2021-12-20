# FIXME You need to get the feedback from loop_closure node that it finishes reading two images in that directory
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
# Instantiate CvBridge
bridge = CvBridge()
state = {}

dir_num = 1
dir_num2=1

iteration = 0

former=0


def callback(rgb, id):
    global img_number, state, dir_num, iteration,former

    iteration += 1

    cv2_img = bridge.imgmsg_to_cv2(rgb, "bgr8")
# FIXME Maybe should change to (640,480)
    cv2_img = cv2.resize(cv2_img, dsize=(512, 384))

    cv2.imwrite(path + str(dir_num)+"_rgb/"+str(img_number) + ".jpg", cv2_img)
    cv2.imwrite(all_rgb + str(img_number) + ".jpg", cv2_img)


    state["first"] = 1
    # まだ相方の写真がない時
    if("second" not in state):
        while ("second" not in state):
            time.sleep(0.0000000000001)

        if (iteration > 1):
            if dir_num == 1:
                os.remove(path + "2_rgb/"+str(img_number-2) + ".jpg")
            if dir_num == 2:
                os.remove(path + "1_rgb/"+str(img_number-2) + ".jpg")

        img_number += total_num
        # もう既にある時
        if dir_num == 1:
            dir_num = 2
        else:
            dir_num = 1
    else:
        if (iteration > 1):
            if dir_num == 1:
                os.remove(path + "2_rgb/"+str(img_number-2) + ".jpg")
            if dir_num == 2:
                os.remove(path + "1_rgb/"+str(img_number-2) + ".jpg")

        img_number += total_num



def callback2(rgb, id):
    global img_number2, state, dir_num, iteration,former

    cv2_img = bridge.imgmsg_to_cv2(rgb, "bgr8")
# FIXME Maybe should change to (640,480)
    cv2_img = cv2.resize(cv2_img, dsize=(512, 384))


    cv2.imwrite(path + str(dir_num)+"_rgb/"+str(img_number2) + ".jpg", cv2_img)
    cv2.imwrite(all_rgb + str(img_number2) + ".jpg", cv2_img)

    state["second"] = 1
    if("first" not in state):
        while ("first" not in state):
            time.sleep(0.0000000000001)

        if (iteration > 1):
            if dir_num == 1:
                os.remove(path + "2_rgb/"+str(img_number2-2) + ".jpg")
            if dir_num == 2:
                os.remove(path + "1_rgb/"+str(img_number2-2) + ".jpg")

        img_number2 += total_num
        if dir_num == 1:
            dir_num = 2
        else:
            dir_num = 1
            
    else:
        if (iteration > 1):
            if dir_num == 1:
                os.remove(path + "2_rgb/"+str(img_number2-2) + ".jpg")
            if dir_num == 2:
                os.remove(path + "1_rgb/"+str(img_number2-2) + ".jpg")

        img_number2 += total_num


    state.clear()

# NOTE wait until detection finishes
def loop_CB(loop):
    global already_loop
    already_loop=1



if __name__ == '__main__':

    shutil.rmtree(path)

    os.mkdir(path)
    os.mkdir(all_rgb)
    os.mkdir(path+"/1_rgb/")
    os.mkdir(path+"/2_rgb/")

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

    detect_finish=rospy.Subscriber("loop",String,loop_CB)


    # ts = message_filters.TimeSynchronizer([rgb_sub, myid_sub], 10)
    ts = message_filters.ApproximateTimeSynchronizer(
        [rgb_sub, myid_sub], 10, 0.1, allow_headerless=False)
    ts.registerCallback(callback)

    ts2 = message_filters.ApproximateTimeSynchronizer(
        [rgb_sub2, myid_sub2], 10, 0.1, allow_headerless=False)
    ts2.registerCallback(callback2)
    # rospy.Subscriber(image_topic, Image, rgb_callback)
    rospy.spin()
