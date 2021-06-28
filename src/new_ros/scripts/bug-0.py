import rospy
import math
import tf
import time
from nav_msgs.msg import Odometry
from geometry_msgs.msg import Twist
from sensor_msgs.msg import LaserScan

right_distance,left_distance,distance,min_index,distance,present_x,present_y,present_yaw,goal_yaw=0,0,0,0,0,0,0,0,0

def perceive(data):
    global present_x,present_y,present_yaw,goal_yaw

    present_x = data.pose.pose.position.x
    present_y = data.pose.pose.position.y

    r = data.pose.pose.orientation

    present_yaw = tf.transformations.euler_from_quaternion((r.x, r.y, r.z, r.w))[2]
    goal_yaw = math.atan2(goal_y - present_y, goal_x - present_x)



def scan_result(data):
    global distance,min_index
    scan_score=[i for i in scan.ranges if not math.isnan(i)]
    right_side = scan_score[85:96]
    left_side = scan_score[265:275]

    Scan_score=scan_score[:5]+scan_score[355:]


    distance=min(Scan_score)
    right_distance = min(right_side)
    left_distance=min(left_side)

    min_index=Scan_score.index(distance)





def straight_to_goal():
    global present_yaw,goal_yaw,present_x,present_y,command
    global present_x

    if math.sqrt((goal_x-present_x)**2+(goal_y-present_y)**2)>=0.5:
        cmd.linear.x =0.5
    else:
        cmd.linear.x = math.sqrt((goal_x - present_x) ** 2 + (goal_y - present_y) ** 2)

    command.publish(cmd)
    rate.sleep()


def turn():
    global present_yaw, goal_yaw, present_x, present_y, command
    cmd.angular.z = goal_yaw - present_yaw

    command.publish(cmd)
    rate.sleep()

def change_direction():
    print('a')
    # global min_index
    # if min_index<=5:
    #     # 90+min_index
    # else:
    #     # 95-min_index

if __name__ == '__main__':
    rospy.init_node('test')

    goal_x=int(input('What is the x of target?>>>>'))
    goal_y = int(input('What is the y of target?>>>>'))

    rate=rospy.Rate(10)
    cmd=Twist()
    scan=LaserScan()

    rospy.Subscriber('/odom',Odometry,perceive)
    rospy.Subscriber('/scan',LaserScan,scan_result)
    command=rospy.Publisher('cmd_vel',Twist,queue_size=1)
    status=0
    while True:
        if status==0 and distance>3:
            straight_to_goal()
            turn()
        elif distance<=3:
            status+=1
    rospy.spin()

