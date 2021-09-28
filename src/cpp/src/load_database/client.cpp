#include <ros/ros.h>
#include <std_msgs/Int32.h>
#include <std_msgs/String.h>
#include <actionlib/server/simple_action_server.h>
#include <stdio.h>
#include "rtabmap_ros/LoadDatabase.h"
#include <cpp/StringArray.h>
#include <vector>

#define SWITCH_LOAD_TOPIC "LoopClosureDetection"
#define ACTIVATE_REPROCESS_TOPIC "Reprocess_start"
#define PATH_TOPIC "path_info"

class LoadDatabase_Client
{
protected:
    ros::NodeHandle nh;

    int robots_num;

    std::vector<ros::ServiceClient> clients;

    std::string action_name;

    std::string template_database_path{"/home/ayumi/Documents/RTAB-Map/Experiment/No"};
    std::string template_loaddatabase_topic{"/rtabmap/load_database"};

    int status{1};
    int database_num{1};

    std::vector<std::string> databases;
    std::vector<std::string> srv_topics;
    std::vector<std::string> previous_pathinfo;

    ros::Subscriber result_checker;
    ros::Subscriber switch_sub;

    ros::Publisher databasepath_transmitter;
    ros::Publisher reprocess_commander;

public:
    LoadDatabase_Client(int num) : robots_num{num}
    {
        ROS_INFO("launching Rtabmap-reprocess...");

        for (int i = 0; !i == robots_num; i++)
        {
            srv_topics.push_back("No." + std::to_string(i) + template_loaddatabase_topic);
            clients.push_back(nh.serviceClient<rtabmap_ros::LoadDatabase>(srv_topics[i]));
        }

        switch_sub = nh.subscribe(SWITCH_LOAD_TOPIC, 10,
                                  &LoadDatabase_Client::switchCB, this);

        databasepath_transmitter = nh.advertise<cpp::StringArray>(PATH_TOPIC, 10);
        reprocess_commander = nh.advertise<std_msgs::Int32>(ACTIVATE_REPROCESS_TOPIC, 10);
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
        if (msg->data == 1 || status == 1)
        {
            status = 0;
            if (!previous_pathinfo.size() == 0)
            {
                cpp::StringArray reading_path;

                for (auto val : previous_pathinfo)
                    reading_path.strings.push_back(val);

                databasepath_transmitter.publish(reading_path);
                previous_pathinfo.clear();
            }

            for (int i = 0; !i == robots_num; i++)
            {
                rtabmap_ros::LoadDatabase srv;

                srv.request.database_path = template_database_path + std::to_string(database_num) + ".db";
                srv.request.clear = true;

                database_num += 1;

                if (clients[i].call(srv))
                {
                    ROS_INFO("Loading succeeded!");
                }
                else
                {
                    ROS_ERROR("Failed to call LoadDatabase service");
                    break;
                }

                previous_pathinfo.push_back(srv.request.database_path);
            }

            LoadDatabase_Client::checkCB();
        }
    }
};

int main(int argc, char **argv)
{
    ros::init(argc, argv, "LoadDatabase_client");

    ros::NodeHandle pnh("~");
    int number;
    pnh.getParam("num", number);

    LoadDatabase_Client client_agent(number);

    ros::spin();

    return 0;
}