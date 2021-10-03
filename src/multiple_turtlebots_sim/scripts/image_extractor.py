#! /usr/bin/python
import rospy
# ROS Image message
from sensor_msgs.msg import Image
# ROS Image message -> OpenCV2 image converter
from cv_bridge import CvBridge, CvBridgeError
# OpenCV2 for saving an image
from std_msgs.msg import String

from std_msgs.msg import Int32

import cv2
import os
import time


FIRST_IMAGE_PATH = "/home/ayumi/Documents/tohoku_uni/CLOVERs/1-image/"

rospy.init_node('image_listener', anonymous=True)

robot_number = rospy.get_param("~robot_number")
total_num = rospy.get_param("~total")

# Instantiate CvBridge
bridge = CvBridge()
mypath = FIRST_IMAGE_PATH
dir_number = 0
switch = 1
img_number=robot_number

def dir_callback(data):
    global mypath, img_number
    mypath = data.data

    if not os.path.exists(mypath):
        os.mkdir(mypath)
        rospy.loginfo("New image path was sucessfully created!")
    else:
        rospy.loginfo("The new image path already exists. Now new one is created.")
        os.rmdir(mypath)
        os.mkdir(mypath)

    img_number = robot_number


def image_callback(msg):
    global img_number, mypath, switch

    if switch == 1:
        cv2_img = bridge.imgmsg_to_cv2(msg, "bgr8")
        cv2_img = cv2.resize(cv2_img, dsize=(512, 384))
        # cv2.imwrite('/home/ayumi/test'+str(file_number)+"/"+str(img_number)+"camera_image.jpeg", cv2_img)
        cv2.imwrite(mypath + str(img_number) + ".jpg", cv2_img)
        # Depth image はpng!
        img_number += total_num

        time.sleep(1);
    else:
        rospy.loginfo("Image extractor is waiting for rtabmap-reprocess...")


def rest_callback(data):
    global switch
    if data.data == 1:
        switch = 0
    else:
        switch = 1

# Define your image topic

image_topic = "/robot"+str(robot_number)+"/camera/rgb/image_raw"
# # To store the images from camera
rospy.Subscriber(image_topic, Image, image_callback)
# # In order to know that the location is changed as soon as loop-closure detection occurs
rospy.Subscriber("dir_info", String, dir_callback)
# # In order to stop storing images during rtabmap-reprocess
rospy.Subscriber("take_rest", Int32, rest_callback)

rospy.spin()
