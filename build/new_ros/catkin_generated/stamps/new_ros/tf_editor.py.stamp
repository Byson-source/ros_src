#!/usr/bin/env python3

import rospy
import tf
from tf2_msgs.msg import TFMessage

def callback(data):



if __name__ == '__main__':
    info=TFMessage()
    rospy.init_node('editor')
    rate=rospy.Rate(1)
    rospy.Subscriber('/tf',TFMessage,callback)
    transformer=tf.TransformBroadcaster()
    transform_listener=tf.TransformListener()
    transform_listener.waitForTransform('/frame_a', '/frame_b', rospy.Time(), rospy.Duration(4))
    while not rospy.is_shutdown():
        try:
            now=rospy.Time()
            (translation,rotation)=transform_listener.lookupTransform('/frame_a','/frame_b',rospy.Time())
        except (tf.LookupException,tf.ConnectivityException,tf.ExtrapolationException):
            continue
        translation[0]+=2


        rate.sleep()


    # while not rospy.is_shutdown():
    #
    #     quaternion=tf.transformations.quaternion_from_euler(0.1,0.2,0.3)
    #     translation=(0.5,0.2,1)
    #
    #     now=rospy.Time.now()
    #
    #     transform.sendTransform(translation,quaternion,now,'frame_b','frame_a')
    #
    #     time.sleep(1)
    # rospy.spin()


