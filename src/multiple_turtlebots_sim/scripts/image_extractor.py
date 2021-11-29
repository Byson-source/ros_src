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


first_path = "/home/ayumi/Documents/tohoku_uni/CLOVERs/images/1-image/"
second_path = "/home/ayumi/Documents/tohoku_uni/CLOVERs/images/2-image/"
all_path="/home/ayumi/Documents/tohoku_uni/CLOVERs/images/all-image/"
depth_path = "/home/ayumi/Documents/tohoku_uni/CLOVERs/images/depth-image/"

rospy.init_node('image_listener', anonymous=True)

robot_number = rospy.get_param("~robot_number")
total_num = rospy.get_param("~total")

# Instantiate CvBridge
bridge = CvBridge()
mypath = FIRST_IMAGE_PATH
dir_number = 0
state=0
depth_number,img_number=robot_number,robot_number

def rgb_callback(msg):
    global img_number, mypath,state
    cv2_img = bridge.imgmsg_to_cv2(msg, "bgr8")
    cv2_img = cv2.resize(cv2_img, dsize=(512, 384))
    if state==0:
        
        cv2.imwrite(first_path + str(img_number) + ".jpg", cv2_img)
        state=1
    else:
        cv2.imwrite(second_path + str(img_number) + ".jpg", cv2_img)
        state=0
    # Depth image はpng!
    img_number += total_num
    
    chatter.publish(mypath + str(img_number) + ".jpg")

    time.sleep(1/5);



def image_callback(msg):
    global depth_number,switch

    if switch == 1:
        cv2_img_depth=bridge.imgmsg_to_cv2(msg,"passthrough")
        # cv2_img_depth=cv2.applyColorMap(cv2.convertScaleAbs(cv2_img_depth, alpha=0.03),cv2.COLORMAP_JET)
        
        
        
        cv2.imwrite(depth_path + str(depth_number) + ".png", cv2_img_depth)
        # Depth image はpng!
        depth_number += total_num

        time.sleep(1/5);


def rest_callback(data):
    global switch
    if data.data == 1:
        switch = 0
    else:
        switch = 1

# Define your image topic

image_topic = "/robot"+str(robot_No)+"/camera/rgb/image_raw"
depth_topic = "/robot"+str(robot_No)+"/camera/depth/image_raw"
# # To store the images from camera
rospy.Subscriber(image_topic, Image, rgb_callback)
rospy.Subscriber(depth_topic, Image, image_callback)
# # In order to know that the location is changed as soon as loop-closure detection occurs
rospy.Subscriber("dir_info", String, dir_callback)
# # In order to stop storing images during rtabmap-reprocess
rospy.Subscriber("take_rest", Int32, rest_callback)

chatter=rospy.Publisher("current_image",String,queue_size=1)

rospy.spin()
