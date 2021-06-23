import roslib
import rospy
import math
import tf
import geometry_msgs.msg
import tutlesim.srv

if __name__ == '__main__':
    rospy.init_node('turtle_tf_listener')

    transform_listener=tf.TransformListener()

    rospy.wait_for_service('spawn')
    spawner=rospy.ServiceProxy('spawn',turtlesim.srv.Spawn)
    spawner(4,2,0,'turtle2')
    follower_cmd=rospy.Publisher('turtle2/cmd_vel',geometry_msgs.msg.Twist,queue_size=1)

    rate=rospy.Rate(10)

    while not rospy.is_shutdown():
        rate.sleep()
        try:
            translation,rotation=transform_listener.lookupTransform('/turtle2_frame','/turtle1_frame',rospy.Time(0))
        except(tf.LookupException,tf.ConnectivityExceptiontf.ExtrapolationException):
            continue

        follower_to_turtle1_x=translation[0]
        follower_to_turtle1_y=translation[1]

        angular=4*math.atan2(follower_to_turtle1_y,follower_to_turtle1_x)
        linear=0.5*math.sqrt(follower_to_turtle1_x**2+follower_to_turtle1_y**2)

        cmd=geometry_msgs.msg.Twist()
        cmd.linear.x=linear
        cmd.angular.z=angular
        follower_cmd.publish(cmd)


