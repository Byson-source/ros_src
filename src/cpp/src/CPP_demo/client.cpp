#include"ros/ros.h"
#include "cpp/AddTwoInts.h"
#include "cpp/AddThreeInts.h"
#include <cstdlib>

int main(int argc, char **argv)
{
    ros::init(argc, argv, "add_two_ints_client");
    ros::NodeHandle n;
    

    ros::ServiceClient client=n.serviceClient<cpp::AddTwoInts>("add_two_ints");
    ros::ServiceClient client2=n.serviceClient<cpp::AddThreeInts>("add_three_ints");
    cpp::AddTwoInts srv;
    cpp::AddThreeInts srv2;
    srv.request.a=3;
    srv.request.b=4;

    srv2.request.a=10;
    srv2.request.b=10;
    srv2.request.c=10;


    if(client.call(srv))
    {
        ROS_INFO("Sum:%ld",(long int)srv.response.sum);
    }
    else{
        ROS_ERROR("Failed to call service add_two_ints");
        return 1;

    }

    if(client2.call(srv2))
    {
        ROS_INFO("Sum:%ld",(long int)srv2.response.result);
    }
    else{
        ROS_ERROR("Failed to call service add_three_ints");
        return 1;

    }
    return 0;

}
