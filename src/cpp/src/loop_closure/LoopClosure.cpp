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
        // REVIEW ここのデータベースを消さずにrtabmapのインスタンスを作ると、前回のdetectionの記憶を引き継ぐことができる。
        UFile::erase(database_path);

        rtabmap.setTimeThreshold(700.0f); // Time threshold : 700 ms, 0 ms means no limit

        parameters.insert(rtabmap::ParametersPair(rtabmap::Parameters::kRtabmapLoopThr(), "0.2"));
        parameters.insert(rtabmap::ParametersPair(rtabmap::Parameters::kRGBDEnabled(), "false"));

        // UFile::erase(database_path);
        // REVIEW 何故か強制シャットダウンする。.ros/rtabmap.dbを消すと治った？？
        rtabmap.init(parameters, database_path);

        index_pub = n.advertise<std_msgs::Int32MultiArray>("index_array", 10);
        val_pub = n.advertise<std_msgs::Int32MultiArray>("val_array", 10);
        img_switch = n.advertise<std_msgs::Int32>("loop", 10);

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

        std::string jpg{".jpg"};
        std::string io_num{std::to_string(nextIndex) + jpg};

        // NOTE この時点で、imgの保存が止まっている必要がある
        // REVIEW ここをwhileにすると、confirmCBが動かなくなる.→confirm_numを参照できなくなる？？
        // REVIEW sleepは全ての機能を一時停止にする
        // WARNING CBにwhileを書いてはいけない.forなら耐える
        for (int iteration{0}; iteration < 200; ++iteration)
        {
            if (confirm_num == 1)
                break;
        }

        ROS_INFO("Confirmed");

        data = camera.takeImage();
        int i = 0;

        if (data.imageRaw().empty())
        {
            ROS_ERROR("Images are not stored");
            exit(1);
        }

        for (int iteration{0}; iteration < std::pow(10, 3); ++iteration)
        {
            i += 1;

            if (!data.imageRaw().empty())
            {
                rtabmap.process(data.imageRaw(), nextIndex);

                if (rtabmap.getLoopClosureId())
                {
                    printf(" #%d ptime(%fs) STM(%d) WM(%d) hyp(%d) value(%.2f) *LOOP %d->%d*\n",
                           i,
                           rtabmap.getLastProcessTime(),
                           (int)rtabmap.getSTM().size(), // short-term memory
                           (int)rtabmap.getWM().size(),  // working memory
                           (int)rtabmap.getLoopClosureId(),
                           (float)rtabmap.getLoopClosureValue(),
                           nextIndex,
                           (int)rtabmap.getLoopClosureId());
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

                        clear_dir();

                        io_num = std::to_string(nextIndex) + jpg;

                        send_command();
                    }
                }
                else
                {

                    printf(" #%d ptime(%fs) STM(%d) WM(%d) hyp(%d) value(%.2f)\n",
                           i,
                           rtabmap.getLastProcessTime(),
                           (int)rtabmap.getSTM().size(),     // short-term memory
                           (int)rtabmap.getWM().size(),      // working memory
                           rtabmap.getHighestHypothesisId(), // highest loop closure hypothesis
                           rtabmap.getLoopClosureValue());
                }
                // NOTE loop detect終了

                ++nextIndex;
                data = camera.takeImage();
            }
            else
            {
                break;
            }
            // while (!data.imageRaw().empty())
            // {
        }
        img_switch.publish(msg);
    }

    void confirm_CB(const std_msgs::Int32::ConstPtr &msg)
    {
        ROS_INFO("Hi");
        confirm_num = msg->data;
    }
};

int main(int argc, char **argv)
{

    ros::init(argc, argv, "detection_checker");
    Loop_Closure detector;

    // WARNING classを使う場合、publishするのにmain関数内でwhileループを使うな.
    ros::spin();

    return 0;
}