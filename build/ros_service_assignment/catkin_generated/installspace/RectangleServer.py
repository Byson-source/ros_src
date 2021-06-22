import rospy

from ros_service_assignment.srv import RectangleAreaService
from ros_service_assignment.srv import RectangleAreaServiceRequest
from ros_service_assignment.srv import RectangleAreaServiceResponse

def handle_multiply_two_ints(data):
    print('Returning %s*%s=%s'%(data.a,data.b,data.a*data.b))
    return RectangleAreaResponse(data.a*data.b)

def add_twi_int():
    rospy.init_node('add_two_int')
    rospy.Service('add_two_ints',RectangleAreaService,handle_multiply_two_ints)
    print('Ready multiply two ints...')
    rospy.spin()

if __name__ == '__main__':
    add_twi_int()

