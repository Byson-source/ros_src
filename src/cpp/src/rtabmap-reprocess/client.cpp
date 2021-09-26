#include <ros/ros.h>
#include <std_msgs/Int32.h>
#include <std_msgs/String.h>
#include <actionlib/server/simple_action_server.h>
#include <cpp/RtabmapReprocess.h>
#include <cpp/RtabmapReprocessResult.h>
#include <stdio.h>

#define CHECK_REPROCESS_FINISH_TOPIC "RtabmapReprocess_node/result"
#define SWITCH_REPROCESS_TOPIC "LoopClosureDetection"
#define EXIT_TOPIC "wait_for_rtabmap_reprocess"
#define DATABASE_DIR_TOPIC "path_info"

class Reprocess_Client
{
protected:
    ros::NodeHandle nh;

    actionlib::SimpleActionServer<cpp::RtabmapReprocessAction> ac;

    std::string action_name;

    int status{0};

    std::string databasepath;

    ros::Subscriber result_checker;
    ros::Subscriber switch_sub;
    ros::Subscriber databasepath_receiver;

    ros::Publisher restart_loop_closure;

public:
    Reprocess_Client(std::string name)
        : action_name{name}, ac(nh, name, false)
    {
        ROS_INFO("launching Rtabmap-reprocess...");

        result_checker = nh.subscribe(CHECK_REPROCESS_FINISH_TOPIC, 10,
                                      &Reprocess_Client::checkCB, this);
        switch_sub = nh.subscribe(SWITCH_REPROCESS_TOPIC, 10,
                                  &Reprocess_Client::switchCB, this);

        databasepath_receiver = nh.subscribe(DATABASE_DIR_TOPIC, 10, &Reprocess_Client::dirCB, this);

        restart_loop_closure = nh.advertise<std_msgs::Int32>(EXIT_TOPIC, 10);
    }

    void dirCB(const std_msgs::String::ConstPtr &msg)
    {
        databasepath = msg->data;
    }

    void checkCB(const cpp::RtabmapReprocessActionResult::ConstPtr &msg)
    {
        if (msg->result.result == 1)
        {
            std_msgs::Int32 data;
            ROS_INFO("Rtabmap-reprocess finishes!!");

            data.data = 1;
            restart_loop_closure.publish(data);
        }
    }

    void switchCB(const std_msgs::Int32::ConstPtr &msg)
    {
        if (msg->data == 1)
        {

            ROS_INFO("Rtabmap-reprocess server started, sending goal...");
            ac.waitForServer();

            cpp::RtabmapReprocessGoal goal;
            goal.databasepath=databasepath;
            ac.sendGoal(goal);
        }
    }
};