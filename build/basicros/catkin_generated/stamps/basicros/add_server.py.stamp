import rospy

from basicros.srv import AddTwoPoints
from basicros.srv import AddTwoPointsRequest
from basicros.srv import AddTwoPointsResponse

def handle_add_two_ints(data):
    print('Returning %s+%s=%s'%(data.a,data.b,data.a+data.b))
    return AddTwoPointsResponse(data.a+data.b)

def add_twi_int():
    rospy.init_node('add_two_int')
    rospy.Service('add_two_ints',AddTwoPoints,handle_add_two_ints)
    print('Ready add two ints...')
    rospy.spin()

if __name__ == '__main__':
    add_twi_int()

