#include <ros/ros.h>
#include <std_msgs/Int32.h>
#include <std_msgs/String.h>
#include <actionlib/server/simple_action_server.h>
#include <stdio.h>
#include "rtabmap_ros/LoadDatabase.h"
#include <cpp/StringArray.h>
#include <vector>
#include <iostream>

#define FROM_ROBOT1"LoopClosureDetection"
#define ACTIVATE_REPROCESS "Reprocess_start"

class LoadDatabase_Client
{
protected:
    ros::NodeHandle nh;

    std::string robot_name;
    

    ros::ServiceClient client;


    std::string template_database_path{"/home/ayumi/Documents/RTAB-Map/Experiment/No"};
    std::string template_loaddatabase_topic{"/rtabmap/load_database"};

    int database_num{1};

    std::string srv_topics;
    std::string previous_path;

    ros::Subscriber switch_sub;

    ros::Publisher reprocess_commander;

public:
    // LoadDatabase_Client(std::string name) :robot_name{name}
    LoadDatabase_Client(std::string name) : robot_name{name}
    {
        ROS_INFO("launching Loaddatabase service...");

        srv_topics = robot_name + "/" + template_loaddatabase_topic;


        ros::service::waitForService(srv_topics);

        client = nh.serviceClient<rtabmap_ros::LoadDatabase>(srv_topics);

        switch_sub = nh.subscribe(FROM_ROBOT1, 10,
                                  &LoadDatabase_Client::switchCB, this);

        reprocess_commander = nh.advertise<std_msgs::Int32>(ACTIVATE_REPROCESS, 10);

        rtabmap_ros::LoadDatabase srv;

        srv.request.database_path = template_database_path + std::to_string(database_num) + "_" + robot_name + ".db";
        srv.request.clear = true;

        database_num += 1;

        if (client.call(srv))
        {
            ROS_INFO("Loading succeeded!");
        }
        else
        {
            ROS_ERROR("Failed to call LoadDatabase service");
        }
    }

    void checkCB(void)
    {
        std_msgs::Int32 data;
        ROS_INFO("Loading finish!!");

        data.data = 1;
        reprocess_commander.publish(data);
    }

    void switchCB(const std_msgs::Int32::ConstPtr &msg)
    {
        if (msg->data == 1)
        {
            std_msgs::String reading_path;

            reading_path.data = previous_path;

            previous_path = "";

            rtabmap_ros::LoadDatabase srv;

            srv.request.database_path = template_database_path + std::to_string(database_num) + "_" + robot_name + ".db";
            srv.request.clear = true;

            database_num += 1;

            if (client.call(srv))
            {
                ROS_INFO("Loading succeeded!");
            }
            else
            {
                ROS_ERROR("Failed to call LoadDatabase service");
            }

            previous_path = srv.request.database_path;

            LoadDatabase_Client::checkCB();
        }
    }
};

int main(int argc, char **argv)
{
    ros::init(argc, argv, "LoadDatabase_client");

    ros::NodeHandle pnh("~");
    std::string name;
    pnh.getParam("name", name);
    LoadDatabase_Client client_agent(name);

    ros::spin();

    return 0;
}