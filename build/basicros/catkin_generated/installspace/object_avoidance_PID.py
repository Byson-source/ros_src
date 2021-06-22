import rospy
import sys
from sensor_msgs.msg import LaserScan
from geometry_msgs.msg import Twist
import time
import math


laser=LaserScan()

def callback(data):
    global distance,least_angle
    data.ranges=data.ranges[:15]+data.ranges[345:360]
    true_ranges=[i for i in data.ranges if i !=math.isnan(i)]
    distance=min(true_ranges)
    least_angle=true_ranges.index(distance)
    if least_angle>=len(true_ranges)//2:
        #345<least_angle<360
        least_angle=least_angle-345
    else:
        least_angle=-(15-least_angle)


def main(args):
    t1=time.time()

    global order
    order = Twist()
    rate=rospy.Rate(10)

    rotate_status=0

    while True:
        t2=time.time()
        if t2-t1>30:
            break
        rate.sleep()

        k_linear=0.3
        order.linear.x=k_linear*distance
        if order.linear.x>=0.3:
            order.linear.x=0.3

        k_angular=0.05
        order.angular.z=1/distance*least_angle*k_angular
        if abs(order.angular.z)>0.5:
            if order.angular.z<0:
                order.angular.z = -0.7
            else:
                order.angular.z = 0.7
        print(order.angular.z)
        pub.publish(order)

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
#
# import rospy
# import math
# from sensor_msgs.msg import LaserScan
# from geometry_msgs.msg import Twist
# import time
#
# plusminus5 = []
# plusminusright = []
# plusminusleft = []
#
#
# def perceive(scan_val):
#     global min_ind, max_ind
#     global scan_ranges
#     global plusminus5
#     global plusminusleft
#     global plusminusright
#
#     scan_ranges = scan_val.ranges
#     lt = len(scan_ranges)
#
#     # For -5 to +5 range
#     if scan_val.angle_min == 0:
#         min_index = math.floor(-5 // (180 * (scan_val.angle_increment / math.pi)))
#         max_index = math.ceil(5 / (180 * (scan_val.angle_increment / math.pi)))
#
#         # print(min_index, max_index) -----> For testing
#
#         plusminus5 = scan_ranges[0:max_index] + scan_ranges[min_index:]
#         plusminusleft = scan_ranges[:lt // 2]
#         plusminusright = scan_ranges[lt // 2:]
#
#         # print([round(elem, 2) for elem in list(plusminus5)]) #-----> For testing
#
#
# # Function for straight ahead motion
# def move_strt(vel_message, vel_pub):
#     # Accesing global variable containing ranges in +5 to -5 degrees
#     global plusminus5
#
#     # Linear Proportional Controller Constant
#     Kl = 0.5
#
#     # Loop and publish linear speed as long as the min distance is above 0.6m
#     while not min(plusminus5) < 0.6:
#
#         if min(plusminus5) > 3.0:
#             vel_message.linear.x = 0.25
#             vel_pub.publish(vel_message)
#         else:
#             vel_message.linear.x = (min(plusminus5) - 0.6) * Kl
#             vel_pub.publish(vel_message)
#
#     vel_message.linear.x = 0
#     vel_pub.publish(vel_message)
#     return True
#
#
# def rotate(vel_message, vel_pub):
#     # Accesing global variable containing ranges in +5 to -5 degrees
#     global plusminus5, plusminusright, plusminusleft
#
#     Ka = 0.2
#
#     if min(plusminusright) >= min(plusminusleft):
#         Ka = -1 * Ka
#
#     # Loop and publish angular speed as long as the min distance is below 3.0m
#     while not min(plusminus5) > 3.0:
#         vel_message.angular.z = (3.0 - min(plusminus5)) * Ka
#         vel_pub.publish(vel_message)
#
#     vel_message.angular.z = 0
#     vel_pub.publish(vel_message)
#
#
# def avoid_obstacles(vel_pub):
#     # Creating velocity message to be published
#     vel_message = Twist()
#     rate = rospy.Rate(100)
#
#     # Loop to run the obstacle avoidance
#     try:
#         time.sleep(1)
#         while True:
#
#             if move_strt(vel_message, vel_pub):
#                 rotate(vel_message, vel_pub)
#
#             # rate.sleep()
#
#     except KeyboardInterrupt:
#         print("Exiting Program")
#
#
# def main():
#     rospy.init_node('Rudimentary_Obstacle_Avoidance', anonymous=True)
#
#     lscanner_sub = rospy.Subscriber('scan', LaserScan, perceive)
#     vel_pub = rospy.Publisher('/cmd_vel', Twist, queue_size=10)
#
#     avoid_obstacles(vel_pub)
#
#     rospy.spin()
#
#
# if __name__ == '__main__':
#     main()