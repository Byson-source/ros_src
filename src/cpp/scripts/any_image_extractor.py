#! /usr/bin/python
import rospy

import os
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
import os

path = "/home/ayumi/Documents/tohoku_uni/CLOVERs/images/"

all_rgb = "/home/ayumi/Documents/tohoku_uni/CLOVERs/images/all_rgb/"


total_num = 2
img_number = 2
dir_num=1
# Instantiate CvBridge
bridge = CvBridge()
state=1
state_beta=1
iteration=0

def callback(rgb, id):
    global img_number,state,state_beta,iteration,dir_num
    
    if state_beta==1:
        cv2_img = bridge.imgmsg_to_cv2(rgb, "bgr8")
        cv2_img = cv2.resize(cv2_img, dsize=(512, 384))

        cv2.imwrite(path + str(state)+"_rgb/"+str(img_number) + ".jpg", cv2_img)
        cv2.imwrite(all_rgb + str(img_number) + ".jpg", cv2_img)
        
        state_beta=0
    # こちらのノードのほうが早く動くと、同じディレクトリに連続で写真を保存してしまうため
        iteration+=1

        while not (os.path.exists(path + str(dir_num)+"_rgb/"+str(img_number-1) + ".jpg")):
            pass

        if (iteration > 1):
            if dir_num == 1:
                os.remove(path + "2_rgb/"+str(img_number-2) + ".jpg")
            if dir_num == 2:
                os.remove(path + "1_rgb/"+str(img_number-2) + ".jpg")

        img_number += total_num
    else:
        pass
    

def changeCB(data):
    global state,state_beta,iteration
    state_beta=1
    if (state==1 and iteration>0):
        state=2
    else:
        state=1

if __name__=='__main__':

    rospy.init_node('image_listener', anonymous=True)
    if not (os.path.isdir(all_rgb) or os.path.isdir(path+"1_rgb")
            or os.path.isdir(path+"2_rgb")):
        time.sleep(0.0001)
    rgb_topic = "/robot"+str(img_number)+"/camera/rgb/image_raw"
    ID_topic = "/robot"+str(img_number)+"/rtabmap/info"
    # # To store the images from camera
    rgb_sub = message_filters.Subscriber(rgb_topic, Image)
    myid_sub = message_filters.Subscriber(ID_topic, Info)

    path_changer=rospy.Subscriber("imagepath_change",Int32,changeCB)

    # ts = message_filters.TimeSynchronizer([rgb_sub, myid_sub], 10)
    ts=message_filters.ApproximateTimeSynchronizer([rgb_sub,myid_sub],10,0.1,allow_headerless=False)
    ts.registerCallback(callback)
    # rospy.Subscriber(image_topic, Image, rgb_callback)
    rospy.spin()