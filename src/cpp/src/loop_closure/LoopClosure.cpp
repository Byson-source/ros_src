#include "rtabmap/core/Rtabmap.h"
#include "rtabmap/core/CameraRGB.h"
#include <opencv2/core/core.hpp>
#include "rtabmap/utilite/UFile.h"
#include "rtabmap/utilite/UDirectory.h"
#include <stdio.h>
#include "std_msgs/Int32.h"
#include "std_msgs/String.h"
#include "std_msgs/Int32MultiArray.h"
#include <ros/ros.h>
#include <string>
#include <vector>
#include <unistd.h>

#define DATABASEPATH "/home/ayumi/Documents/RTAB-Map/rtabmap.db"

//detector, dir_changer,rest_commander

class Loop_Closure
{
private:
    ros::NodeHandle n;

    int dir_number;

    std::vector<int> detection_list;

    int robot_number;

    std::string template_path{"/home/ayumi/Documents/tohoku_uni/CLOVERs/images/"};
    std::string template_path2{"_rgb"};

    ros::Rate loop_rate{0.5};

    std::string database_path{DATABASEPATH};

    std::vector<int> detect_index;
    std::vector<int> detect_val;
    std::map<int, int> who_detect;

    rtabmap::Rtabmap rtabmap;
    rtabmap::ParametersMap parameters;
    rtabmap::SensorData data;

    ros::Subscriber switch_sub;
    ros::Publisher index_pub;
    ros::Publisher val_pub;
    int nextIndex{1};

public:
    Loop_Closure(int num) : status(1), dir_number(1), robot_number(num)
    {
        rtabmap.setTimeThreshold(700.0f); // Time threshold : 700 ms, 0 ms means no limit

        parameters.insert(rtabmap::ParametersPair(rtabmap::Parameters::kRtabmapLoopThr(), "0.2"));
        parameters.insert(rtabmap::ParametersPair(rtabmap::Parameters::kRGBDEnabled(), "false"));

        rtabmap.init(parameters, DATABASEPATH);

        switch_sub = n.subscribe("imagepath_change", 10, &Loop_Closure::switchCB, this);

        index_pub = n.advertise<std_msgs::Int32MultiArray>("index_array", 10);
        val_pub = n.advertise<std_msgs::Int32MultiArray>("val_array", 10);

        UFile::erase(database_path);

        who_detect[-1] = 0;
        who_detect[0] = 0;
        // NOTE key -1は何回連続でloopが続いているか
        // NOTE key 0 はどのロボット（インデックス）がloopを感知し続けているのか？？
    }

    void switchCB(const std_msgs::Int32::ConstPtr &msg)
    {
        if (dir_number == 1)
            dir_number = 2;
        else
            dir_number = 1;

        Loop_Closure::detection();
    }

    void send_command(int threshold = 0)
    {
        if (threshold != 0)
        {
            if (who_detect.size() > threshold + 2)
            {
                for (auto [key, val] : who_detect)
                {
                    if ((key != -1) && (key != 0))
                    {
                        detect_index.push_back(key);
                        detect_val.push_back(val);
                    }
                }
            }
        }
        else
        {
            for (auto [key, val] : who_detect)
            {
                if ((key != -1) && (key != 0))
                {
                    detect_index.push_back(key);
                    detect_val.push_back(val);
                }
            }
        }

        std_msgs::Int32MultiArray index_array;
        std_msgs::Int32MultiArray val_array;
        // NOTE Ignore keys -1 and 0
        index_array.data.resize(who_detect.size() - 2);
        val_array.data.resize(who_detect.size() - 2);

        for (int iter{0}, iter < who_detect.size() - 2; ++iter)
        {
            index_array.data[iter] = detect_index[iter];
            val_array.data[iter] = detect_val[iter];
        }

        index_pub.publish(index_array);
        val_array.publish(val_array);
    }

    void detection(void)
    {
        rtabmap::CameraImages camera(template_path + std::to_string(dir_number) + template_path2);

        camera.init();

        std::string jpg{".jpg"};
        std::string io_num{std::to_string(nextIndex) + jpg};

        for (int iter{0}; iter < robot_number; ++iter)
        {

            if (UFile::exists(target_path + io_num))
            {
                data = camera.takeImage();
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
                    ++nextIndex;
                    io_num = std::to_string(nextIndex) + jpg;

                    if ((nextIndex - rtabmap.getLoopClosureId()) % 2 == 1)
                    {
                        // NOTE This system only considers only one of them detects where the other passes.

                        if ((who_detect[-1] > 0) && (iter == who_detect[0]))
                        {
                            who_detect[nextIndex] = rtabmap.getLoopClosureId();
                        }

                        ROS_ERROR("!!!!!!!!!!!!!!!!!!!!!!!!!");
                    }
                    // NOTE もし,loopが五回以上検出されたら

                    send_command(5);
                }
                else
                {
                    ++nextIndex;
                    io_num = std::to_string(nextIndex) + jpg;

                    if (who_detect.size() > 4)
                        send_command();
                    who_detect.clear();
                    who_detect[-1] = 0;
                    who_detect[0] = 0;
                }
            }
            else
            {
                std::cout << "Images are not stored yet..." << std::endl;
                who_detect.clear();
                who_detect[-1] = 0;
                who_detect[0] = 0;
            }
        }
    }
};

int main(int argc, char **argv)
{

    ros::init(argc, argv, "detection_checker");

    ros::NodeHandle pnh("~");
    int number;
    pnh.getParam("robot_num", number);

    Loop_Closure detector(number);

    ros::spin();

    return 0;
}
