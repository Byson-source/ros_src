import roslib
import rospy
import tf
import turtlesim.msg
from turtlesim.msg import Pose

def pose_callback(data,turtlename):
    transform_broadcaster=tf.TransformBroadcaster()
    rotation_quaternion=tf.transformations.quaternion_from_euler(0,0,data.theta)
    translation_vector=(data.x,data.y,0)
    current_time=rospy.Time.now()

    transform_broadcaster.sendTransform(translation_vector,rotation_quaternion,current_time,turtlename+"_frame","world")


if __name__ == '__main__':
    rospy.init_node('turtle_tf_broadcaster')
    turtlename=rospy.get_param('~turtle')

    rospy.Subscriber('/%s/pose'%turtlename,Pose,pose_callback,turtlename)

    rospy.spin()