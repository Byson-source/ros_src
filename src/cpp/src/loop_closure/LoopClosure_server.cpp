//Input the first target_path!!
#include "rtabmap/core/Rtabmap.h"
#include "rtabmap/core/CameraRGB.h"
#include <opencv2/core/core.hpp>
#include "rtabmap/utilite/UFile.h"
#include <stdio.h>
#include "std_msgs/Int32.h"
#include "std_msgs/String.h"
#include <ros/ros.h>
#include <string>
#include <direct.h>
#include <actionlib/server/simple_action_server.h>
#include <cpp/LoopClosure.h>

#define DATABASEPATH "/home/ayumi/Documents/RTAB-MAP/rtabmap.db"

//detector, dir_changer,rest_commander

class Loop_Closure
{
private:
    ros::NodeHandle n;

    ros::Rate loop_rate{0.5};

    actionlib::SimpleActionServer<cpp::LoopClosureAction> as_;
    std::string action_name;

    cpp::LoopClosureFeedback feedback;
    cpp::LoopClosureResult result;

    int i;
    int nextIndex;

    std::string target_path;

    rtabmap::Rtabmap rtabmap;
    rtabmap::ParametersMap parameters;
    rtabmap::CameraImages camera;
    rtabmap::SensorData data;

public:
    Loop_Closure(std::string name) : image_path{path1}, i{0},
                                     database_path{path2}, camera{path1},
                                     as(nh, name, boost::bind(&Loop_Closure::detection, this, _1), false)
    {

        if (!camera.init())
        {
            ROS_INFO("Camera init failed, using path \"%s\"\n", image_path.c_str());
            return 1;
        }
        rtabmap.setTimeThreshold(700.0f); // Time threshold : 700 ms, 0 ms means no limit

        parameters.insert(rtabmap::ParametersPair(rtabmap::Parameters::kRtabmapLoopThr(), rtabmap::Parameters::defaultRtabmapLoopThr()));
        parameters.insert(rtabmap::ParametersPair(rtabmap::Parameters::kRGBDEnabled(), "false"));

        UFile::erase(database_path);

        rtabmap.init(parameters, database_path);

        data = camera.takeImage();
        nextIndex = rtabmap.getLastLocationId() + 1;
        //////////////////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////////
        // as_.registerGoalCallback(boost::bind(&Loop_Closure::goalCB, this));
        as_.registerPreemptCallback(boost::bind(&Loop_Closure::preemptCB, this));

        as_.start();
    }

    // void goalCB(void)
    // {
    //     target_path = as_.acceptNewGoal()->imagepath;

    //     nextindex = 1;

    //     i = 0;

    //     UFile::erase(target_path);

    //     rtabmap.init(parameters, DATABASEPATH);

    //     camera = target_path;

    //     camera.init();

    //     data = camera.takeImage();
    // }

    void preemptCB()
    {
        ROS_INFO("%s: Preempted", action_name.c_str());
        // set the action state to preempted
        as_.setPreempted();
    }

    void detection(const cpp::LoopClosureGoal::ConstPtr &goal)
    {
        // target_path = as_.acceptNewGoal()->imagepath;

        target_path = goal->imagepath;

        nextindex = 1;

        i = 0;

        UFile::erase(target_path);

        rtabmap.init(parameters, DATABASEPATH);

        camera = target_path;

        camera.init();

        data = camera.takeImage();

        while (ros::ok())
        {
            if (data.imageRaw().empty() == 1 || !as_.isActive())
            {
                //when the speed of image extractor is low...
                break;
            }
            else
            {
                rtabmap.process(data.imageRaw(), nextIndex);

                ++i;
                if (rtabmap.getLoopClosureId())
                {

                    ROS_INFO("Loop-closure detected!!");

                    rtabmap.close();

                    ROS_INFO("Rtabmap for next is ready...");
                    result.result = 1;
                    as_.setSucceeded(result);
                    break;
                }
                else
                {
                    //When no loop closure is detected.

                    // ROS_INFO("Nothing happened...");
                    ++nextIndex;
                    data = camera.takeImage();
                }
            }
        }
    }
};

int main()
{

    ros::init(argc, argv, "detection_checker");
    Loop_Closure detector("loop_closure");

    return 0;
}
