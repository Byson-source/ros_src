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

depth_path = "/home/ayumi/Documents/tohoku_uni/CLOVERs/images/depth-image/"

rospy.init_node('image_listener', anonymous=True)

robot_number = rospy.get_param("~robot_number")
total_num = rospy.get_param("~total")
robot_No=rospy.get_param("~robot_name")

# Instantiate CvBridge
bridge = CvBridge()
dir_number = 0
switch = 1
depth_number=robot_number

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
    global depth_number,switch

    if switch == 1:
        cv2_img_depth=bridge.imgmsg_to_cv2(msg,"passthrough")
        # cv2_img_depth=cv2.applyColorMap(cv2.convertScaleAbs(cv2_img_depth, alpha=0.03),cv2.COLORMAP_JET)
        
        
        
        cv2.imwrite(depth_path + str(depth_number) +".png", cv2_img_depth)
        # Depth image はpng!
        depth_number += total_num

        time.sleep(1);


def rest_callback(data):
    global switch
    if data.data == 1:
        switch = 0
    else:
        switch = 1

# Define your image topic

depth_topic = "/robot"+str(robot_No)+"/camera/depth/image_raw"
# # To store the images from camera
rospy.Subscriber(depth_topic, Image, image_callback)
# # In order to know that the location is changed as soon as loop-closure detection occurs
rospy.Subscriber("dir_info", String, dir_callback)
# # In order to stop storing images during rtabmap-reprocess
rospy.Subscriber("take_rest", Int32, rest_callback)

chatter=rospy.Publisher("current_image",String,queue_size=1)

rospy.spin()