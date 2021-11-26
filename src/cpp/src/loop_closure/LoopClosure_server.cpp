//Input the first target_path!!
#include "rtabmap/core/Rtabmap.h"
#include "rtabmap/core/CameraRGB.h"
#include <opencv2/core/core.hpp>
#include "rtabmap/utilite/UFile.h"
#include "rtabmap/utilite/UDirectory.h"
#include <stdio.h>
#include "std_msgs/Int32.h"
#include "std_msgs/String.h"
#include <ros/ros.h>
#include <string>
#include <actionlib/server/simple_action_server.h>
#include <cpp/LoopClosureAction.h>
#include <vector>
#include <unistd.h>

#define DATABASEPATH "/home/ayumi/Documents/RTAB-Map/rtabmap.db"

//detector, dir_changer,rest_commander

class Loop_Closure
{
private:
    ros::NodeHandle n;

    std::vector<int> detection_list;

    int state{0};

    ros::Rate loop_rate{0.5};

    actionlib::SimpleActionServer<cpp::LoopClosureAction> as_;

    cpp::LoopClosureResult result;
    cpp::LoopClosureFeedback feedback;

    std::string database_path{DATABASEPATH};

    int stage{1};

    rtabmap::Rtabmap rtabmap;
    rtabmap::ParametersMap parameters;

public:
    Loop_Closure(std::string name) : as_(n, name, boost::bind(&Loop_Closure::detection, this, _1), false)
    {
        rtabmap.setTimeThreshold(700.0f); // Time threshold : 700 ms, 0 ms means no limit

        parameters.insert(rtabmap::ParametersPair(rtabmap::Parameters::kRtabmapLoopThr(), "0.2"));
        parameters.insert(rtabmap::ParametersPair(rtabmap::Parameters::kRGBDEnabled(), "false"));

        UFile::erase(database_path);

        as_.start();
    }

    void detection(const cpp::LoopClosureGoal::ConstPtr &goal)
    {
        // target_path = as_.acceptNewGoal()->imagepath;
        feedback.stage = state;

        std::string target_path{goal->imagepath};

        sleep(30);
        rtabmap::CameraImages camera(target_path);
        rtabmap::SensorData data;

        int nextIndex{1};

        UFile::erase(target_path);
        UDirectory::makeDir(target_path);

        rtabmap.init(parameters, DATABASEPATH);

        camera.init();

        std::string jpg{".jpg"};
        std::string io_num{std::to_string(nextIndex) + jpg};
        while (ros::ok())
        {
            // sleep(0.7);
            // if (data.imageRaw().empty()){
            //     std::cout<<"There is still something wrong..."<<std::endl;
            // }
            if (UFile::exists(target_path+io_num))
            {
                data=camera.takeImage();
                std::cout<<target_path+io_num<<std::endl;
                std::cout<<data.imageRaw().empty()<<std::endl;
                rtabmap.process(data.imageRaw(), nextIndex);

                if (rtabmap.getLoopClosureId())
                {
                    printf("time(%fs) STM(%d) WM(%d) hyp(%d) value(%.2f) *LOOP %d->%d*\n",
                           rtabmap.getLastProcessTime(),
                           (int)rtabmap.getSTM().size(), // short-term memory
                           (int)rtabmap.getWM().size(),  // working memory
                           rtabmap.getLoopClosureId(),
                           rtabmap.getLoopClosureValue(),
                           nextIndex,
                           rtabmap.getLoopClosureId());
                    if ((nextIndex - rtabmap.getLoopClosureId()) % 2 == 1)
                    {
                        state += 1;
                        ROS_ERROR("!!!!!!!!!!!!!!!!!!!!!!!!!");
                        ROS_ERROR_STREAM(state);
                        ROS_ERROR("!!!!!!!!!!!!!!!!!!!!!!!!!");
                        if (state == 100)
                        {
                            feedback.stage = 0;
                            state = 0;
                            as_.publishFeedback(feedback);

                            ROS_INFO("Loop-closure detected!!");

                            rtabmap.close();

                            ROS_INFO("Rtabmap for next is ready...");
                            result.result = 1;
                            as_.setSucceeded(result);

                            break;
                        }
                    }
                    ++nextIndex;
                    io_num=std::to_string(nextIndex) + jpg;
                }else
                {
                    as_.publishFeedback(feedback);
                    //When no loop closure is detected.
                    // ROS_ERROR("Nothing happened...");
                    ++nextIndex;
                    io_num=std::to_string(nextIndex) + jpg;

                    // if (UFile::exists(target_path + io_num))
                    // {
                    //     data = camera.takeImage();
                    // }
                    // else
                    // {
                    //     std::cout << "No file!!" << std::endl;
                    //     std::cout << "Wait for image to come..." << std::endl;
                    //     sleep(1);
                    //     std::cout << UFile::exists(target_path + io_num) << std::endl;
                    //     data = camera.takeImage();
                    // }
                }
            }else{
                std::cout<<"I'm waiting..."<<std::endl;
                sleep(1);
            }
        }
    }
};

int main(int argc, char **argv)
{

    ros::init(argc, argv, "detection_checker");

    Loop_Closure detector("loop_closure");

    ros::spin();

    return 0;
}
