#include <ros/ros.h>
#include <std_msgs/String.h>
#include <std_msgs/Float32.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include <cpp/AveragingAction.h>
#include <boost/thread.hpp>
#include <string>
#include <iostream>
#include <stdio.h>

class Average_Client
{
private:
  ros::NodeHandle n;

  ros::Subscriber switch_sub;

  ros::Publisher pub;

  // boost::thread spin_thread(&Average_Client::spinThread());

  actionlib::SimpleActionClient<cpp::AveragingAction> ac;

  float status{0.0};

  std::string action_name;

  float previous_status{0.0};

  ros::Time t_start{ros::Time::now()};

public:
  Average_Client(std::string name) : action_name(name),ac(name, true)
  {
    ROS_INFO("Now launching...");

    pub = n.advertise<std_msgs::String>("test", 10);
    switch_sub = n.subscribe("/switch_number", 100, &Average_Client::switchCB, this);
  }

  void switchCB(const std_msgs::Float32::ConstPtr &msg)
  {
    std_msgs::String datada;
    datada.data = "Yes";
    status = msg->data;
    pub.publish(datada);
    // std::cout<<"status: "<<status<<"previous_status: "<<status<<std::endl;
    if (status == 1.0 && !previous_status == status)
    // if (status == 1.0)
    {
      previous_status = 1;

      bool finished_before_timeout = ac.waitForResult(ros::Duration(30.0));

      ROS_INFO("Waiting for action server to start.");
      ac.waitForServer();

      ROS_INFO("Action server started, sending goal.");
      // send a goal to the action
      cpp::AveragingGoal goal;
      goal.samples = 100;
      ac.sendGoal(goal);
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
  ros::init(argc, argv, "test_averaging");
  Average_Client test("averaging");

  ros::spin();

  return 0;
}