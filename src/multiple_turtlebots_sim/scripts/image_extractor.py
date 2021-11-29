#! /usr/bin/python
import rospy
# ROS Image message
from sensor_msgs.msg import Image
# ROS Image message -> OpenCV2 image converter
from cv_bridge import CvBridge, CvBridgeError
# OpenCV2 for saving an image

import message_filters

import cv2
import time

rgb_path = "/home/ayumi/Documents/tohoku_uni/CLOVERs/images/1-image/"
depth_path = "/home/ayumi/Documents/tohoku_uni/CLOVERs/images/depth-image/"

rospy.init_node('image_listener', anonymous=True)

robot_number = rospy.get_param("~robot_number")
total_num = rospy.get_param("~total")
img_number=robot_number

# Instantiate CvBridge
bridge = CvBridge()

def callback(rgb,depth):
    global img_number
    
    cv2_img = bridge.imgmsg_to_cv2(rgb, "bgr8")
    cv2_img = cv2.resize(cv2_img, dsize=(512, 384))
    cv2.imwrite(rgb_path + str(img_number) + ".jpg", cv2_img)
    
    cv2_img_depth=bridge.imgmsg_to_cv2(depth,"passthrough")
    cv2.imwrite(depth_path + str(img_number) +".png", cv2_img_depth)
    # Depth image はpng!
    img_number += total_num

    time.sleep(1/5);

# Define your image topic

rgb_topic = "/robot"+str(robot_number)+"/camera/rgb/image_raw"
depth_topic = "/robot"+str(robot_number)+"/camera/depth/image_raw"
# # To store the images from camera
rgb_sub=message_filters.Subscriber(rgb_topic,Image)
depth_sub=message_filters.Subscriber(depth_topic,Image)

# ts=message_filters.ApproximateTimeSynchronizer([rgb_sub,depth_sub],10,0.1,allow_headerless=False)
ts=message_filters.ApproximateTimeSynchronizer([rgb_sub,depth_sub],10,0.1,allow_headerless=False)
ts.registerCallback(callback)
# rospy.Subscriber(image_topic, Image, rgb_callback)
rospy.spin()