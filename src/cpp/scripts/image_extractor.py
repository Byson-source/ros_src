# FIXME You need to get the feedback from loop_closure node that it finishes reading two images in that directory
# NOTE This node works only for robot1. And gives commands to any_image_extractorand loop closure node
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

shutil.rmtree(path)

os.mkdir(path)
os.mkdir(all_rgb)
os.mkdir(path+"/1_rgb/")
os.mkdir(path+"/2_rgb/")

rospy.init_node('image_listener', anonymous=True)

total_num = rospy.get_param("~total")

img_number = 1
# Instantiate CvBridge
bridge = CvBridge()
state = {}
dir_num = 1

iteration = 0


def callback(rgb, id):
    global img_number, state, dir_num, already_loop, iteration, path_changer

    iteration += 1

    cv2_img = bridge.imgmsg_to_cv2(rgb, "bgr8")
# FIXME Maybe should change to (640,480)
    cv2_img = cv2.resize(cv2_img, dsize=(512, 384))

    cv2.imwrite(path + str(dir_num)+"_rgb/"+str(img_number) + ".jpg", cv2_img)
    cv2.imwrite(all_rgb + str(img_number) + ".jpg", cv2_img)


    state["first"] = 1

    while (("second" not in state) or (state["second"] == 0)):
        time.sleep(0.0001)

    int_msg = Int32()
    int_msg.data = 1

    # while(already_loop==0):
    #     time.sleep(0.0001)

    # Remove stored images
    if (iteration > 1):
        if dir_num == 1:
            os.remove(path + "2_rgb/"+str(img_number-2) + ".jpg")
            os.remove(path + "2_rgb/"+str(img_number-1) + ".jpg")
        if dir_num == 2:
            os.remove(path + "1_rgb/"+str(img_number-2) + ".jpg")
            os.remove(path + "1_rgb/"+str(img_number-1) + ".jpg")

    path_changer.publish(int_msg)
    img_number += total_num
    if dir_num == 1:
        dir_num = 2
    else:
        dir_num = 1

    rate = rospy.Rate(100)
    rate.sleep()
    # Define your image topic


def state_CB(id):
    global state
    state["second"] = 1


# NOTE wait until detection finishes
def loop_CB(loop):
    global already_loop
    already_loop=1

if __name__ == '__main__':

    rgb_topic = "/robot1/camera/rgb/image_raw"
    ID_topic = "/robot1/rtabmap/info"
    # FIXME Fix to make it more easy to use for any-number usage
    monitor_id = "/robot2/rtabmap/info"
    # # To store the images from camera

    # Remove all image

    rgb_sub = message_filters.Subscriber(rgb_topic, Image)

    myid_sub = message_filters.Subscriber(ID_topic, Info)

    detect_finish=rospy.Subscriber("loop",String,loop_CB)

    id_monitor = rospy.Subscriber(monitor_id, Info, state_CB)

    path_changer = rospy.Publisher("imagepath_change", Int32, queue_size=1)

    # ts = message_filters.TimeSynchronizer([rgb_sub, myid_sub], 10)
    ts = message_filters.ApproximateTimeSynchronizer(
        [rgb_sub, myid_sub], 10, 0.1, allow_headerless=False)
    ts.registerCallback(callback)
    # rospy.Subscriber(image_topic, Image, rgb_callback)
    rospy.spin()
