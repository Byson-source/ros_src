#!/usr/bin/env python3
import rospy
from std_msgs.msg import Float32
import random
import time

def gen_number():
    now1=time.time()
    pub = rospy.Publisher('random_number', Float32,queue_size=10)
    switch=rospy.Publisher("switch_number",Float32,queue_size=10)
    rospy.init_node('random_number_generator', log_level=rospy.INFO)
    rospy.loginfo("Generating random numbers")
    i=1
    while not rospy.is_shutdown():
        now2=time.time()
        pub.publish(Float32(random.normalvariate(5, 1)))
        if(now2-now1>15*i):
            switch.publish(1.0)
            rospy.loginfo("Changed!!")
            i+=1
        else:
            switch.publish(0.0)

        rospy.sleep(0.05)


if __name__ == '__main__':
    gen_number()
