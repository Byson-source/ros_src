import roslib
import math
import rospy
import tf_broadcaster
import tf
from geometry_msgs.msg import Twist
import turtlesim.srv




if __name__ == '__main__':
    rospy.init_node('turtle_tf_listener')
    rate = rospy.Rate(10)
    command = Twist()
    transform_listener=tf.TransformListener()

    rospy.wait_for_service('spawn')
    spawner=rospy.ServiceProxy('spawn',turtlesim.srv.Spawn)
    spawner(4,2,0,'turtle2')


    cmd=rospy.Publisher('turtle2/cmd_vel', Twist, queue_size=1)
    while not rospy.is_shutdown():
        try:
            (translation,rotation)=transform_listener.lookupTransform('/turtle2_frame','turtle1_frame',rospy.Time(0))
        except (tf.LookupException,tf.ConnectivityException,tf.ExtrapolationException):
            continue

        x_follower=translation[0]
        y_follower=translation[1]

        angular=4*math.atan2(y_follower,x_follower)
        linear=0.5*math.sqrt(x_follower**2+y_follower**2)

        command.linear.x=linear
        command.angular.z=angular

        cmd.publish(command)

        rate.sleep()





