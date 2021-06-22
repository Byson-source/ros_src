import roslib
import rospy
import tf
import time
import math

if __name__ == '__main__':
    rospy.init_node('Convert_node')
    time.sleep(1)
    transform_broadcaster=tf.TransformBroadcaster()
    while not rospy.is_shutdown():
        rotation_quaternion=tf.transformations.quaternion_from_euler(0.2,0.3,0.4)

        translation_vector=(1.0,2.0,3.0)

        current_time=rospy.Time.now()

        transform_broadcaster.sendTransform(
            translation_vector,
            rotation_quaternion,
            current_time,
            'frame_b','frame_a') #childFrame,Parent_Frame
        time.sleep(0.3)
    rospy.spin()