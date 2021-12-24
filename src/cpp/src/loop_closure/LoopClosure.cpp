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

//detector, dir_changer,rest_commander

class Loop_Closure
{
private:
    ros::NodeHandle n;

    std::vector<int> detection_list;

    int robot_number;
    int confirm_num{0};
    int nextIndex{1};

    // NOTE 二秒ごとにimageをチェックする。
    ros::Rate loop_rate{0.5};

    std::string template_path{"/home/ayumi/Documents/tohoku_uni/CLOVERs/images/rgb/"};
    std::string database_path{"/home/ayumi/Documents/RTAB-Map/rtabmap.db"};

    std::map<int, std::map<std::string, std::vector<int>>> who_detect;
    // NOTE {int robotID;{"index":std::vector<int>,"LoopID":std::vector<int>}} このindexには連続するものしか入らない
    std::map<int, std::map<std::string, std::vector<int>>> all_loop;
    // NOTE {int robotID;{"index":std::vector<int>,"LoopID":std::vector<int>}} このindexにはロボット間の検知だったら全て格納する

    rtabmap::Rtabmap rtabmap;
    rtabmap::ParametersMap parameters;
    rtabmap::SensorData data;

    ros::Publisher index_pub;
    ros::Publisher val_pub;
    ros::Publisher img_switch;

    ros::Subscriber switch_sub;
    ros::Subscriber confirm;

public:
    Loop_Closure() : robot_number(2)
    {
        rtabmap.setTimeThreshold(700.0f); // Time threshold : 700 ms, 0 ms means no limit

        parameters.insert(rtabmap::ParametersPair(rtabmap::Parameters::kRtabmapLoopThr(), "0.2"));
        parameters.insert(rtabmap::ParametersPair(rtabmap::Parameters::kRGBDEnabled(), "false"));

        // UFile::erase(database_path);
        // NOTE 何故か強制シャットダウンする。.ros/rtabmap.dbを消すと治った？？
        rtabmap.init(parameters, database_path);

        index_pub = n.advertise<std_msgs::Int32MultiArray>("index_array", 10);
        val_pub = n.advertise<std_msgs::Int32MultiArray>("val_array", 10);
        img_switch = n.advertise<std_msgs::Int32>("loop", 10);

        switch_sub = n.subscribe("command", 1000, &Loop_Closure::detection, this);
        confirm = n.subscribe("stop_storing", 1000, &Loop_Closure::confirmCB, this);

        std::map<std::string, std::vector<int>> R1_info;
        std::map<std::string, std::vector<int>> R2_info;

        who_detect[1] = R1_info;
        who_detect[2] = R2_info;

        std::map<std::string, std::vector<int>> R1_loop;
        std::map<std::string, std::vector<int>> R2_loop;

        all_loop[1] = R1_loop;
        all_loop[2] = R2_loop;
    }

    void send_command(int threshold = 0)
    {
        for (auto [key, val] : who_detect)
        {
            std_msgs::Int32MultiArray index_array;
            // NOTE　最初のインデックスに1とあったらR1のdetection、2とあったらR2のもの
            std_msgs::Int32MultiArray val_array;
            // NOTE　最初のインデックスに1とあったらR1のdetection、2とあったらR2のもの

            index_array.data.resize(val["index"].size() + 1);
            val_array.data.resize(val["index"].size() + 1);

            for (int iter{0}; iter < val["index"].size() + 1; ++iter)
            {
                if (iter == 0)
                {
                    index_array.data[iter] = key;
                    val_array.data[iter] = key;
                }
                else
                {
                    index_array.data[iter] = val["index"][iter - 1];
                    val_array.data[iter] = val["LoopID"][iter - 1];
                }
            }

            index_pub.publish(index_array);
            val_pub.publish(val_array);
        }
    }

    void clear_dir(void)
    {
        who_detect[1]["index"].clear();
        who_detect[2]["index"].clear();
        who_detect[1]["LoopID"].clear();
        who_detect[2]["LoopID"].clear();

        all_loop[1]["index"].clear();
        all_loop[2]["index"].clear();
        all_loop[1]["LoopID"].clear();
        all_loop[2]["LoopID"].clear();
    }
    // FIXME main関数内でwhileループを使うな
    void detection(const std_msgs::Int32::ConstPtr &turn_on)
    {

        std_msgs::Int32 msg;

        msg.data = 1;

        // NOTE Loop Detect開始(msgの内容はどうでもいい)
        img_switch.publish(msg);

        rtabmap::CameraImages camera(template_path);
        // NOTE ディレクトリができるのを待つ必要がある.
        if (!camera.init())
        {
            ROS_ERROR("Camera init failed!");
            exit(1);
        }

        ROS_ERROR("CHECK!!!");
        std::string jpg{".jpg"};
        std::string io_num{std::to_string(nextIndex) + jpg};

        // NOTE この時点で、imgの保存が止まっている必要がある
        for (int wait{0}; wait < std::pow(10, 9); ++wait)
        {
            if (wait == std::pow(10, 9) - 1)
            {
                ROS_ERROR("Can't wait anymore!!");
                exit(1);
            }
            else if (confirm_num == 1)
                break;
        }
        data = camera.takeImage();

        for (int wait{0}; wait < std::pow(10, 9); ++wait)
        {

            if (UFile::exists(template_path + io_num))
            {
                rtabmap.process(data.imageRaw(), nextIndex);
            }

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
                    if (nextIndex % 2 == 1)
                    {
                        // NOTE R1が検知
                        // NOTE loopが連続しているか否か
                        if ((all_loop[1]["index"].size() > 0) && (all_loop[1]["index"].back() == nextIndex - 1))
                        {
                            who_detect[1]["index"].push_back(nextIndex);
                            who_detect[1]["LoopID"].push_back(rtabmap.getLoopClosureId());
                        }
                    }

                    else
                    // NOTE R2が検知
                    {
                        if ((all_loop[2]["index"].size() > 0) && (all_loop[2]["index"].back() == nextIndex - 1))
                        {
                            who_detect[2]["index"].push_back(nextIndex);
                            who_detect[2]["LoopID"].push_back(rtabmap.getLoopClosureId());
                        }

                        all_loop[2]["index"].push_back(nextIndex);
                        all_loop[2]["LoopID"].push_back(rtabmap.getLoopClosureId());
                    }

                    ROS_ERROR("!!!!!!!!!!!!!!!!!!!!!!!!!");

                    send_command(5);
                }
                else
                // NOTE loopが検出されなくなった瞬間
                {
                    ++nextIndex;

                    clear_dir();

                    io_num = std::to_string(nextIndex) + jpg;

                    send_command();
                }

                data = camera.takeImage();
            }
            else
            {
                ROS_ERROR("Images are not stored yet...");

                exit(1);
            }
            // NOTE loop detect終了
            img_switch.publish(msg);
        }
        // while (!data.imageRaw().empty())
        // {
        ros::spinOnce();
        loop_rate.sleep();
    }

    void confirmCB(const std_msgs::Int32::ConstPtr &msg)
    {
        confirm_num = 1;
    }
};

int main(int argc, char **argv)
{

    ros::init(argc, argv, "detection_checker");

    Loop_Closure detector;
    
    ros::spin();

    return 0;
}
