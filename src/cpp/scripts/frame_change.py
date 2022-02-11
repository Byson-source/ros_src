import sys
import os
import rosbag
from geometry_msgs.msg import TransformStamped
# PKG = 'bag_tools'  # this package name
# NAME="/robot1"
topic_list = ["/camera/aligned_depth_to_color/camera_info",
              "/camera/aligned_depth_to_color/image_raw",
              "/camera/color/camera_info",
              "/camera/color/image_raw",
              ]


topic_frame = "camera_color_optical_frame"

secs_stamp = {"/camera/aligned_depth_to_color/camera_info": [],
              "/camera/aligned_depth_to_color/image_raw": [],
              "/camera/color/camera_info": [],
              "/camera/color/image_raw": []}

nsecs_stamp = {"/camera/aligned_depth_to_color/camera_info": [],
               "/camera/aligned_depth_to_color/image_raw": [],
               "/camera/color/camera_info": [],
               "/camera/color/image_raw": []}

secs_tf_dict = {"camera_depth_frame": [], "camera_aligned_depth_to_color_frame": [], "base_link": [],
                "camera_link": {"camera_color_frame": [], "camera_aligned_depth_to_color_frame": [], "camera_depth_frame": []}}
nsecs_tf_dict = secs_tf_dict = {"camera_depth_frame": [], "camera_aligned_depth_to_color_frame": [], "base_link": [],
                                "camera_link": {"camera_color_frame": [], "camera_aligned_depth_to_color_frame": [], "camera_depth_frame": []}}


def change_frame_id(inbag, outbag, header):
    print("Please wait...")
    i = 0
    outbag = rosbag.Bag(outbag, 'w')
    for topic, msg, t in rosbag.Bag(inbag, 'r').read_messages():
        if ((topic_list[0] in topic) or (topic_list[1] in topic) or (topic_list[2] in topic) or (topic_list[3] in topic)):
            msg.header.frame_id = header+topic_frame
            # print(msg.header.frame_id)
        outbag.write(topic, msg, t)
    i += 1
    outbag.close()


def two_round(outbag1, outbag2, header):
    outbag2 = rosbag.Bag(outbag2, 'w')
    for topic, msg, t in rosbag.Bag(outbag1, 'r').read_messages():
        if "tf" in topic:
            for val in msg.transforms:
                val.header.frame_id = header+val.header.frame_id
                val.child_frame_id = header+val.child_frame_id
        outbag2.write(topic, msg, t)

    print('Closing output bagfile and exit...')
    outbag2.close()

# NOTE choose longer bag as a referred bag!


def convert(path, header):
    path_1 = "/home/ayumi/Documents/tohoku_uni/CLOVERs/datasets/field_test/20220203_indoor_test/bags/a.bag"
    change_frame_id(path, path_1, header)
    two_round(path_1, path, header)


home_path = "/home/ayumi/Documents/tohoku_uni/CLOVERs/datasets/field_test/2022_0211_indoor/"


convert(home_path+"record_testr1.bag", "robot1/")
convert(home_path+"record_testb1.bag", "robot2/")
# convert(home_path+"record_b3.bag", "robot2/")
# convert(home_path+"record_p1.bag", "robot2/")
# convert(home_path+"record_p2.bag", "robot2/")
# convert(home_path+"record_p3.bag", "robot2/")
# convert(home_path+"record_p1.bag", "robot2/")
# convert(home_path+"record_p2.bag", "robot2/")
# convert(home_path+"record_p3.bag", "robot2/")
