#!/usr/bin/env python3

from service_practice.srv import AddTwoInts
from service_practice.srv import AddTwoIntsRequest
from service_practice.srv import AddTwoIntsResponse

import rospy

def handle_add_two_ints(data):
    print("Returning [%s+%s=%s]"%(data.a, data.b, (data.a+data.b)))
    return AddTwoIntsResponse(data.a+data.b)#sumに書き込んでいる。。。
def add_two_ints_server():
    rospy.init_node('Adder')
    s=rospy.Service('Add_two_ints',AddTwoInts,handle_add_two_ints)
    print("Ready to go...")#Listening....

    rospy.spin()


if __name__=="__main__":
    add_two_ints_server()


