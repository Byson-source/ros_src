// FIXME loopが連続すると"つられ"現象が起こる。True positiveが異なるロボット間で起きた→つられ現象によりFPになった場合、反芻作業が必要？
// FIXME TPとFPのふるいわけ
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
#include <sys/types.h>
#include <dirent.h>
#include <cstdio>

//detector, dir_changer,rest_commander

class Loop_Closure
{
private:
    ros::NodeHandle n;

    std::vector<int> detection_list;

    int robot_number;
    int nextIndex{1};
    int criteria_R1{0};
    int criteria_R2{0};
    // hypothesis criteria

    std::string template_path{"/home/ayumi/Documents/tohoku_uni/CLOVERs/images/rgb/"};
    std::string database_path{"/home/ayumi/Documents/RTAB-Map/rtabmap.db"};
    std::string sorted_path{"/home/ayumi/Documents/tohoku_uni/CLOVERs/images/sorted_rgb/"};

    std::map<int, std::map<std::string, std::vector<int>>> who_detect;
    // NOTE {int robotID;{"index":std::vector<int>,"LoopID":std::vector<int>}} このindexには連続するものしか入らない
    std::map<int, std::map<std::string, std::vector<int>>> all_loop;
    // NOTE {int robotID;{"index":std::vector<int>,"LoopID":std::vector<int>}} このindexにはロボット間の検知だったら全て格納する
    std::map<int, int> sorted_original;

    rtabmap::Rtabmap rtabmap;
    rtabmap::ParametersMap parameters;
    rtabmap::SensorData data;

    ros::Publisher index_pub;
    ros::Publisher val_pub;
    ros::Publisher img_switch;

