#include <ros/ros.h>
#include <std_msgs/Int32.h>
#include <std_msgs/String.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include <cpp/RtabmapReprocessAction.h>
#include <cpp/RtabmapReprocessActionResult.h>
#include <stdio.h>
#include <vector>

#define CHECK_REPROCESS_FINISH_TOPIC "RtabmapReprocess_node/result"
#define SWITCH_REPROCESS_TOPIC "Reprocess_start"
#define EXIT_TOPIC "wait_for_rtabmap_reprocess"
#define GOAL_DATABASEPATH "/home/ayumi/.ros/rtabmap.db"

class Reprocess_Client
{
protected:
    ros::NodeHandle nh;

    actionlib::SimpleActionClient<cpp::RtabmapReprocessAction> ac;

    std::string template_databasepath{"/home/ayumi/Documents/RTAB-Map/Experiment/No"};

    int status{0};
    int database_num{0};
    int robot_number;

    std::string goal_path;

    ros::Subscriber result_checker;
    ros::Subscriber switch_sub;

    ros::Publisher restart_loop_closure;

public:
    Reprocess_Client(std::string name, int number)
        :ac(name, true), robot_number{number}
    {
        ROS_INFO("launching Rtabmap-reprocess...");

        goal_path = GOAL_DATABASEPATH;

        result_checker = nh.subscribe(CHECK_REPROCESS_FINISH_TOPIC, 10,
                                      &Reprocess_Client::checkCB, this);
        switch_sub = nh.subscribe(SWITCH_REPROCESS_TOPIC, 10,
                                  &Reprocess_Client::switchCB, this);

        restart_loop_closure = nh.advertise<std_msgs::Int32>(EXIT_TOPIC, 10);
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

            database_num += 1;

            ac.waitForServer();
            ROS_INFO("Rtabmap-reprocess server started, sending goal...");

            cpp::RtabmapReprocessGoal goal;

            for (int i{1}; !i == robot_number + 1; i++)
                goal.databasepaths.push_back(template_databasepath + std::to_string(database_num) + "_robot" + std::to_string(i) + ".db");

            goal.goal_path = goal_path;
            

            ac.sendGoal(goal);
        }
    }
};

int main(int argc, char **argv)
{
    ros::init(argc, argv, "reprocess_client_node");
    ros::NodeHandle pnh("~");
    int number;
    pnh.getParam("robot_num", number);

    Reprocess_Client client_agent("RtabmapReprocess_node", number);
    ros::spin();

    return 0;
}