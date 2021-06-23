import rospy
import tf
from gazebo_msgs.msg import ModelStates

def pose_callback(data):
    transform_broadcaster=tf.TransformBroadcaster()

    q=data.pose[4].orientation
    pos=data.pose[4].position

    rotation_quaternion=(q.x,q.y,q.z,q.w)
    translation_vector=(pos.x-goal_x,pos.y-goal_y,0)
    current_time=rospy.Time.now()

    transform_broadcaster.sendTransform(translation_vector, rotation_quaternion, current_time,
                                            "goal_frame", "waffle_frame")
def callback(data):
    print(data.twist[4])

if __name__ == '__main__':

    rospy.init_node('turtle_tf_broadcaster')
    #Target
    # goal_x=rospy.get_param('~goal_x')
    # goal_y = rospy.get_param('~goal_y')

    goal_x=int(input('What is the x of target?>>>>'))
    goal_y = int(input('What is the y of target?>>>>'))

    rospy.Subscriber('/gazebo/model_states',ModelStates,pose_callback)
    # rospy.Subscriber('/gazebo/model_states',ModelStates,callback)
    rospy.spin()