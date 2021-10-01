#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "turtlesim/Pose.h"

double turtle_x{5.5};
double turtle_y{5.5};
double *turtle_x_ptr{&turtle_x};
double *turtle_y_ptr{&turtle_y};

void move(double distance, ros::Publisher &commander)
{
    geometry_msgs::Twist vel_msg;

    vel_msg.linear.x=5;

//    double t0{ros::Time::now().toSec()};
    double current_distance{0.0};
    ros::Rate loop_rate{100};

    while(current_distance<distance)
    {
        commander.publish(vel_msg);
//        double t1=ros::Time::now().toSec();
        current_distance=std::pow(std::pow(turtle_x,2)+std::pow(turtle_y,2),0.5);
        ros::spinOnce();
        loop_rate.sleep();
    }
    vel_msg.linear.x=0.0;
    commander.publish(vel_msg);

}

void posecallback(const turtlesim::Pose &data)
{

    *turtle_x_ptr=data.x;
    *turtle_y_ptr=data.y;
    ROS_INFO("Now, the x of turtle is %lf, y is %lf", turtle_x,turtle_y);
}

int main(int argc, char **argv){
    ros::init(argc,argv,"turtlesimNode");
    ros::NodeHandle node;
    ros::Subscriber sub=node.subscribe("turtle1/pose",1000,posecallback);
    ros::Publisher pub=node.advertise<geometry_msgs::Twist>("turtle1/cmd_vel",1000);
    move(20,pub);
    ros::spin();

    return 0;
}

