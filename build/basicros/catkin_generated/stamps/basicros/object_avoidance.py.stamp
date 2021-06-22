import rospy
import sys
from sensor_msgs.msg import LaserScan
from geometry_msgs.msg import Twist
import time
import math


laser=LaserScan()

def callback(data):
    global distance
    data.ranges=data.ranges[:15]+data.ranges[345:360]
    true_ranges=[i for i in data.ranges if i !=math.isnan(i)]
    distance=min(true_ranges)

def rotate(a):
    global order

    order.linear.x=0
    order.angular.z=a

    pub.publish(order)



def straight(a):
    global order

    order.linear.x = a
    order.angular.z = 0

    pub.publish(order)

def main(args):
    t1=time.time()

    global order
    order = Twist()
    rate=rospy.Rate(10)

    rotate_status=0

    while True:
        t2=time.time()
        rate.sleep()
        print(distance)

        if t2-t1>120:
            break

        if rotate_status==0:
            if distance<0.8:
                rotate_status+=1
            else:
                straight(0.3)
        else:
            if distance<2:
                rotate(0.3)
            else:
                rotate_status-=1

    # print('STOP!!')
    order.linear.x = 0
    order.angular.z=0
    pub.publish(order)

if __name__ == '__main__':
    rospy.init_node('Brain', anonymous=True)
    order_topic = '/cmd_vel'
    info_topic = '/scan'
    pub = rospy.Publisher(order_topic, Twist, queue_size=10)
    rospy.Subscriber(info_topic, LaserScan, callback)
    time.sleep(2)
    main(sys.argv)