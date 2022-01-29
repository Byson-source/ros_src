import sys
import os
import rosbag
from geometry_msgs.msg import TransformStamped
# PKG = 'bag_tools'  # this package name

# roslib.load_manifest(PKG)
NAMESPACE = "robot1/"
# NAME="/robot1"
topic_list = ["/camera/aligned_depth_to_color/camera_info",
              "/camera/aligned_depth_to_color/image_raw",
              "/camera/color/camera_info",
              "/camera/color/image_raw",
              ]

topic_frame = "camera_color_optical_frame"
# topic_list = ["camera/depth/image",
#               "camera/rgb/camera_info", "camera/rgb/image_color/compressed"]
start_frame = ["camera_link", "camera_depth_frame", "camera_link",
               "camera_aligned_depth_to_color_frame", "camera_link"]
goal_frame = ["camera_depth_frame", "camera_depth_optical_frame", "camera_color_frame",
              "camera_color_optical_frame", "camera_aligned_depth_to_color_frame"]


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
        if topic in "tf_static":
            for val in msg.transforms:
                val.header.frame_id = header+val.header.frame_id
                val.child_frame_id = header+val.child_frame_id
        outbag2.write(topic, msg, t)

    print('Closing output bagfile and exit...')
    outbag2.close()


change_frame_id("/home/ayumi/Documents/tohoku_uni/CLOVERs/datasets/field_test/20220128_indoor_test/bag/record_g1.bag",
                "/home/ayumi/Documents/tohoku_uni/CLOVERs/datasets/field_test/20220128_indoor_test/bag/record_gg1.bag", "robot1/")
two_round("/home/ayumi/Documents/tohoku_uni/CLOVERs/datasets/field_test/20220128_indoor_test/bag/record_gg1.bag",
          "/home/ayumi/Documents/tohoku_uni/CLOVERs/datasets/field_test/20220128_indoor_test/bag/record_g1.bag", "robot1/")

change_frame_id("/home/ayumi/Documents/tohoku_uni/CLOVERs/datasets/field_test/20220128_indoor_test/bag/record_g2.bag",
                "/home/ayumi/Documents/tohoku_uni/CLOVERs/datasets/field_test/20220128_indoor_test/bag/record_gg1.bag", "robot1/")
two_round("/home/ayumi/Documents/tohoku_uni/CLOVERs/datasets/field_test/20220128_indoor_test/bag/record_gg1.bag",
          "/home/ayumi/Documents/tohoku_uni/CLOVERs/datasets/field_test/20220128_indoor_test/bag/record_g2.bag", "robot1/")

change_frame_id("/home/ayumi/Documents/tohoku_uni/CLOVERs/datasets/field_test/20220128_indoor_test/bag/record_g3.bag",
                "/home/ayumi/Documents/tohoku_uni/CLOVERs/datasets/field_test/20220128_indoor_test/bag/record_gg1.bag", "robot1/")
two_round("/home/ayumi/Documents/tohoku_uni/CLOVERs/datasets/field_test/20220128_indoor_test/bag/record_gg1.bag",
          "/home/ayumi/Documents/tohoku_uni/CLOVERs/datasets/field_test/20220128_indoor_test/bag/record_g3.bag", "robot1/")

change_frame_id("/home/ayumi/Documents/tohoku_uni/CLOVERs/datasets/field_test/20220128_indoor_test/bag/record_p1.bag",
                "/home/ayumi/Documents/tohoku_uni/CLOVERs/datasets/field_test/20220128_indoor_test/bag/record_gg1.bag", "robot2/")
two_round("/home/ayumi/Documents/tohoku_uni/CLOVERs/datasets/field_test/20220128_indoor_test/bag/record_gg1.bag",
          "/home/ayumi/Documents/tohoku_uni/CLOVERs/datasets/field_test/20220128_indoor_test/bag/record_p1.bag", "robot2/")

change_frame_id("/home/ayumi/Documents/tohoku_uni/CLOVERs/datasets/field_test/20220128_indoor_test/bag/record_p2.bag",
                "/home/ayumi/Documents/tohoku_uni/CLOVERs/datasets/field_test/20220128_indoor_test/bag/record_gg1.bag", "robot2/")
two_round("/home/ayumi/Documents/tohoku_uni/CLOVERs/datasets/field_test/20220128_indoor_test/bag/record_gg1.bag",
          "/home/ayumi/Documents/tohoku_uni/CLOVERs/datasets/field_test/20220128_indoor_test/bag/record_p2.bag", "robot2/")

change_frame_id("/home/ayumi/Documents/tohoku_uni/CLOVERs/datasets/field_test/20220128_indoor_test/bag/record_p3.bag",
                "/home/ayumi/Documents/tohoku_uni/CLOVERs/datasets/field_test/20220128_indoor_test/bag/record_gg1.bag", "robot2/")
two_round("/home/ayumi/Documents/tohoku_uni/CLOVERs/datasets/field_test/20220128_indoor_test/bag/record_gg1.bag",
          "/home/ayumi/Documents/tohoku_uni/CLOVERs/datasets/field_test/20220128_indoor_test/bag/record_p3.bag", "robot2/")
