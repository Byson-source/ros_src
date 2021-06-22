import rospy
import math
from geometry_msgs.msg import Twist
from turtlesim.msg import Pose
import time


def move(velocity_publisher,speed,distance,is_forward):
    velocity_message=Twist()

    global x,y
    x0=x
    y0=y

    if is_forward:
        velocity_message.linear.x=abs(speed)
    else:
        velocity_message.linear.x = -abs(speed)


    loop_rate=rospy.Rate(10)

    while True:
        rospy.loginfo('Turtlesim is moving...')
        velocity_publisher.publish(velocity_message)
        loop_rate.sleep()
        moved_distance=abs(math.sqrt((x-x0)**2+(y-y0)**2))
        print(moved_distance)
        if not (moved_distance<distance):
            rospy.loginfo('reached')
            break

    velocity_message.linear.x=0
    velocity_publisher.publish(velocity_message)

def posecallback(pose_message):
    global x
    global y, yaw
    x=pose_message.x
    y=pose_message.y
    yaw=pose_message.theta

def rotate(velocity_publisher,angular_speed_degree,relative_angle_degree,clockwise=True):
    velocity_message=Twist()
    angular_speed=math.radians(abs(angular_speed_degree))

    if clockwise:
        velocity_message.angular.z=-abs(angular_speed)
    else:
        velocity_message.angular.z =abs(angular_speed)

    loop_rate=rospy.Rate(10)
    t0=rospy.Time.now().to_sec()

    while True:
        rospy.loginfo('Turtlesim rotating...')
        velocity_publisher.publish(velocity_message)

        t1=rospy.Time.now().to_sec()
        current_angle_degree=(t1-t0)*angular_speed_degree
        loop_rate.sleep()

        if current_angle_degree>relative_angle_degree:
            rospy.loginfo('reached')
            break

    velocity_message.angular.z=0
    velocity_publisher.publish(velocity_message)

def gotoGoal(velocity_publisher,x_goal,y_goal):
    global x
    global y,yaw

    velocity_message=Twist()
    while True:
        k_linear=0.5
        distance=abs(math.sqrt((x_goal-x)**2+(y_goal-y)**2))

        velocity_message.linear.x=k_linear*distance

        k_angular=4.0
        desired_angle_goal=math.atan2(y_goal-y,x_goal-x)
        angular_speed=(desired_angle_goal-yaw)*k_angular

        velocity_message.angular.z=angular_speed

        velocity_publisher.publish(velocity_message)

        if distance<0.01:
            break

    velocity_message.linear.x=0
    velocity_message.angular.z=0
    velocity_publisher.publish(velocity_message)


def setDesiredorientation(publisher,speed_in_degree,desired_angle_degree):
    global yaw
    relative_angle_radians=math.radians(desired_angle_degree)-yaw
    clockwise=True
    if relative_angle_radians<0:
        pass
    else:
        clockwise=False
    rotate(publisher,speed_in_degree,math.degrees(abs(relative_angle_radians)),clockwise)
    
def spiral(velocity_publisher,wk,rk):
    vel_msg=Twist()
    loop_rate=rospy.Rate(1)
    
    while(x<10.5 and y<10.5):
        # rk=rk+1
        vel_msg.linear.x=rk
        vel_msg.angular.z=wk
        
        velocity_publisher.publish(vel_msg)
        loop_rate.sleep()
    vel_msg.linear.x =0
    vel_msg.angular.z =0

def gridClean(publisher):
    desired_pose=Pose()
    desired_pose.x=1
    desired_pose.y=1
    desired_pose.theta=0

    gotoGoal(publisher,1,1)
    rotate(velocity_publisher, 30, 135, False)
    for i in range(5):
        move(publisher,2,1,True)
        rotate(publisher,20,90,False)
        move(publisher,2,9,True)
        rotate(publisher, 20, 90)
        move(publisher, 2, 1, True)
        rotate(publisher, 20, 90)
        move(publisher, 2, 9, True)
        rotate(publisher, 20, 90,False)

if __name__ == '__main__':
    try:
        """一つのノードにPublisherとSubscriberふた役！！！"""
        rospy.init_node('turtlesim_motion_pose',anonymous=True)
        velocity_topic='turtle1/cmd_vel'
        """turtlesimノードに司令を送る"""
        velocity_publisher=rospy.Publisher(velocity_topic,Twist,queue_size=10)
        """ここでturtlesimノードから位置や場所などの基本情報をもらう"""
        position_topic='turtle1/pose'
        """ここでxとyとyawを求める！！！"""
        pose_subscriber=rospy.Subscriber(position_topic,Pose,posecallback)
        time.sleep(2)
        # move(velocity_publisher,1.0,100,True)

        # setDesiredorientation(velocity_publisher,30,90)
        # gotoGoal(velocity_publisher,10,10)
        # spiral(velocity_publisher,10,10)
        gridClean(velocity_publisher) 
    except rospy.ROSInterruptException:
        rospy.loginfo('node terminated')