    ros::Subscriber confirm;

public:
    Loop_Closure() : robot_number(2)
    {
        // REVIEW ここのデータベースを消さずにrtabmapのインスタンスを作ると、前回のdetectionの記憶を引き継ぐことができる。
        UFile::erase(database_path);

        rtabmap.setTimeThreshold(700.0f); // Time threshold : 700 ms, 0 ms means no limit

        parameters.insert(rtabmap::ParametersPair(rtabmap::Parameters::kRtabmapLoopThr(), "0.2"));
        parameters.insert(rtabmap::ParametersPair(rtabmap::Parameters::kRGBDEnabled(), "false"));

        // REVIEW rtabmapのstmの決め方
        parameters.insert(rtabmap::ParametersPair(rtabmap::Parameters::kMemSTMSize(), "40"));

        // UFile::erase(database_path);
        // REVIEW 何故か強制シャットダウンする。.ros/rtabmap.dbを消すと治った？？
        rtabmap.init(parameters, database_path);

        index_pub = n.advertise<std_msgs::Int32MultiArray>("index_array", 10);
        val_pub = n.advertise<std_msgs::Int32MultiArray>("val_array", 10);
        // image node turn ON
        img_switch = n.advertise<std_msgs::Int32>("loop", 10);

        // loop起動
        confirm = n.subscribe("loop_start", 1000, &Loop_Closure::confirm_CB, this);

        std::map<std::string, std::vector<int>> R1_info;
        std::map<std::string, std::vector<int>> R2_info;

        who_detect[1] = R1_info;
        who_detect[2] = R2_info;

        std::map<std::string, std::vector<int>> R1_loop;
        std::map<std::string, std::vector<int>> R2_loop;

        all_loop[1] = R1_loop;
        all_loop[1]["index"].push_back(-1);
        all_loop[2] = R2_loop;
        all_loop[2]["index"].push_back(-1);
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

    int count_files(void)
    {
        // REVIEW ディレクトリの中のファイルの数え方
        DIR *dp;
        int file_number = 0;
        struct dirent *ep;
        dp = opendir(template_path.c_str());

        if (dp != NULL)
        {
            while (ep = readdir(dp))
                file_number++;

            (void)closedir(dp);
        }
        else
        {
            perror("Couldn't open the directory");
            exit(1);
        }
        // NOTE なぜ引く必要があるかはわからない
        return file_number - 2;
    }

    std::map<std::string, int> rename(int file_num, std::string jpg)
    {
        std::map<std::string, std::vector<int>> file_dir;
        for (int iteration = 0; iteration < file_num; ++iteration)
        {
            if ((nextIndex + iteration) % 2 == 1)
                file_dir["R1"].push_back(nextIndex + iteration);
            else
                file_dir["R2"].push_back(nextIndex + iteration);
            std::string old_name{template_path + std::to_string(nextIndex + iteration) + jpg};
            std::string new_name{template_path + std::to_string(nextIndex + iteration + 50) + jpg};
            std::rename(old_name.c_str(), new_name.c_str());
        }
        // NOTE まず、R1を先にソートする
        for (int iteration = 0; iteration < file_dir["R1"].size(); ++iteration)
        {
            std::string old_name{template_path + std::to_string(file_dir["R1"][iteration] + 50) + jpg};
            std::string old_name2{template_path + std::to_string(file_dir["R1"][iteration]) + jpg};
            std::string new_name{template_path + std::to_string(nextIndex + iteration) + jpg};

            sorted_original[nextIndex + iteration] = file_dir["R1"][iteration];
            std::rename(old_name.c_str(), new_name.c_str());

            cv::Mat srcImage{cv::imread(new_name)};
            std::string folder_name{"sorted_rgb/"};
            try
            {
                cv::imwrite(sorted_path + std::to_string(nextIndex + iteration) + jpg, srcImage);
            }
            catch (cv::Exception)
            {
            }

            std::cout << "For R1, old name; " << std::to_string(file_dir["R1"][iteration]) << " to new name; " << std::to_string(nextIndex + iteration) << std::endl;
        }
        // R2
        for (int iteration = 0; iteration < file_dir["R2"].size(); ++iteration)
        {
            std::string old_name{template_path + std::to_string(file_dir["R2"][iteration] + 50) + jpg};
            std::string old_name2{template_path + std::to_string(file_dir["R2"][iteration]) + jpg};
            std::string new_name{template_path + std::to_string(nextIndex + iteration + file_dir["R1"].size()) + jpg};
            sorted_original[nextIndex + iteration + file_dir["R1"].size()] = file_dir["R2"][iteration];
            std::rename(old_name.c_str(), new_name.c_str());

            cv::Mat srcImage{cv::imread(new_name)};
            std::string folder_name{"sorted_rgb/"};
            try
            {

                cv::imwrite(sorted_path + std::to_string(nextIndex + iteration + file_dir["R1"].size()) + jpg, srcImage);
            }
            catch (cv::Exception)
            {
            }

            std::cout << "For R2, old name; " << std::to_string(file_dir["R2"][iteration]) << " to new name; " << std::to_string(nextIndex + iteration + file_dir["R1"].size()) << std::endl;
        }

        std::map<std::string, int> ans;
        ans["R1start"] = nextIndex;
        ans["R1end"] = nextIndex + file_dir["R1"].size() - 1;
        ans["R2start"] = nextIndex + file_dir["R1"].size();
        ans["R2end"] = nextIndex + file_num - 1;

        return ans;
    }

    void clear_dir(void)
    {
        who_detect[1]["index"].clear();
        who_detect[2]["index"].clear();
        who_detect[1]["LoopID"].clear();
        who_detect[2]["LoopID"].clear();
    }

    // FIXME 要修正？
    int judge(int hypothesis, std::map<std::string, int> detect_info)
    {
        std::string index_belong;

        if (detect_info["R1start"] <= nextIndex && nextIndex <= detect_info["R1end"])
            index_belong = "R1";
        else
            index_belong = "R2";
        // NOTE 塊の始まりの検知
        // 初回

        if ((all_loop[1]["index"].size() == 1) && (all_loop[2]["index"].size() == 1))
        {
            if (index_belong == "R1")
                criteria_R1 = hypothesis;
            else
                criteria_R2 = hypothesis;
        }
        // 別の塊
        else if ((nextIndex - std::max(all_loop[1]["index"].back(), all_loop[2]["index"].back()) > 3))
        {
            // ROS_INFO("Now judging...");
            if (index_belong == "R1")
                criteria_R1 = hypothesis;
            else
                criteria_R2 = hypothesis;
        }
        // criteriaは真値のhypothesisと期待できるもの
        // loopが連続している時
        else if (nextIndex - std::max(all_loop[1]["index"].back(), all_loop[2]["index"].back()) <= 3)
        {
            if ((((index_belong == "R2")) && ((criteria_R1 - 5 <= hypothesis) && (criteria_R1 + 5 >= hypothesis))) ||
                (((index_belong == "R1")) && ((criteria_R2 - 5 <= hypothesis) && (criteria_R2 + 5 >= hypothesis))))
                return 0;
            else if (index_belong == "R1")
                criteria_R1 = hypothesis;
            else if (index_belong == "R2")
                criteria_R2 = hypothesis;
        }
        return 1;
    }

    // FIXME Multi thread!!
    void confirm_CB(const std_msgs::Int32::ConstPtr &turn_on)
    {
        // ROS_INFO("Loop node was actuated");
        std_msgs::Int32 msg;

        msg.data = 1;

        rtabmap::CameraImages camera(template_path);
        // ディレクトリができるのを待つ必要がある.
        if (!camera.init())
        {
            ROS_ERROR("Camera init failed!");
            exit(1);
        }

        std::string jpg{".jpg"};
        std::string io_num{std::to_string(nextIndex) + jpg};

        // REVIEW sleepは全ての機能を一時停止にする

        // WARNING CBにwhileを書いてはいけない.forなら耐える

        // Rename the files
        std::map<std::string, int> ans{rename(count_files(), jpg)};
        // NOTE "R1start","R1end","R2start","R2end"

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

                    if (judge(rtabmap.getLoopClosureId(), ans))
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
                        // ロボット間の検知
                        // R1の検知
                        if (ans["R1start"] <= nextIndex && ans["R1end"] >= nextIndex)
                        {
                            if ((sorted_original[nextIndex] - sorted_original[rtabmap.getLoopClosureId()]) % 2 == 1)
                            {
                                ROS_WARN("This is the loop between Robots");
                                who_detect[1]["index"].push_back(sorted_original[nextIndex]);
                                who_detect[1]["LoopID"].push_back(sorted_original[rtabmap.getLoopClosureId()]);
                            }
                            all_loop[1]["index"].push_back(nextIndex);
                            all_loop[1]["LoopID"].push_back(rtabmap.getLoopClosureId());
                        }
                        else
                        {
                            if ((sorted_original[nextIndex] - sorted_original[rtabmap.getLoopClosureId()]) % 2 == 1)
                            {
                                ROS_WARN("This is the loop between Robots");
                                who_detect[2]["index"].push_back(sorted_original[nextIndex]);
                                who_detect[2]["LoopID"].push_back(sorted_original[rtabmap.getLoopClosureId()]);
                            }
                            all_loop[2]["index"].push_back(nextIndex);
                            all_loop[2]["LoopID"].push_back(rtabmap.getLoopClosureId());
                        }
                    }
                    else
                        ROS_ERROR("False positive was detected");
                }

                ++nextIndex;
                data = camera.takeImage();
            }
            else
                break;
        }
        // TODO send_commandの実装
        // send_command
        clear_dir();
        img_switch.publish(msg);
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
