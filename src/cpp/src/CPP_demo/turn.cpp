#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "turtlesim/Pose.h"
#include <stdlib.h>

double turtle_yaw{0.0};
double *turtle_ptr{&turtle_yaw};

void turtle_turn(const double yaw, ros::Publisher &pub){

    double rest_yaw{yaw};
    geometry_msgs::Twist yaw_msg;
    yaw_msg.angular.z=rest_yaw/2;
    ros::Rate loop_rate{100};

    while(rest_yaw>3.14/100){

        pub.publish(yaw_msg);
        rest_yaw=yaw-turtle_yaw;
        ros::spinOnce();
        loop_rate.sleep();
    }
    ROS_INFO("%lf",rest_yaw);
    yaw_msg.angular.z=0;
    pub.publish(yaw_msg);
}

void posecallback(const turtlesim::Pose &data){
    *turtle_ptr=data.theta;
}


int main(int argc, char **argv){

    ros::init(argc,argv,"turnTurtle");
    ros::NodeHandle node;
    ros::Subscriber sub=node.subscribe("turtle1/pose",1000,posecallback);
    ros::Publisher pub=node.advertise<geometry_msgs::Twist>("turtle1/cmd_vel",1000);
    turtle_turn(3.14,pub);
    ros::spin();

    return 0;
}
