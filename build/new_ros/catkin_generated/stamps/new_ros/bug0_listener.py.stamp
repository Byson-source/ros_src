import math
import rospy
import tf
from nav_msgs.msg import Odometry
import turtlesim.srv

if __name__ == '__main__':

    rospy.init_node('bug_listener')
    rate=rospy.Rate(10)
    command=Odometry()
    transform_listener = tf.TransformListener()
    transform_listener.waitForTransform('/frame_a','/frame_b',rospy.Time(),rospy.Duration())

    cmd=rospy.Publisher('odom',Odometry,queue_size=1)

    while not rospy.is_shutdown():
        try:
            (translation,rotation)=transform_listener.lookupTransform('world','odom',rospy.Time(0))
        except (tf.LookupException,tf.ConnectivityException,tf.ExtrapolationException):
            continue


        # print(command.pose.pose)
        cmd_x=translation[0]
        cmd_y=translation[1]

        angular=4*math.atan2(cmd_y,cmd_x)
        linear = 0.5 * math.sqrt(cmd_x ** 2 + cmd_y ** 2)

        command.twist[4].linear.x=linear
        command.twist[4].angular.z=angular

        cmd.publish(command)

        rate.sleep()


