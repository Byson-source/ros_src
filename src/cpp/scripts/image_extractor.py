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

img_number=1
# Instantiate CvBridge
bridge = CvBridge()
state={}
dir_num=1

def callback(rgb, id):
    global img_number,state,dir_num

    cv2_img = bridge.imgmsg_to_cv2(rgb, "bgr8")
    cv2_img = cv2.resize(cv2_img, dsize=(512, 384))
    
    cv2.imwrite(path + str(dir_num)+"_rgb/"+str(img_number) + ".jpg", cv2_img)
    cv2.imwrite(all_rgb + str(img_number) + ".jpg", cv2_img)
    
    img_number += total_num
    
    state["first"]=1
    
    while (("second" not in state) or (state["second"]==0)):
        time.sleep(0.0001)
    
    if dir_num==1:dir_num=2
    else:dir_num=1
        
    int_msg=Int32()
    int_msg.data=1
    path_changer.publish(int_msg)
    rate=rospy.Rate(100)
    rate.sleep()
    

    # Define your image topic
    
def state_CB(id):
    global state
    state["second"]=1

rgb_topic = "/robot1/camera/rgb/image_raw"
ID_topic = "/robot1/rtabmap/info"
monitor_id="/robot2/rtabmap/info"
# # To store the images from camera

# Remove all image

rgb_sub = message_filters.Subscriber(rgb_topic, Image)

myid_sub = message_filters.Subscriber(ID_topic, Info)

id_monitor = rospy.Subscriber(monitor_id, Info, state_CB)

path_changer=rospy.Publisher("imagepath_change",Int32,queue_size=1)

# ts = message_filters.TimeSynchronizer([rgb_sub, myid_sub], 10)
ts=message_filters.ApproximateTimeSynchronizer([rgb_sub,myid_sub],10,0.1,allow_headerless=False)
ts.registerCallback(callback)
# rospy.Subscriber(image_topic, Image, rgb_callback)
rospy.spin()
