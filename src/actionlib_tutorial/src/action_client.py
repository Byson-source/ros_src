import roslib
roslib.load_manifest('actionlib_tutorial')
import rospy
import actionlib

from actionlib_tutorial.msg import DoDishesAction, DoDishesGoal

if __name__ == '__main__':

    rospy.init_node('do_dishes_client')
    client=actionlib.SimpleActionClient('do_dishes',DoDishesAction)

    #serverの準備が終わるまで待つ
    client.wait_for_server()

    goal=DoDishesGoal()
    client.send_goal(goal)
    client.wait_for_result(rospy.Duration.from_sec(5.0))
