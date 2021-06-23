import sys
import rospy
from new_ros.srv import *

def add_two_ints_client(x,y):
    rospy.wait_for_service('add_two_ints')
    try:
        add_two_ints=rospy.ServiceProxy('add_two_ints',AddTwoPoints)
        """↓ここで送信と受信を同時に行っている！！"""
        respl=add_two_ints(x,y)
        return respl.sum

    except rospy.ServiceException(e):
        print('Service failed:%s'%e)

if __name__ == '__main__':
    if len(sys.argv)==3:
        x=int(sys.argv[1])
        y=int(sys.argv[2])
    else:
        print('Failed')
        sys.exit(1)

    print('Requesting....%s+%s=?' % (x, y))
    print(add_two_ints_client(x,y))

