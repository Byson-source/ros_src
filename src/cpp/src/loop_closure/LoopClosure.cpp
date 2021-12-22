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

    std::vector<int> detection_list;

    int robot_number;

    std::string template_path{"/home/ayumi/Documents/tohoku_uni/CLOVERs/images/rgb/"};

    // NOTE 二秒ごとにimageをチェックする。
    ros::Rate loop_rate{0.5};

    std::string database_path{DATABASEPATH};

    //FIXME who_detectを辞書化
    std::map < int, std::map<std::string, std::vector<int>> who_detect;
    // NOTE {int robotID;{"index":std::vector<int>,"LoopID":std::vector<int>}} このindexには連続するものしか入らない

    rtabmap::Rtabmap rtabmap;
    rtabmap::ParametersMap parameters;
    rtabmap::SensorData data;

    ros::Publisher index_pub;
    ros::Publisher val_pub;
    ros::Publisher img_switch;
    int nextIndex{1};

public:
    Loop_Closure(int num) : robot_number(num)
    {
        rtabmap.setTimeThreshold(700.0f); // Time threshold : 700 ms, 0 ms means no limit

        parameters.insert(rtabmap::ParametersPair(rtabmap::Parameters::kRtabmapLoopThr(), "0.2"));
        parameters.insert(rtabmap::ParametersPair(rtabmap::Parameters::kRGBDEnabled(), "false"));

        rtabmap.init(parameters, DATABASEPATH);

        img_switch = n.advertise<std_msgs::Int32>("loop", 10);
        index_pub = n.advertise<std_msgs::Int32MultiArray>("index_array", 10);
        val_pub = n.advertise<std_msgs::Int32MultiArray>("val_array", 10);

        UFile::erase(database_path);

        std::map<std::string, std::vector<int>> R1_info;
        std::map<std::string, std::vector<int>> R2_info;

        who_detect["R1"] = R1_info;
        who_detect["R2"] = R2_info;
        // NOTE key -1は何回連続でloopが続いているか
        // NOTE key 0 はどのロボット（インデックス）がloopを感知し続けているのか？？
    }

    void send_command(int threshold = 0)
    {
        for (auto [key, val]; who_detect)
        {
            std_msgs::Int32MultiArray index_array{[key]};
            // NOTE　最初のインデックスに1とあったらR1のdetection、2とあったらR2のもの
            std_msgs::Int32MultiArray val_array{[key]};
            // NOTE　最初のインデックスに1とあったらR1のdetection、2とあったらR2のもの

            index_array.data.resize(val["index"].size() + 1);
            val_array.data.resize(val["index"].size() + 1);

            for (int iter{0}; iter < who_detect.size() - 2; ++iter)
            {
                if (iter == 0)
                {
                    index_array.data[iter] = key;
                    val_array.data[iter] = key;
                }
                else
                {
                    index_array.data[iter] = detect_index[iter];
                    val_array.data[iter] = detect_val[iter];
                }
            }

            index_pub.publish(index_array);
            val_pub.publish(val_array);
        }
    }

    void detection(void)
    {

        std_msgs::Int32 msg;
        msg.data = 1;

        img_switch.publish(msg);
        // NOTE Loop Detect開始(msgの内容はどうでもいい)

        rtabmap::CameraImages camera(template_path);

        camera.init();

        std::string jpg{".jpg"};
        std::string io_num{std::to_string(nextIndex) + jpg};

        data = camera.takeImage();
        // NOTE この時点で、imgの保存が止まっている必要がある
        while (!data.imageRaw().empty())
        {
            if (UFile::exists(template_path + io_num))
            {
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

                    send_command(5);
                }
                else
                // NOTE loopが検出されなくなった瞬間
                {
                    ++nextIndex;
                    io_num = std::to_string(nextIndex) + jpg;

                    send_command();
                    who_detect.clear();
                    who_detect[-1] = 0;
                    who_detect[0] = 0;
                }

                data = camera.takeImage();
            }
            else
            {
                std::cout << "Images are not stored yet..." << std::endl;
                who_detect.clear();
                who_detect[-1] = 0;
                who_detect[0] = 0;
            }
        }
        // NOTE loop detect終了
        img_switch.publish(msg);
        ros::spinOnce();
        loop_rate.sleep();
    }
};

int main(int argc, char **argv)
{

    ros::init(argc, argv, "detection_checker");

    ros::NodeHandle pnh("~");
    int number;
    pnh.getParam("robot_num", number);

    Loop_Closure detector(number);

    while (ros::ok())
    {
        detector.detection();
    }

    return 0;
}
