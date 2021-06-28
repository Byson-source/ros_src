import rospy
import math
import tf
import time
from nav_msgs.msg import Odometry
from geometry_msgs.msg import Twist

from sensor_msgs.msg import LaserScan

# def pose_callback(data):
#     transform_broadcaster = tf2_ros.TransformBroadcaster()
#     q=data.pose.pose.orientation
#     pos=data.pose.pose.position
#     t=TransformStamped()
#
#     (t.transform.translation.x,t.transform.translation.y,t.transform.translation.z)=(pos.x,pos.y,0)
#     (t.transform.rotation.x,t.transform.rotation.y,t.transform.rotation.z,t.transform.rotation.w)=(q.x,q.y,q.z,q.w)
#     t.header.stamp=rospy.Time.now()
#     t.header.frame_id='world'
#     t.child_frame_id='A'
    # rotation_quaternion=(q.x,q.y,q.z,q.w)
    # translation_vector=(pos.x,pos.y,0)
    # current_time=rospy.Time.now()


    # transform_broadcaster.sendTransform(translation_vector, rotation_quaternion,"odom", "world")
    # transform_broadcaster.sendTransform(t)

def straight_to_goal(data):
    cmd=Twist()
    present_x=data.pose.pose.position.x
    present_y=data.pose.pose.position.y

    r=data.pose.pose.orientation

    present_yaw=tf.transformations.euler_from_quaternion((r.x,r.y,r.z,r.w))[2]
    goal_yaw=math.atan2(goal_y-present_y,goal_x-present_x)

    if math.sqrt((goal_x-present_x)**2+(goal_y-present_y)**2)>=0.5:
        cmd.linear.x =0.5
        cmd.angular.z = (goal_yaw - present_yaw) * 0.4
    else:
        cmd.linear.x = math.sqrt((goal_x - present_x) ** 2 + (goal_y - present_y) ** 2)



    command.publish(cmd)
    rate.sleep()

def turn_right(data):
    print(data)


if __name__ == '__main__':
    rospy.init_node('test')

    goal_x=int(input('What is the x of target?>>>>'))
    goal_y = int(input('What is the y of target?>>>>'))

    rate=rospy.Rate(10)


    rospy.Subscriber('/odom',Odometry,straight_to_goal)
    command=rospy.Publisher('cmd_vel',Twist,queue_size=1)
    rospy.spin()