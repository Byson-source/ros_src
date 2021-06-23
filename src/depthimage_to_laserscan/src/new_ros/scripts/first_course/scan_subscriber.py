import rospy
from sensor_msgs.msg import LaserScan
import math

def scan_callback(data):
    ranges=[i for i in data.ranges if i!=math.isnan()]




if __name__ == '__main__':
    rospy.init_node('test')
    rospy.Publisher