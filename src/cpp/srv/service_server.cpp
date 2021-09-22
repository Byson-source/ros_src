#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "geometry_msgs/Pose.h"
#include "std_msgs/String.h"

bool maxAngle(cpp::srvDemo::Request &req,cpp::srvdemo &res)
{
    ros::NodeHandle nh;
    ros::Subscriber sub_laser=nh.subscribe("/scan",1,maxScanCallback);
    ros::Subscriber sub_odom=nh.subscribe("/odom",1,headingCallback);
    ros::Publisher pub_vel=nh.advertise<geometry_msgs::Twist>("/cmd_vel",1);
    ros::rate rate(50);

    geometry_msgs::Twist move;
    double init_angle;

    int calib_count{50};
    while(calib_count--){
        init_angle=calibrate();
        ros::spinOnce();
        rate.sleep();
    }
    std::cout<<"Initial angle is "<<init_angle<<'\n';
    double target_angle=getTarget360(init_angle);
    std::cout<<"Target angle is "<<target_angle<<'\n';

    double max_angle{init_angle};
    double max_dist{0};
    double diff;
    move.angular.z=0.15;

    while(ros::ok()){
        diff=heading_angle-target_angle;
    }



}
int main(int argc,char **argv){
    ros::init(argc,argv,"service_server");
    ros::NodeHandle nh;
    ros::ServiceServer ser=nh.advertiseService("ros_service_demo",maxAngle);

    ROS_INFO("server is ready!!");
    ros::spin();
    return 0;
}