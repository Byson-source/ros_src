import numpy as np
import python_module.feature_header as feature_header
import rospy
import cv2
from cpp.msg import FeatureArray
import pyrealsense2 as rs2
import sys

file1 = sys.argv[0]
file1 = sys.argv[1]
file1 = sys.argv[2]
file1 = sys.argv[3]
if __name__ == "__main__":
    rospy.init_node("g2o_test")
    feature_pub = rospy.Publisher("features", FeatureArray, queue_size=10)
