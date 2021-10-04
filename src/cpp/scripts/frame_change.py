

import sys
import os
import rosbag
from geometry_msgs.msg import TransformStamped
# PKG = 'bag_tools'  # this package name

# roslib.load_manifest(PKG)

INPUTDATABASE = "/home/ayumi/Documents/RTAB-Map/map1.bag"
OUTPUTDATABASE = "/home/ayumi/Documents/RTAB-Map/map1_changed.bag"
NAMESPACE = "robot1/"
NAME="/robot1"
topic_list = ["base_controller/odom", "base_scan", "data_throttled_camera_info",
              "data_throttled_image/compressed", "data_throttled_image_depth/compressedDepth"]
goal_frame = ["odom", "base_laser_link", "camera_rgb_optical_frame",
              "camera_rgb_optical_frame", "camera_rgb_optical_frame"]

for val in goal_frame:
    val = NAMESPACE+val


def change_frame_id(inbag, outbag, frame_lists, topic_lists):
    print("Please wait...")
    i=0
    outbag = rosbag.Bag(outbag, 'w')
    for frame_val in frame_lists:
        for topic, msg, t in rosbag.Bag(inbag, 'r').read_messages():
            if topic in topic_lists[i]:
                if msg._has_header:
                    msg.header.frame_id = frame_val
            # outbag.write(topic, msg, t)
        i+=1
        print(str(i)+"/5")
    for topic, msg, t in rosbag.Bag(inbag, 'r').read_messages():
            if topic in "tf":
                # if msg._has_transforms:
                # for val in msg:
                #     if 'frame_id: "' in val;
                for val in msg.transforms:
                    val.header.frame_id=NAME+val.header.frame_id
                    val.child_frame_id=NAME+val.child_frame_id

    outbag.write(topic, msg, t)
    
    print('Closing output bagfile and exit...')
    outbag.close()


change_frame_id(INPUTDATABASE, OUTPUTDATABASE, topic_list, goal_frame)
