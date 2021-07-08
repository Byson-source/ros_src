"""
TO DO LIST
1:Change the algorithm for object avoidance
2:Change the algorithm to "turn"
"""

import rospy
import math
import tf
import time
from nav_msgs.msg import Odometry
from geometry_msgs.msg import Twist
from sensor_msgs.msg import LaserScan

"""angular.z>0の場合、反時計回り！"""

distance,present_x,present_y,present_yaw,goal_yaw=0,0,0,0,0
distance=2

def perceive(data):
    global present_x,present_y,present_yaw,goal_yaw

    present_x = data.pose.pose.position.x
    present_y = data.pose.pose.position.y

    r = data.pose.pose.orientation

    present_yaw = tf.transformations.euler_from_quaternion((r.x, r.y, r.z, r.w))[2]
    goal_yaw = math.atan2(goal_y - present_y, goal_x - present_x)



def scan_result(data):
    global distance

    scan_score=[i for i in data.ranges if not math.isnan(i)]

    Scan_score=scan_score[:45]+scan_score[315:]

    distance=min(Scan_score)


def straight_to_goal():
    global present_yaw,goal_yaw,present_x,present_y,command,status

    if math.sqrt((goal_x-present_x)**2+(goal_y-present_y)**2)>=0.5:
        cmd.linear.x =0.2
    else:
        cmd.linear.x = 0
        status=4

    command.publish(cmd)
    rate.sleep()

def turn():
    global present_yaw, goal_yaw, present_x, present_y, command
    cmd.angular.z = goal_yaw - present_yaw
    if math.sqrt((goal_x - present_x) ** 2 + (goal_y - present_y) ** 2) <= 0.5:
        cmd.angular.z=0
    command.publish(cmd)
    rate.sleep()

def go_straight():
    cmd.linear.x=0.2
    command.publish(cmd)
    rate.sleep()

def change_direction():
    """反時計回り"""
    cmd.angular.z=0.2
    command.publish(cmd)
    rate.sleep()

def stop():
    cmd.angular.z=0
    cmd.linear.x=0
    command.publish(cmd)
    rate.sleep()

if __name__ == '__main__':
    rospy.init_node('test')

    goal_x = int(input('What is the x of target?>>>>'))
    goal_y = int(input('What is the y of target?>>>>'))

    rate=rospy.Rate(100)
    cmd=Twist()
    scan=LaserScan()
    scan.range_max=10

    rospy.Subscriber('/odom',Odometry,perceive)
    rospy.Subscriber('/scan',LaserScan,scan_result)
    command=rospy.Publisher('cmd_vel',Twist,queue_size=1)
    status=0
    phase1_iteration=0
    while not rospy.is_shutdown():
        print("status:%s"%(status))
#障害物がないときに目標地点にまっすぐ移動する。
        if status==0:
            if distance<=1:
                stop()
                status+=1
                target_where_is_wall=where_is_wall
                continue
            straight_to_goal()
            turn()


#壁に面したときの方向転換
        elif status==1:
            phase1_iteration+=1
            if phase1_iteration==1:
                if 0<=target_where_is_wall<=90:
                    ideal_direction=present_yaw+math.radians(90+target_where_is_wall)
                else:
                    ideal_direction=present_yaw+math.radians(target_where_is_wall-270)

                if ideal_direction > math.pi:
                    ideal_direction=-2*math.pi+ideal_direction
                elif ideal_direction<-math.pi:
                    ideal_direction=2*math.pi+ideal_direction


            else:
                print('ideal_direction:%f,present_yaw:%f'%(ideal_direction,present_yaw))
                change_direction()
                if 0<(present_yaw-ideal_direction)<=1/360*math.pi:
                    print('!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!')
                    stop()
                    phase1_iteration=0
                    status+=1
        elif status==2:
            go_straight()
            if right_distance>1:
                stop()
                status=0
            elif distance<=0.7:
                stop()
                status=1
        else:
            break

    rospy.spin()



