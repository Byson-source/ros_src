import rospy
from sensor_msgs.msg import Joy
from geometry_msgs.msg import Twist
axes,buttons=[],[]
def perceive(data):
    global axes,buttons
    axes=data.axes
    buttons=data.buttons


def transmit():
    command.publish(cmd)
    rate.sleep()

if __name__ == '__main__':
    rospy.init_node('Operation_com')
    rate=rospy.Rate(10)
    cmd=Twist()

    rospy.Subscriber('clover2/joy',Joy,perceive)
    command=rospy.Publisher('clover2/cmd_vel',Twist,queue_size=1)

    while True:
        transmit()
