import rospy
from std_msgs.msg import String
def callback(data):
    print(data.data)
def subscriber():
    rospy.init_node('Serving_Coffee', anonymous=True)
    rospy.Subscriber('Your_Coffee', String, callback)
    rospy.spin()

if __name__ == '__main__':
    subscriber()
