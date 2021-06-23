import rospy
from std_msgs.msg import String
pub = rospy.Publisher('Your_Coffee', String, queue_size=10)
"""(message Type,型,バッファの数"""
def callback(data):
    #loginfoはprintと同じ意味？
    print(rospy.get_caller_id() + 'I heard %s'%(data.data))
    """%s はオブジェクトをstr型で返す"""
    pub.publish(data)

def prepare():

    # In ROS, nodes are uniquely named. If two nodes with the same
    # name are launched, the previous one is kicked off. The
    # anonymous=True flag means that rospy will choose a unique
    # name for our 'listener' node so that multiple listeners can
    # run simultaneously.
    rospy.init_node('Preparing_Hot_Coffee', anonymous=True)
#"listener"というノードを作っている
    rospy.Subscriber('ingredients' , String, callback)
#'chatter'というトピックはtalker.pyと共通している。
    rospy.spin()
    # spin() simply keeps python from exiting until this node is stopped


if __name__ == '__main__':
    prepare()

