#include <ros/ros.h>
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

  ros::Subscriber random_sub;

  // boost::thread spin_thread(&Average_Client::spinThread());

  float status{0.0};

  std::string action_name;

  float previous_status{0.0};

  ros::Time t_start{ros::Time::now()};

public:
  Average_Client(std::string name) : action_name(name)
  {
    ROS_INFO("Now launching...");

    random_sub = n.subscribe("/random_number", 1, &Average_Client::randomCB, this);

    switch_sub = n.subscribe("/switch_number", 1, &Average_Client::switchCB, this);
  }

  void switchCB(const::std_msgs::Float32::ConstPtr &msg){
    status=msg->data;
  }

  void randomCB(const std_msgs::Float32::ConstPtr &msg)
  {
    if (status == 1.0 && !previous_status == status)
    {
      previous_status=1;

      actionlib::SimpleActionClient<cpp::AveragingAction> ac(action_name,true);

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
    }else{
      previous_status = status;

    }
    
  }

  // void spinThread()
  // {
  //   ros::spin();
  // }
};
int main(int argc, char **argv)
{
  ros::init(argc, argv, "test_averaging");
  Average_Client test("averaging");

  ros::spin();

  return 0;
}