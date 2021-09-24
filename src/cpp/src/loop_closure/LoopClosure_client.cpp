#include <ros/ros.h>
#include <direct.h>
#include <std_msgs/int32.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include <cpp/AveragingAction.h>
#include <boost/thread.hpp>
#include <string>
#include <stdio.h>

#define TEMPLATE_IMAGE_PATH "/home/ayumi/Documents/CLOVERs/"

class Client
{
private:
    ros::NodeHandle n;

    ros::Subscriber switch_sub;

    //To turn on-off the node that conducts rtabmap-reprocess
    ros::Publisher rtabmap_reprocess_commander;
    //To notify the change of location where the image is stored
    ros::Publisher dir_changer;
    //To turn on-off the image extractor node.
    ros::Publisher rest_commander;

    std::string image_path;
    std::string action_name;

    std_msgs::Int32 for_rtabmap_reprocess;
    std_msgs::String dir_info;
    std_msgs::Int32 rest_command;

    ros::Rate loop_rate{0.5};

    int status{1};
    int previous_status{0};
    int dir_number{0};

public:
    Client(std::string name) : action_name(name)
    {
        ROS_INFO("Now launching...");

        rtabmap_reprocess_commander = n.advertise<std_msgs::Int32>("LoopClosureDetection", 10);
        dir_changer = n.advertise<std_msgs::String>("dir_info", 10);
        rest_commander = n.advertise<std_msgs::Int32>("take_rest", 10);

        switch_sub = n.subscribe("waiting_for_reprocess", 10, Client::switchCB, this);
    }

    void send_command(void)
    {

        dir_info.data = image_path;
        dir_changer.publish(dir_info);

        if (status == 0)
        {
            for_rtabmap_reprocess.data = 1;
            rest_command.data = 1;

            rtabmap_reprocess_commander.publish(for_rtabmap_reprocess);
            rest_commander.publish(rest_commander);
        }
        else
        {
            for_rtabmap_reprocess.data = 0;
            rest_command.data = 0;

            rtabmap_reprocess_commander.publish(for_rtabmap_reprocess);
            rest_commander.publish(rest_commander);
            
        }
    }

    void change_dir(void)
    {
        std::string new_path = TEMPLATE_IMAGE_PATH + std::to_string(dir_number) + "-image/";
        image_path = new_path;

        if (_mkdir(new_path) == 1)
            printf("Directory was successfully created!");
        else
        {
            printf("There is a directory already exists. Now deleting...");
            _rmdir(new_path);
            _mkdir(new_path);
        }
    }

    void switchCB(const std_msgs::int32::ConstPtr &msg)
    {

        status = msg->data;
        //Loop closure ON
        if (status == 1 && !previous_status == status)
        {
            dir_number += 1;
            change_dir();

            previous_status = 1;

            actionlib::SimpleActionClient<cpp::LoopClosureAction> ac(action_name, true);

            ROS_INFO("Waiting for action server to start.");
            ac.waitForServer();

            ROS_INFO("Action server started, sending goal.");
            // send a goal to the action
            cpp::LoopClosureGoal goal;
            goal.databasepath = image_path;
            ac.sendGoal(goal);

            bool finished_before_timeout = ac.waitForResult(ros::Duration(3000));
            if (finished_before_timeout)
            {
                actionlib::SimpleClientGoalState state = ac.getState();
                ROS_INFO("Action finished: %s", state.toString().c_str());
            }
            else
                ROS_INFO("Action did not finish before the time out.");
            
        }
        else
        {
            previous_status = status;
        }
    }
};

int main(int argc, char **argv)
{
    ros::init(argc, argv, "loop_closure_client");
    Client client_agent("loop_closure");

    ros::spin();

    return 0;
}