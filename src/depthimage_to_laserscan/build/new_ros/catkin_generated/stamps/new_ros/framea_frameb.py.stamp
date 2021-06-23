import rospy
import tf

while rospy.is_shutdown():
    rospy.init_node('convert')
    transform=tf.TransformBroadcaster()
    quaternion=tf.transformations.quaternion_from_euler(0.1,0.2,0.3)
    translation=(0.5,0.2,1)

    now=rospy.Time.now()

    transform.sendTransform(translation,quaternion,now,'frame_b','frame_a')

