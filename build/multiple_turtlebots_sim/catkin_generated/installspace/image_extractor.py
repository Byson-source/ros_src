#! /usr/bin/python
# Copyright (c) 2015, Rethink Robotics, Inc.

# Using this CvBridge Tutorial for converting
# ROS images to OpenCV2 images
# http://wiki.ros.org/cv_bridge/Tutorials/ConvertingBetweenROSImagesAndOpenCVImagesPython

# Using this OpenCV2 tutorial for saving Images:
# http://opencv-python-tutroals.readthedocs.org/en/latest/py_tutorials/py_gui/py_image_display/py_image_display.html

# rospy for the subscriber
import rospy
# ROS Image message
from sensor_msgs.msg import Image
# ROS Image message -> OpenCV2 image converter
from cv_bridge import CvBridge, CvBridgeError
# OpenCV2 for saving an image
from std_msgs.msg import String

import cv2
import os

# Instantiate CvBridge
bridge = CvBridge()
mypath="/home/ayumi/Documents/tohoku_uni/CLOVERs/1database/"
dir_number=0
img_number=0
switch=1

def dir_callback(data):
    global mypath,img_number
    mypath=data.data
    img_number=0

def image_callback(msg):
    global file_number,img_number,mypath,switch

    if switch==1:
        cv2_img = bridge.imgmsg_to_cv2(msg, "bgr8")
        cv2_img=cv2.resize(cv2_img,dsize=(512,384))
        # cv2.imwrite('/home/ayumi/test'+str(file_number)+"/"+str(img_number)+"camera_image.jpeg", cv2_img)
        cv2.imwrite(mypath+ str(img_number) + ".jpg", cv2_img)
        #Depth image はpng!
        img_number+=1
    else:
        print("Image extractor is waiting for rtabmap-reprocess...")

def rest_callback(data):
    global switch
    if data.data=="Still rtabmap-reprocess running":
        switch=0
    else:
        switch=1



if __name__ == '__main__':
    # rate=rospy.Rate(10)

    rospy.init_node('image_listener', anonymous=True)
    # Define your image topic
    image_topic = "/camera/rgb/image_raw"
    #To store the images from camera
    rospy.Subscriber(image_topic, Image, image_callback)
    #In order to know that the location is changed as soon as loop-closure detection occurs
    rospy.Subscriber("dir_info",String,dir_callback);
    #In order to stop storing images during rtabmap-reprocess
    rospy.Subscriber("take_rest",String,rest_callback)



    rospy.spin()