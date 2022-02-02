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


def stamp_convert_any_topic(referred_bag, inbag, outbag):
    print("timestamp is converted...")
    outbag = rosbag.Bag(outbag, 'w')
    for topic, msg, t in rosbag.Bag(referred_bag, 'r').read_messages():
        if ((topic_list[0] in topic) or (topic_list[1] in topic) or (topic_list[2] in topic) or (topic_list[3] in topic)):
            secs_stamp[topic].append(msg.header.stamp.secs)
            nsecs_stamp[topic].append(msg.header.stamp.nsecs)
            # msg.header.frame_id = header+topic_frame
            # print(msg.header.frame_id)
        elif "tf" in topic:
            for val in msg.transforms:
                if not val.header.frame_id[7:] == "camera_link":
                    secs_tf_dict[val.header.frame_id[7:]].append(
                        val.header.stamp.secs)
                    nsecs_tf_dict[val.header.frame_id[7:]].append(
                        val.header.stamp.nsecs)
                else:
                    secs_tf_dict["camera_link"][val.child_frame_id[7:]].append(
                        val.header.stamp.secs)
                    nsecs_tf_dict["camera_link"][val.child_frame_id[7:]].append(
                        val.header.stamp.nsecs)

            # for val in msg.transforms:
            #     val.header.frame_id = header+val.header.frame_id
            #  child_frame_id = headchild_frame_id

    for topic, msg, t in rosbag.Bag(inbag, 'r').read_messages():
        if ((topic_list[0] in topic) or (topic_list[1] in topic) or (topic_list[2] in topic) or (topic_list[3] in topic)):

            msg.header.stamp.secs = secs_stamp[topic][0]
            msg.header.stamp.nsecs = nsecs_stamp[topic][0]
            secs_stamp[topic].pop(0)
            nsecs_stamp[topic].pop(0)

        elif "tf" in topic:
            for val in msg.transforms:
                if not val.header.frame_id[7:] == "camera_link":
                    val.header.stamp.secs = secs_tf_dict[val.header.frame_id[7:]][0]
                    secs_tf_dict[val.header.frame_id[7:]].pop(0)
                    val.header.stamp.nsecs = nsecs_tf_dict[val.header.frame_id[7:]][0]
                    nsecs_tf_dict[val.header.frame_id[7:]].pop(0)
                else:
                    val.header.stamp.secs = secs_tf_dict[
                        "camera_link"][val.child_frame_id[7:]][0]
                    val.header.stamp.nsecs = nsecs_tf_dict[
                        "camera_link"][val.child_frame_id[7:]][0]
                    secs_tf_dict["camera_link"][val.child_frame_id[7:]].pop(
                        0)
                    nsecs_tf_dict["camera_link"][val.child_frame_id[7:]].pop(
                        0)

        outbag.write(topic, msg, t)
    outbag.close()


def convert(path, header):
    path_1 = "/home/ayumi/Documents/tohoku_uni/CLOVERs/datasets/field_test/20220201_indoor_test/a.bag"
    change_frame_id(path, path_1, header)
    two_round(path_1, path, header)


home_path = "/home/ayumi/Documents/tohoku_uni/CLOVERs/datasets/field_test/20220201_indoor_test/"
stamp_convert_any_topic(home_path+"record_b4.bag",
                        home_path+"record_r3.bag", home_path+"record_r3_.bag")
