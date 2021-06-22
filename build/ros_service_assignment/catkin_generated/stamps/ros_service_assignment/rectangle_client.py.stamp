import sys
import rospy

from ros_service_assignment.srv import *

def add_two_int_client(x,y):
    rospy.wait_for_service('add_two_ints')
    try:
        add_two_integer=rospy.ServiceProxy('add_two_ints',RectangleAreaService)
        response=add_two_integer(x,y)
        return response.area
    except rospy.ServiceException(e):
        print('Service call failed...')

def usage():
    pass

if __name__ == '__main__':
    if len(sys.argv)==3:
        x=int(sys.argv[1])
        y = int(sys.argv[2])
    else:
        print('failed...')
        sys.exit(1)
    print('Requesting %s*%s='%(x,y))
    s=add_two_int_client(x,y)
    print('[%s*%s=%s]'%(x,y,s))
