#include"ros/ros.h"
#include "cpp/AddTwoInts.h"
#include <cstdlib>

int main(int argc, char **argv)
{
    ros::init(argc, argv, "add_two_ints_client");
    ros::NodeHandle n;
    if(argc!=3)
    {
        ROS_INFO("Error occured...");
        return 1;
    }

    ros::ServiceClient client=n.serviceClient<cpp::AddTwoInts>("add_two_ints");
    cpp::AddTwoInts srv;
    srv.request.a=atoll(argv[1]);
    srv.request.b=atoll(argv[2]);

    if(client.call(srv))
    {
        ROS_INFO("Sum:%ld",(long int)srv.response.sum);
    }
    else{
        ROS_ERROR("Failed to call service add_two_ints");
        return 1;

    }
    return 0;

}
