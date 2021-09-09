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
mypath="/home/ayumi/test"
file_number=0
img_number=0

def make_dir(data):
    global file_number
    global img_number
    while True:
        file_number+=1
        if os.path.exists(mypath+str(file_number)):
            continue
        else:
            os.mkdir(mypath+str(file_number))
            img_number=0

def image_callback(msg):
    global file_number
    global img_number
    cv2_img = bridge.imgmsg_to_cv2(msg, "bgr8")
    # cv2.imwrite('/home/ayumi/test'+str(file_number)+"/"+str(img_number)+"camera_image.jpeg", cv2_img)
    cv2.imwrite('/home/ayumi/test/' + str(img_number) + "camera_image.jpeg", cv2_img)
    img_number+=1


def main():
    rospy.init_node('image_listener')
    # Define your image topic
    image_topic = "/camera/rgb/image_raw"
    # Set up your subscriber and define its callback
    rospy.Subscriber(image_topic, Image, image_callback)
    # Spin until ctrl + c
    rospy.Subscriber("LoopClosureDetection",String,make_dir)

    rospy.spin()

if __name__ == '__main__':
    main()