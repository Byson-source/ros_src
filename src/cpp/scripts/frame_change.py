import sys
import os
import rosbag
from geometry_msgs.msg import TransformStamped
# PKG = 'bag_tools'  # this package name

# roslib.load_manifest(PKG)
NAMESPACE = "robot1/"
NAME="/robot1"
topic_list = ["camera/depth/image",
              "camera/rgb/camera_info", "camera/rgb/image_color/compressed"]
goal_frame = "camera_rgb_optical_frame"



def change_frame_id(inbag, outbag):
    print("Please wait...")
    i=0
    outbag = rosbag.Bag(outbag, 'w')
    for topic, msg, t in rosbag.Bag(inbag, 'r').read_messages():
        if ((topic_list[0] in topic) or (topic_list[1] in topic) or (topic_list[2] in topic)):
            msg.child_frame_id = NAMESPACE+goal_frame
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
i=0
for frame_val in goal_frame:
    change_frame_id(out[i], out[i+1], frame_val, topic_list[i])
    i+=1
    
    
two_round(OUTPUTDATABASE6)
change_frame_id("/home/ayumi/Documents/bag_dataset/Arc.bag",
                "/home/ayumi/Documents/bag_dataset/Arc1.bag")