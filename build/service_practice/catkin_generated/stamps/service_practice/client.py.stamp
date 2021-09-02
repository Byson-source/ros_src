#!/usr/bin/env python3

import sys
import rospy
from service_practice.srv import AddTwoInts
from service_practice.srv import AddTwoIntsRequest
from service_practice.srv import AddTwoIntsResponse

def add_two_ints_clients(x,y):
    rospy.wait_for_service("Add_two_ints")
    try:
        add_two_ints=rospy.ServiceProxy("Add_two_ints",AddTwoInts)
        resp1=add_two_ints(x,y)
        return resp1.sum
    except rospy.ServiceException():
        print("Service call failed")


def usage():
    return

if __name__ == '__main__':
    if len(sys.argv)==3:
        x=int(sys.argv[1])
        y=int(sys.argv[2])
    else:
        sys.exit(1)

    print("Requesting %s+%s"%(x,y))
    s=add_two_ints_clients(x,y)#使いパシリ
    print("%s+%s=%s"%(x,y,s))