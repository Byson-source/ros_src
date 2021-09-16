#include "listener.h"
#include <iostream>
#include "ros/ros.h"
#include "std_msgs/String.h"

listener_node::listener_node()
{
    sub1.subscribe("gui_info",1000,gui_Callback,this);
    sub2.subscribe("stored_dir",1000,dir_callback);
    ros::spin();
}

listener_node::gui_Callback(const std_msgs::String::ConstPtr &msg){
    if(&msg.data->c_str()=="show_me")
        status=1;
    else
        status=0;
}

listener_node::dir_callback(const std_msgs::String::ConstPtr &msg){
    database_path=msg.data->c_str;
    
}

// listener_node::notify(){
//     if (status==1){
//         emit valueChanged(status);
//         status-=1;
//         std::cout<<"GUI is activating..."<<std::endl;
// }


