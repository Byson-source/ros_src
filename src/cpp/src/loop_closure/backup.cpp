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
    int iter{0};

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

    ros::Publisher img_switch;
    ros::Publisher val_pub;
    ros::Publisher index_pub;

    ros::Subscriber switch_sub;
    ros::Subscriber confirm;

public:
    Loop_Closure() : robot_number(2)
    {
        rtabmap.setTimeThreshold(700.0f); // Time threshold : 700 ms, 0 ms means no limit

        parameters.insert(rtabmap::ParametersPair(rtabmap::Parameters::kRtabmapLoopThr(), "0.2"));
        parameters.insert(rtabmap::ParametersPair(rtabmap::Parameters::kRGBDEnabled(), "false"));

        // UFile::erase(database_path);
        // REVIEW 何故か強制シャットダウンする。.ros/rtabmap.dbを消すと治った？？
        rtabmap.init(parameters, database_path);

        img_switch = n.advertise<std_msgs::Int32>("loop", 10);
        index_pub = n.advertise<std_msgs::Int32MultiArray>("index_array", 10);
        val_pub = n.advertise<std_msgs::Int32MultiArray>("val_array", 10);

        // FIXME
        confirm = n.subscribe("stop_storing", 1000, &Loop_Closure::confirm_CB, this);
        switch_sub = n.subscribe("command", 1000, &Loop_Closure::detection, this);

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
        for (auto item : who_detect)
        {
            std_msgs::Int32MultiArray index_array;
            // NOTE　最初のインデックスに1とあったらR1のdetection、2とあったらR2のもの
            std_msgs::Int32MultiArray val_array;
            // NOTE　最初のインデックスに1とあったらR1のdetection、2とあったらR2のもの

            index_array.data.resize(item.second["index"].size() + 1);
            val_array.data.resize(item.second["index"].size() + 1);

            for (int iter{0}; iter < item.second["index"].size() + 1; ++iter)
            {
                if (iter == 0)
                {
                    index_array.data[iter] = item.first;
                    val_array.data[iter] = item.first;
                }
                else
                {
                    index_array.data[iter] = item.second["index"][iter - 1];
                    val_array.data[iter] = item.second["LoopID"][iter - 1];
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

    void detection(const std_msgs::Int32::ConstPtr &turn_on)
    {
        iter += 1;

        std_msgs::Int32 msg;

        msg.data = 1;

        // NOTE Loop Detect開始(msgの内容はどうでもいい)
        if (iter == 1)
        {

            img_switch.publish(msg);
        }

        rtabmap::CameraImages camera(template_path);
        // NOTE ディレクトリができるのを待つ必要がある.
        if (!camera.init())
        {
            ROS_ERROR("Camera init failed!");
            exit(1);
        }

        std::string jpg{".jpg"};
        std::string io_num{std::to_string(nextIndex) + jpg};
        int iter{0};
        // REVIEW ここをwhileにすると、confirmCBが動かなくなる.→confirm_numを参照できなくなる？？
        // REVIEW sleepは全ての機能を一時停止にする
        // WARNING CBにwhileを書いてはいけない.forなら耐える
        for (int i{0};i<100;++i)
        {
            if (confirm_num == 1)
                break;
            ROS_ERROR("HEY");
        }

        // NOTE この時点で、imgの保存が止まっている必要がある
    }
    // FIXME
    void confirm_CB(const std_msgs::String::ConstPtr &msg)
    {
        ROS_ERROR("Hi");
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