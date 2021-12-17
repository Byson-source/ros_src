#! /usr/bin/python
import rospy
# ROS Image message
from sensor_msgs.msg import Image
# ROS Image message -> OpenCV2 image converter
from cv_bridge import CvBridge, CvBridgeError
# OpenCV2 for saving an image
from rtabmap_ros.msg import Info

import message_filters

import cv2
import time

path = "/home/ayumi/Documents/tohoku_uni/CLOVERs/images/"
all_rgb = "/home/ayumi/Documents/tohoku_uni/CLOVERs/images/all_rgb/"

rospy.init_node('image_listener', anonymous=True)

robot_number = rospy.get_param("~robot_number")
total_num = rospy.get_param("~total")

img_number=robot_number
img_number = robot_number


# Instantiate CvBridge
bridge = CvBridge()
state={}
dir=1

def callback(rgb, id):
    global img_number,state

    cv2_img = bridge.imgmsg_to_cv2(rgb, "bgr8")
    cv2_img = cv2.resize(cv2_img, dsize=(512, 384))
    
    cv2.imwrite(path + str(state)+"_rgb"+str(img_number) + ".jpg", cv2_img)
    cv2.imwrite(all_rgb + str(img_number) + ".jpg", cv2_img)
    
    img_number += total_num
    
    state["first"]=1
    
    while (("second" not in state) or (state["second"]==0)):
        time.sleep(0.0000001)
    
    # Define your image topic
    
def state_CB(id):
    global state
    state["second"]=1

rgb_topic = "/robot1/camera/rgb/image_raw"
ID_topic = "/robot1/rtabmap/info"

rgb_topic2 = "/robot2/camera/rgb/image_raw"
ID_topic2 = "/robot2/rtabmap/info"
# # To store the images from camera
rgb_sub = message_filters.Subscriber(rgb_topic, Image)
rgb_sub2 = message_filters.Subscriber(rgb_topic2, Image)

myid_sub = message_filters.Subscriber(ID_topic, Info)
myid_sub2 = message_filters.Subscriber(ID_topic2, Info)

id_monitor = rospy.Subscriber(ID_topic, Info, state_CB)

ts = message_filters.TimeSynchronizer([rgb_sub, myid_sub], 10)
ts.registerCallback(callback)

ts2 = message_filters.TimeSynchronizer([rgb_sub2, myid_sub2], 10)
ts2.registerCallback(callback2)
# rospy.Subscriber(image_topic, Image, rgb_callback)
rospy.spin()
