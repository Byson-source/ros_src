#!/usr/bin/env python3
import rospy
from sensor_msgs.msg import Joy
from geometry_msgs.msg import Twist
axes,buttons=[0,0],[0,0]
def perceive(data):
    global axes,buttons
    axes=data.axes
    buttons=data.buttons
    # print(buttons)

def transmit():
    global axes,buttons
    if buttons[0]==1:
        if axes[-1]==1.0:
            cmd.linear.x=0.5
        elif axes[-1]==-1.0:
            cmd.linear.x = -0.5
        elif axes[-2]==1.0:
            cmd.angular.z=0.4
        elif axes[-2]==-1.0:
            cmd.angular.z = -0.4
        else:
            cmd.linear.x = 0
            cmd.angular.z = 0
    elif buttons[1]==1:
        if axes[-1]==1.0:
            cmd.linear.x=1.5
        elif axes[-1]==-1.0:
            cmd.linear.x = -1.5
        elif axes[-2]==1.0:
            cmd.angular.z=0.4
        elif axes[-2]==-1.0:
            cmd.angular.z = -0.4
        else:
            cmd.linear.x = 0
            cmd.angular.z = 0
    else:
        cmd.linear.x=0
        cmd.angular.z=0

    command.publish(cmd)
    rate.sleep()


if __name__ == '__main__':
    rospy.init_node('Operation_com')
    rate=rospy.Rate(10)
    cmd=Twist()

    rospy.Subscriber('clover2/joy',Joy,perceive)
    command=rospy.Publisher('clover2/cmd_vel',Twist,queue_size=1)

    while True:
        transmit()

    rospy.spin()