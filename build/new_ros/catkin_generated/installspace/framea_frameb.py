import rospy
import tf
import time

if __name__ == '__main__':
    rospy.init_node('convert')
    time.sleep(2)
    transform = tf.TransformBroadcaster()
    while not rospy.is_shutdown():

        quaternion=tf.transformations.quaternion_from_euler(0.1,0.2,0.3)
        translation=(0.5,0.2,1)

        now=rospy.Time.now()

        transform.sendTransform(translation,quaternion,now,'frame_b','frame_a')

        time.sleep(1)
    rospy.spin()


