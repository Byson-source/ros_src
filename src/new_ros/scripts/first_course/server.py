from new_ros.srv import *

import time
import rospy
def handle_add_two_ints(data):
    print("Returning %s+%s=%s"%(data.a,data.b,data.a+data.b))
    time.sleep(5)
    sum_response=AddTwoPointsResponse(data.a+data.b)
    return sum_response

def add_two_ints_server():
    rospy.init_node("add_two_ints_server")
    s=rospy.Service("add_two_ints",AddTwoPoints,handle_add_two_ints)
    print('Ready to add two ints')
    rospy.spin()


if __name__ == '__main__':
    add_two_ints_server()
