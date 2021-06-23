import rospy
import math
import frameA_to_frameB
import geometry_msgs.msg
import tf
import turtlesim.srv

if __name__ == '__main__':
    rospy.init_node('frame_a_frame_b_listener_node')
    listener=tf.TransformListener()
    rate=rospy.Rate(1.0)

    listener.waitForTransform('/frame_a','/frame_b',rospy.Time(),rospy.Duration())

    while not rospy.is_shutdown():
        try:
            (trans,rot)=listener.lookupTransform('/frame_a','/frame_b',rospy.Duration())
        except (tf.LookupException,tf.ConnectivityException,tf.ExtrapolationException):
            continue

    quaternion=rot
    rpy=tf.transformations.euler_from_quaternion(quaternion)
    print('transformation between frame_a and frame_b is detected!')
    print('translation vector:%.1f,%.1f,%.1f'%(trans[0],trans[1],trans[2]))
    print('rotation angles:roll=%.2f,pitch=%.2f,yaw=%.2f'%(rpy[0],rpy[1],rpy[2]))

    rate.sleep()
