#ifndef LISTENER_H
#define LISTENER_H
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Int32.h"
#include <string>
#include <QString>
class listener_node
{

public:
    listener_node();
    void gui_Callback(const std_msgs::Int32::ConstPtr &msg);
    void dir_callback(const std_msgs::String::ConstPtr &msg);

protected:
    ros::NodeHandle n;
    ros::Subscriber sub1;
    ros::Subscriber sub2;
    
    int status{0};
    int previous_status=0;
    QString database_path{"/home/ayumi/.ros/"};

};

#endif