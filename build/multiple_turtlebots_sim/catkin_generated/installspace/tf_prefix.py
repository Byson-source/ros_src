

import rospy
import rosparam
from tf2_msgs.msg import TFMessage

def callback_tftopic(data):
    global publisher, prefix
    n_transforms = len(data.transforms)
    print(n_transforms)
    for i in range(n_transforms):
        if prefix not in data.transforms[i].header.frame_id or prefix not in data.transforms[i].child_frame_id:
            data.transforms[i].header.frame_id = prefix + data.transforms[i].header.frame_id
            data.transforms[i].child_frame_id = prefix + data.transforms[i].child_frame_id
        publisher.publish(data)
        rate.sleep()

if __name__ == '__main__':
    rospy.init_node('tf_prefixer')

    publisher = rospy.Publisher("/tf", TFMessage, queue_size=10)
    # prefix = rospy.get_param('tf_prefix')
    prefix="test/"
    rate=rospy.Rate(100)
    listener = rospy.Subscriber("tf", TFMessage, callback_tftopic)


    rospy.spin()