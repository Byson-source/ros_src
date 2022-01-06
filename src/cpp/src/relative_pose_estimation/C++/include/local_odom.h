#ifndef LOCAL_ODOM_H
#define LOCAL_ODOM_H
#include "ros/ros.h"
#include "nav_msgs/Odometry.h"
#include <iostream>
#include <vector>
#include <map>


class Local_odom{
    private:
    ros::NodeHandle n;
    ros::Subscriber twist_catcher;
    ros::Publisher odom_transmitter;

    int pose_num;
    std::vector<double> onesec_odoms;
    std::map<std::string,double> local_dict;
    std::map<std::string,double> output;
    

    public:
    Local_odom(int local_num):pose_num{local_num}
    {
        twist_catcher=n.subscribe("robot1/odom",1000,&Local_odom::twistCB,this);
        odom_transmitter=n.advertise
    }

    void twistCB(const nav_msgs::Odometry::ConstPtr &msg){
        local_dict["x"]+=msg->twist.linear.x;
        local_dict["y"]+=msg->twist.linear.y;
        local_dict["z"]+=msg->twist.linear.z;
        local_dict["roll"]+=msg->twist.angular.x;
        local_dict["pitch"]+=msg->twist.angular.y;
        local_dict["yaw"]+=msg->twist.angular.z;
    }

    std::map sec_odom(void){
        output=local_dict;
        // All clear
        for(auto [key,val]:local_dict){
            val=0.0;
        }

        return output;

    }
};

#endif