#include <ros/ros.h>
#include <std_msgs/Int32.h>
#include <std_msgs/String.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include <cpp/LoopClosureAction.h>
#include <cpp/LoopClosureActionResult.h>
#include <boost/thread.hpp>
#include <string>
#include <stdio.h>

#define TEMPLATE_IMAGE_PATH "/home/ayumi/Documents/tohoku_uni/CLOVERs/"
#define DIR_INFO_TOPIC "dir_info"
#define REST_TOPIC "take_rest"
#define RESTART_TOPIC "wait_for_rtabmap_reprocess"
#define RESULT_INFO "loop_closure/result"
#define FIRST_IMAGEPATH "/home/ayumi/Documents/tohoku_uni/CLOVERs/1-image/"
#define ROBOT1_ACTIVATE_LOAD_TOPIC "LoopClosureDetection"

class Client
{
private:
    ros::NodeHandle n;

    actionlib::SimpleActionClient<cpp::LoopClosureAction> ac;

    ros::Subscriber switch_sub;
    ros::Subscriber result_catcher;
    ros::Subscriber image_chatter;

    //To turn on-off the node that conducts rtabmap-reprocess
    ros::Publisher robot1_load_commander;
    //To notify the change of location where the image is stored
    ros::Publisher dir_changer;
    //To turn on-off the image extractor node.
    ros::Publisher rest_commander;

    std::string image_path;
    std::string activate_load_topic{"LoopClosureDetection"};

    ros::Rate loop_rate{0.5};

    int status{1};
    int previous_status{0};
    int dir_number{0};

public:
    Client(std::string name) : image_path{FIRST_IMAGEPATH},ac(name,true)
    {

        ROS_INFO("LoopClosure_client now launching...");

        robot1_load_commander = n.advertise<std_msgs::Int32>(ROBOT1_ACTIVATE_LOAD_TOPIC, 10);
        dir_changer = n.advertise<std_msgs::String>(DIR_INFO_TOPIC, 10);
        rest_commander = n.advertise<std_msgs::Int32>(REST_TOPIC, 10);

        switch_sub = n.subscribe(RESTART_TOPIC, 10, &Client::switchCB, this);
        result_catcher = n.subscribe(RESULT_INFO, 10, &Client::result_CB, this);
        image_chattter=n.subscribe("current_image",)

        // //At the first time, the client sends the request.
        // actionlib::SimpleActionClient<cpp::LoopClosureAction> ac(action_name, true);

        ROS_INFO("Waiting for action server to start.");
        ac.waitForServer();

        ROS_INFO("Action server started, sending goal.");
        // send a goal to the action
        cpp::LoopClosureGoal goal;
        goal.imagepath = image_path;
        ac.sendGoal(goal);
    }

    void send_command(void)
    {
        if (status == 0)
        {
            std_msgs::Int32 for_load_database;
            std_msgs::Int32 rest_command;
            //Start rtabmap-reprocess
            for_load_database.data = 1;
            rest_command.data = 1;

            robot1_load_commander.publish(for_load_database);
            rest_commander.publish(rest_command);
        }
        else
        {
            //Start loop-closure detection
            std_msgs::String dir_info;
            std_msgs::Int32 rest_command;

            dir_info.data = image_path;
            dir_changer.publish(dir_info);

            rest_command.data = 0;

            rest_commander.publish(rest_command);
        }
    }

    void change_dir(void)
    {
        image_path = TEMPLATE_IMAGE_PATH + std::to_string(dir_number) + "-image/";
    }

    //Wait for rtabmap-reprocess. Then restart the loop closure detection
    void switchCB(const std_msgs::Int32::ConstPtr &msg)
    {

        status = msg->data; //==1

        dir_number += 1;
        change_dir();

        send_command();

        // actionlib::SimpleActionClient<cpp::LoopClosureAction> ac(action_name, true);

        ROS_INFO("Waiting for action server to start.");
        ac.waitForServer();

        ROS_INFO("Action server started, sending goal.");
        // send a goal to the action
        cpp::LoopClosureGoal goal;
        goal.imagepath = image_path;
        ac.sendGoal(goal);
    }

    void result_CB(const cpp::LoopClosureActionResult::ConstPtr &msg)
    {
        if (msg->result.result == 1)
        {
            ROS_INFO("result was back!!!!");
            status = 0;
            Client::send_command();
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