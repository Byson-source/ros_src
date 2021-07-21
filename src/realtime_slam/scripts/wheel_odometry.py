#!/usr/bin/env python
import roslib; roslib.load_manifest('wheel_odometry')
import rospy
import time
from std_msgs.msg import *
from nav_msgs.msg import Odometry
import numpy as np

#=========Initial Parameter=============
D_RIGHT=0.245462    #Pulse ratio of the right wheel encoder
D_LEFT=0.243768     #Pulse ratio of the left wheel encoder
GR=100.0            #Gear ratio
PR=32.0             #Pulse ratio of encoders
TREAD=1.002405      #Tread

rs=[0,0]; #pluse store
u=np.array([0,0]);
count=0
odocount=0

result=open('odo.txt','w')

def talker():
    rospy.init_node('infant_odometry', anonymous=True)
    rospy.Subscriber("/recover/odom", Odometry, odom_callback)
    pub = rospy.Publisher('/odom', Odometry)

    odo=Odometry()
    odo.header.frame_id='1'
    odo.child_frame_id='1'

    current_time=time.time()
    last_time=time.time()

    #======= Parameter Setting with Parameter Server=======
    #   * If the parameter server didn't work, the following sequence would be passed.
    if rospy.has_param('infant_learning_odometry/right_wheel_p'):
        global D_RIGHT
        D_RIGHT=rospy.get_param('infant_learning_odometry/right_wheel_p')
    if rospy.has_param('infant_learning_odometry/left_wheel_p'):
        global D_LEFT
        PRIGHT=rospy.get_param('infant_learning_odometry/left_wheel_p')
    if rospy.has_param('infant_learning_odometry/Tread'):
        global TREAD
        TREAD=rospy.get_param('infant_learning_odometry/Tread')

    #print PRIGHT
    #print PLEFT
    #print TREAD
    #===========================================================
    odo.pose.pose.position.x=0
    odo.pose.pose.orientation.w=0
    odo.twist.twist.linear.x=0
    odo.twist.twist.angular.z=0

    r = rospy.Rate(10)
    global dt
    global x
    global count
    x=np.array([0,0,toRadian(0.0)]);
    dt=0.1
    t=0;

    while not rospy.is_shutdown():
        count=count+1

        current_time=time.time()

        dt=current_time-last_time

        odo.header.seq=odocount
        odo.pose.pose.position.x=x[0]
        odo.pose.pose.position.y=x[1]
        odo.pose.pose.orientation.w=x[2]
        odo.twist.twist.linear.x=u[0]
        odo.twist.twist.angular.z=u[1]

        pub.publish(odo)

        if count%2000==0:
            print "%8.2f" %x[0],"%8.2f" %x[1],"%8.2f" %toDegree(x[2])
            #print (last_time-current_time)
            #print t

        result.write('%f,%f,%f,%f,%f\n' %(x[0],x[1],x[2],u[0],u[1]))

        last_time=time.time()
        t+=dt

        r.sleep()
    rospy.spin()

def calc_input(rs):
    u=array([0,0]);
    vr=rs[0]/(GR*PR)*pi*D_RIGHT
    vl=rs[1]/(GR*PR)*pi*D_LEFT
    v=(vr+vl)/2.0
    yawrate=(vr-vl)/(TREAD/2.0)
    u=array([v,yawrate])
    return u
def MotionModel(x,u,dt):
    F=eye(3)
    B=array([[dt*cos(x[2]),0],
            [dt*sin(x[2]),0],
            [0,dt]])

    x=dot(F,x)+dot(B,u)
    x[2]=PItoPI(x[2])
    return x

def PItoPI(angle):
    while angle>=pi:
        angle=angle-2*pi
    while angle<=-pi:
        angle=angle+2*pi
    return angle

def odom_callback(data):
    global u
    global x
    global dt
    global odocount
    rs=[data.pose.pose.position.x,data.pose.pose.position.y]
    odocount=data.header.seq

    u=calc_input(rs)

    x=MotionModel(x,u,dt)

def toDegree(angle):
    angle=angle*180.0/pi
    return angle

def toRadian(angle):
    angle=angle*pi/180.0
    return angle

if __name__ == '__main__':
    talker()