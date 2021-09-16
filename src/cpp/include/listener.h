#ifndef LISTENER_H
#define LISTENER_H
#include "listener.h"
#include "ros/ros.h"
#include "std_msgs/String.h"
#include <string>
#define FIRST_DATABASE_PATH "/home/ayumi/.ros/"
class listener_node
{

public:
    listener_node();
    void gui_Callback(const std_msgs::String::ConstPtr &msg);
    void dir_callback(const std_msgs::String::ConstPtr &msg);

protected:
    ros::NodeHandle n;
    ros::Subscriber sub1;
    ros::Subscriber sub2;
    int status{0};
    string database_path{FIRST_DATABASE_PATH};

};

#endif