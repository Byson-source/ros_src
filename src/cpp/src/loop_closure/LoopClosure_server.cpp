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
#include <actionlib/server/simple_action_server.h>
#include <cpp/LoopClosureAction.h>

#define DATABASEPATH "/home/ayumi/Documents/RTAB-MAP/rtabmap.db"

//detector, dir_changer,rest_commander

class Loop_Closure
{
private:
    ros::NodeHandle n;

    ros::Rate loop_rate{0.5};

    actionlib::SimpleActionServer<cpp::LoopClosureAction> as_;
    std::string action_name;

    cpp::LoopClosureResult result;

    std::string database_path{DATABASEPATH};

    rtabmap::Rtabmap rtabmap;
    rtabmap::ParametersMap parameters;

public:
    Loop_Closure(std::string name) : as_(n, name, boost::bind(&Loop_Closure::detection, this, _1), false),
    action_name{name}
    {
        rtabmap.setTimeThreshold(700.0f); // Time threshold : 700 ms, 0 ms means no limit

        parameters.insert(rtabmap::ParametersPair(rtabmap::Parameters::kRtabmapLoopThr(),"0.11"));
        parameters.insert(rtabmap::ParametersPair(rtabmap::Parameters::kRGBDEnabled(), "false"));

        UFile::erase(database_path);
        

        as_.start();
    }

    void detection(const cpp::LoopClosureGoal::ConstPtr &goal)
    {
        // target_path = as_.acceptNewGoal()->imagepath;
        std::string target_path{goal->imagepath};

        rtabmap::CameraImages camera(target_path);
        rtabmap::SensorData data{camera.takeImage()};

        int i{0};
        int nextIndex{1};

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

int main(int argc, char **argv)
{

    ros::init(argc, argv, "detection_checker");
    Loop_Closure detector("loop_closure");

    return 0;
}
