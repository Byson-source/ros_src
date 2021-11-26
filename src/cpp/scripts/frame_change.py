

import sys
import os
import rosbag
from geometry_msgs.msg import TransformStamped
# PKG = 'bag_tools'  # this package name

# roslib.load_manifest(PKG)

INPUTDATABASE = "/home/ayumi/Documents/RTAB-Map/map2.bag"
OUTPUTDATABASE1 = "/home/ayumi/Documents/RTAB-Map/map2_1.bag"
OUTPUTDATABASE2 = "/home/ayumi/Documents/RTAB-Map/map2_2.bag"
OUTPUTDATABASE3 = "/home/ayumi/Documents/RTAB-Map/map2_3.bag"
OUTPUTDATABASE4 = "/home/ayumi/Documents/RTAB-Map/map2_4.bag"
OUTPUTDATABASE5 = "/home/ayumi/Documents/RTAB-Map/map2_5.bag"
OUTPUTDATABASE6 = "/home/ayumi/Documents/RTAB-Map/map2_6.bag"


out=[INPUTDATABASE,OUTPUTDATABASE1,OUTPUTDATABASE2,OUTPUTDATABASE3,OUTPUTDATABASE4, OUTPUTDATABASE5]

NAMESPACE = "robot2/"
NAME="/robot2"
topic_list = ["base_controller/odom", "base_scan", "data_throttled_camera_info",
              "data_throttled_image/compressed", "data_throttled_image_depth/compressedDepth"]
goal_frame = ["odom", "base_laser_link", "camera_rgb_optical_frame",
              "camera_rgb_optical_frame", "camera_rgb_optical_frame"]



def change_frame_id(inbag, outbag, frame_val, topic_):
    print("Please wait...")
    i=0
    outbag = rosbag.Bag(outbag, 'w')
    for topic, msg, t in rosbag.Bag(inbag, 'r').read_messages():
        if topic_ in topic:
            msg.child_frame_id = NAMESPACE+frame_val
            print(msg.child_frame_id)
        outbag.write(topic, msg, t)
    i+=1
    outbag.close()

def two_round(outbag2):
    outbag2=rosbag.Bag(outbag2,'w')
    for topic, msg, t in rosbag.Bag(OUTPUTDATABASE5, 'r').read_messages():
            if topic in "tf":
                for val in msg.transforms:
                    val.header.frame_id=NAME+val.header.frame_id
                    val.child_frame_id=NAME+val.child_frame_id
            outbag2.write(topic, msg, t)
    
    print('Closing output bagfile and exit...')
    outbag2.close()
# i=0
# for frame_val in goal_frame:
#     change_frame_id(out[i], out[i+1], frame_val, topic_list[i])
#     i+=1
# two_round(OUTPUTDATABASE6)
change_frame_id("/home/ayumi/Documents/RTAB-Map/out2_.bag", "/home/ayumi/Documents/RTAB-Map/out2.bag", 
"base_footprint", "/base_controller/odom")