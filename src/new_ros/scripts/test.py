import rospy
import math
import tf
import time
from nav_msgs.msg import Odometry
from geometry_msgs.msg import Twist
from sensor_msgs.msg import LaserScan

def turn(data):
    r = data.pose.pose.orientation
    present_yaw = tf.transformations.euler_from_quaternion((r.x, r.y, r.z, r.w))[2]
    print(present_yaw)
    rate.sleep()

def scan_result(data):

    scan_score=[i for i in data.ranges if not math.isnan(i)]

    Scan_score=scan_score[:5]+scan_score[355:]

    distance=min(Scan_score)
    min_index = Scan_score.index(distance)
    print('min_index:%s'%min_index)
    print(distance)

if __name__ == '__main__':
    rospy.init_node('test')
    rate = rospy.Rate(10)
    cmd = Twist()
    scan = LaserScan()

    rospy.Subscriber('odom',Odometry,turn)
    rospy.Subscriber('/scan', LaserScan, scan_result)
    command = rospy.Publisher('cmd_vel', Twist, queue_size=1)

    rospy.spin()