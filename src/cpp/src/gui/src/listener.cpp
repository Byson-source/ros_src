#include "listener.h"
#include <iostream>
#include "ros/ros.h"
#include "std_msgs/Int32.h"
#include "std_msgs/String.h"

#define GUI_SWITCH_TOPIC "gui_info"
#define STORED_DIR_TOPIC "stored_dir"

listener_node::listener_node()
{
    sub1.subscribe(GUI_SWITCH_TOPIC,10,&listener_node::gui_Callback,this);
    sub2.subscribe(STORED_DIR_TOPIC,10,&listener_node::dir_callback,this);
    // ros::spin();
}

listener_node::gui_Callback(const std_msgs::Int32::ConstPtr &msg){
    // previous_status=status;
    if(msg->data==1)
        status=1;
    else
        status=0;
}

listener_node::dir_callback(const std_msgs::String::ConstPtr &msg){
    database_path=msg.data->c_str;
    
}