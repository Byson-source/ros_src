import rospy
import math
import tf
import time
from nav_msgs.msg import Odometry
from geometry_msgs.msg import Twist
from sensor_msgs.msg import LaserScan

def turn():
    cmd.linear.x = 0.5
    command.publish(cmd)
    rate.sleep()

if __name__ == '__main__':
    rospy.init_node('test')

    rate = rospy.Rate(10)
    cmd = Twist()
    scan = LaserScan()
    command = rospy.Publisher('cmd_vel', Twist, queue_size=1)
    while True:
        turn()

    rospy.spin()