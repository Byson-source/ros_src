from custom_srv.srv import *
import rospy

def Concatinate_name(req):
    return newResponse(req.name1+req.name2)
def Security_server():
    rospy.init_node('Security')
    rospy.Service('Name_confirmation',new,Concatinate_name)
    rospy.spin()
if __name__=="__main__":
    Security_server()
"""Subscriber"""