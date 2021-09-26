#include <ros/ros.h>
#include <std_msgs/Int32.h>
#include <std_msgs/String.h>
#include <actionlib/server/simple_action_server.h>
#include <cpp/RtabmapReprocess.h>
#include <stdio.h>
#include <string>

class Reprocess_Server
{
protected:
    ros::NodeHandle n;

    actionlib::SimpleActionServer<cpp::RtabmapReprocessAction> as;
    std::string action_name;

    std::string 1_target_databasepath;

    cpp::RtabmapReprocessResult result;

public:
    AveragingAction(std::string name) : as(n, name, boost::bind(&Reprocess_Server::reprocess, this), false), action_name(name)
    {
        //register the goal and feeback callbacks
        // as.registerGoalCallback(boost::bind(&Reprocess_Server::goalCB, this));
        as.registerPreemptCallback(boost::bind(&Reprocess_Server::preemptCB, this));

        as.start();
    }

    // void goalCB()
    // {
    //     1_target_databasepath = as.acceptNewGoal()->databasepath;
    // }

    void preemptCB()
    {
        ROS_INFO("%s: Preempted", action_name.c_str());
        // set the action state to preempted
        as.setPreempted();
    }

    void reprocess(const cpp::RtabmapReprocess::ConstPtr &goal)
    {
        1_target_databasepath = goal->1_databasepath;
        2_target_databasepath=goal->2_databasepath;

    }
}