import math
import rospy
import tf
from gazebo_msgs.msg import ModelStates
import turtlesim.srv

if __name__ == '__main__':

    rospy.init_node('bug_listener')
    rate=rospy.Rate(10)
    command=ModelStates()
    transform_listener = tf.TransformListener()

    cmd=rospy.Publisher('gazebo/model_states',ModelStates,queue_size=1)

    while not rospy.is_shutdown():
        try:
            (translation,rotation)=transform_listener.lookupTransform('waffle_frame','goal_frame',rospy.Time(0))
        except (tf.LookupException,tf.ConnectivityException,tf.ExtrapolationException):
            continue


        print(command.twist)
        # cmd_x=translation[0]
        # cmd_y=translation[1]
        #
        # angular=4*math.atan2(cmd_y,cmd_x)
        # linear = 0.5 * math.sqrt(cmd_x ** 2 + cmd_y ** 2)
        #
        # command.twist[4].linear.x=linear
        # command.twist[4].angular.z=angular
        #
        # cmd.publish(command)
        #
        # rate.sleep()


