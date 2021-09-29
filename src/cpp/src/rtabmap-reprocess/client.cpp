#include <ros/ros.h>
#include <std_msgs/Int32.h>
#include <std_msgs/String.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include <cpp/RtabmapReprocessAction.h>
#include <cpp/RtabmapReprocessActionResult.h>
#include <stdio.h>
#include <cpp/StringArray.h>
#include <vector>

#define CHECK_REPROCESS_FINISH_TOPIC "RtabmapReprocess_node/result"
#define SWITCH_REPROCESS_TOPIC "Reprocess_start"
#define EXIT_TOPIC "wait_for_rtabmap_reprocess"
#define DATABASE_DIR_TOPIC "path_info"

class Reprocess_Client
{
protected:
    ros::NodeHandle nh;

    actionlib::SimpleActionClient<cpp::RtabmapReprocessAction> ac;

    std::string action_name;

    int status{0};

    std::string goal_path;
    std::vector<std::string> databases;

    ros::Subscriber result_checker;
    ros::Subscriber switch_sub;
    ros::Subscriber databasepath_receiver;

    ros::Publisher restart_loop_closure;

public:
    Reprocess_Client(std::string name)
        : action_name{name}, ac(action_name, true)
    {
        ROS_INFO("launching Rtabmap-reprocess...");

        result_checker = nh.subscribe(CHECK_REPROCESS_FINISH_TOPIC, 10,
                                      &Reprocess_Client::checkCB, this);
        switch_sub = nh.subscribe(SWITCH_REPROCESS_TOPIC, 10,
                                  &Reprocess_Client::switchCB, this);

        databasepath_receiver = nh.subscribe(DATABASE_DIR_TOPIC, 10, &Reprocess_Client::dirCB, this);

        restart_loop_closure = nh.advertise<std_msgs::Int32>(EXIT_TOPIC, 10);
    }

    void dirCB(const cpp::StringArray::ConstPtr &msg)
    {
        for (int i = 1; !i == msg->strings.size(); i++)
        {
            databases.push_back(msg->strings[i]);
        }
        goal_path = msg->strings[0];
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

            ac.waitForServer();
            ROS_INFO("Rtabmap-reprocess server started, sending goal...");

            cpp::RtabmapReprocessGoal goal;
            for (auto val : databases)
                goal.databasepaths.push_back(val);

            goal.goal_path = goal_path;

            ac.sendGoal(goal);
        }
    }
};

int main(int argc, char **argv)
{
    ros::init(argc, argv, "reprocess_client_node");

    Reprocess_Client client_agent("RtabmapReprocess_node");
    ros::spin();

    return 0;
}