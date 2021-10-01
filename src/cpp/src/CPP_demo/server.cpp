#include "ros/ros.h"
#include "cpp/AddTwoInts.h"
#include "cpp/AddThreeInts.h"

bool add(cpp::AddTwoInts::Request &req, cpp::AddTwoInts::Response &res)
{
    res.sum=req.a+req.b;
    ROS_INFO("request: x=%ld, y=%ld", (long int)req.a, (long int) req.b);
    ROS_INFO("sending back response...[%ld]",(long int) res.sum);
    return true;
}

bool add2(cpp::AddThreeInts::Request &req, cpp::AddThreeInts::Response &res)
{
    res.result=req.a+req.b+req.c;

    return true;
}

int main(int argc, char **argv)
{
    ros::init(argc,argv, "add_two_ints_server_node");
    ros::NodeHandle n;

    ros::ServiceServer service = n.advertiseService("add_two_ints",add);
    ros::ServiceServer service2 = n.advertiseService("add_three_ints",add2);
    ROS_INFO("Ready to add two ints...");
    ros::spin();

    return 0;
}
