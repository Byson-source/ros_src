#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Int32.h"
#include <unistd.h>

void chatterCallback(const std_msgs::String::ConstPtr &msg)
{
    ROS_INFO("[Listener] I heard: [%s]\n", msg->data.c_str());

    sleep(10);
    
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "listener_node");
    ros::NodeHandle node;
    ros::Subscriber sub=node.subscribe("chatter", 1000, chatterCallback);
    ros::spin();

    return 0;

}