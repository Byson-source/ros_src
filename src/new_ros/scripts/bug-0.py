import rospy
import math
import tf
import time
from nav_msgs.msg import Odometry
from geometry_msgs.msg import Twist
from sensor_msgs.msg import LaserScan

"""angular.z>0の場合、反時計回り！"""

right_distance,left_distance,distance,min_index,distance,present_x,present_y,present_yaw,goal_yaw=0,0,0,0,0,0,0,0,0
right_min_index=0

def perceive(data):
    global present_x,present_y,present_yaw,goal_yaw

    present_x = data.pose.pose.position.x
    present_y = data.pose.pose.position.y

    r = data.pose.pose.orientation

    present_yaw = tf.transformations.euler_from_quaternion((r.x, r.y, r.z, r.w))[2]
    goal_yaw = math.atan2(goal_y - present_y, goal_x - present_x)



def scan_result(data):
    global distance,min_index,right_distance,right_min_index

    scan_score=[i for i in data.ranges if not math.isnan(i)]


    right_side = scan_score[265:276]
    Scan_score=scan_score[:5]+scan_score[355:]

    distance=min(Scan_score)
    min_index = Scan_score.index(distance)

    right_distance = min(right_side)#右側のセンシング
    right_min_index=right_side.index(min(right_side))

def straight_to_goal():
    global present_yaw,goal_yaw,present_x,present_y,command,status

    if math.sqrt((goal_x-present_x)**2+(goal_y-present_y)**2)>=0.5:
        cmd.linear.x =0.5
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
    cmd.linear.x=0.5
    command.publish(cmd)
    rate.sleep()

def change_direction():
    """反時計回り"""
    cmd.angular.z=0.5
    command.publish(cmd)
    rate.sleep()

def stop():
    cmd.angular.z=0
    cmd.linear.x=0
    command.publish(cmd)
    rate.sleep()

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
    phase1_iteration=0
    while not rospy.is_shutdown():
        print(status)
#障害物がないときに目標地点にまっすぐ移動する。
        if status==0:
            straight_to_goal()
            turn()

            if distance<=0.7:
                stop()
                status+=1
#壁に面したときの方向転換
        elif status==1:
            phase1_iteration+=1
            if phase1_iteration==1:
                if 0<=min_index<5:
                    ideal_direction=present_yaw+90+min_index
                else:
                    ideal_direction=present_yaw+95-min_index

                if ideal_direction > 360:
                    ideal_direction -= 360


                ideal_direction=math.radians(ideal_direction)

            else:
                print('ideal_direction:%f,present_yaw:%f'%(ideal_direction,present_yaw))
                print()
                change_direction()
                if abs(present_yaw-ideal_direction)<=1/180*math.pi:
                    print('!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!')
                    stop()
                    phase1_iteration=0
                    status+=1
        elif status==2:
            go_straight()
            if right_distance>1:
                stop()
                status=0
        else:
            break

    rospy.spin()

