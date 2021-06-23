import sys
import rospy
from custom_srv.srv import *
from geometry_msgs.msg import Twist

def security_verifying():
    pub=rospy.Publisher('/turtle1/cmd_vel',Twist)
    rospy.sleep(1)
    r=rospy.Rate(5.0)
    while not rospy.is_shutdown():
        twist=Twist()
        twist.linear.x=0.3
        for i in range(10):
            pub.publish(twist)
            r.sleep()
        twist=Twist()
        twist.angular.z=1.57/2
        for i in range(10):
            pub.publish(twist)
            r.sleep()


def concate_name_client(x,y):
    rospy.wait_for_service('Name_confirmation')
    concat_name=rospy.ServiceProxy('Name_confirmation',new)
    respi=concat_name(x,y)
    return respi

"""https://qiita.com/takahiro_itazuri/items/e585b46d096036bc837f"""

def usage():
    return "%s [x y]"%sys.argv[0]

if __name__=='__main__':
    rospy.init_node('mover_after_security')
    if len(sys.argv)==3:
        x=sys.argv[1]
        y=sys.argv[2]
        result = concate_name_client(x, y)
        if (result.fullname == 'UmemuraAyumi'):
            security_verifying()
        else:
            print('wrong person')
    else:
        print(usage())
        sys.exit(1)

    # print("%s+%s=%s"%(x,y,concate_name_client(x,y)))