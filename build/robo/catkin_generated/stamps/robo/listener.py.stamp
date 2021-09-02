#!/usr/bin/env python3
import rospy
from std_msgs.msg import String
"""Int8をインポートすると、数字が扱える。ただし、１２８まで"""

def callback(data):
    #loginfoはprintと同じ意味？
    print(rospy.get_caller_id() + 'I heard %s'%(data.data))
    # rospy.loginfo(rospy.get_caller_id() + 'I heard %s', data.data)

def listener():
    rospy.init_node('listener', anonymous=True)
#"listener"というノードを作っている
    rospy.Subscriber('chatter', String, callback)
#'chatter'というトピックはtalker.pyと共通している。
    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()

if __name__ == '__main__':
    listener()
