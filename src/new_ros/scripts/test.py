import rospy
import math
import tf
import time
from geometry_msgs.msg import Twist
from sensor_msgs.msg import LaserScan


distance=10
def stop(data):
    r = data.pose.pose.orientation
    present_yaw = tf.transformations.euler_from_quaternion((r.x, r.y, r.z, r.w))[2]
    print(present_yaw)
    rate.sleep()

def go_straight():
    cmd.linear.x=0.5
    command.publish(cmd)
    rate.sleep()

def stop():
    cmd.linear.x=0
    command.publish(cmd)
    rate.sleep()

def scan_result(data):
    global distance
    scan_score=[i for i in data.ranges if not math.isnan(i)]

    distance=min(scan_score)
    min_index = scan_score.index(distance)
    print('min_index:%s'%min_index)
    print('distance:%s'%distance)

if __name__ == '__main__':
    rospy.init_node('test')
    rate = rospy.Rate(10)
    cmd = Twist()
    scan = LaserScan()

    rospy.Subscriber('/scan', LaserScan, scan_result)
    command = rospy.Publisher('cmd_vel', Twist, queue_size=1)
    while True:
        if distance<0.7:
            stop()
        else:
            go_straight()


    rospy.spin()