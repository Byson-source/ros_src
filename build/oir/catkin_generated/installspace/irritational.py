import rospy
import numpy as np
from sensor_msgs.msg import LaserScan
from geometry_msgs.msg import Twist
def callback(data):
    laser_data=np.array(data.ranges)
    laser_data=laser_data[~np.isinf(laser_data)]
    print(laser_data)
    print(max(laser_data))
    if max(laser_data)<0.6:
        take_turn()
        move_forward()
        laser_data=1
    move_forward()

def move_forward():
    twist=Twist()
    twist.linear.x=0.1
    pub.publish(twist)
    print('Moving forward...')

def take_turn():
    twist=Twist()
    twist.angular.z=1.57/2
    r=rospy.Rate(5.0)
    for i in range(1):
        pub.publish(twist)
        r.sleep()
    print('Turning...')

def main():
    rospy.Subscriber('/scan',LaserScan,callback)
    rospy.spin()

rospy.init_node('Avoider')
pub=rospy.Publisher('/cmd_vel',Twist,queue_size=1)
if __name__ == '__main__':
    main()