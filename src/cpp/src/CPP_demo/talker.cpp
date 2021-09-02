#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>//stringstream is included

int main(int argc, char **argv)
{
    ros::init(argc, argv, "talker_node");
    ros::NodeHandle n;
    //rospy.init_node("node_name",anonymous=True)
    ros::Publisher chatter_publisher=n.advertise<std_msgs::String>("chatter",1000);
    ros::Rate loop_rate(0.5);

    int count{0};
    while (ros::ok())
    {
        std_msgs::String msg;
        std::stringstream ss;
        ss<<"Hello World "<<count;
        msg.data=ss.str();//The content in ss
        ROS_INFO("[Talker] I published %s\n",msg.data.c_str());
        chatter_publisher.publish(msg);
        ros::spinOnce();
        loop_rate.sleep();
        count++;
    }
    return 0;
}
