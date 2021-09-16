//Input the first target_path!!
#include "rtabmap/core/Rtabmap.h"
#include "rtabmap/core/CameraRGB.h"
#include <opencv2/core/core.hpp>
#include "rtabmap/utilite/UFile.h"
#include <stdio.h>
#include "std_msgs/String.h"
#include <ros/ros.h>
#include <sstream>//stringstream is included
#include <string>
#include <direct.h>
int dir_number{1};
int *dir_number_ptr{&dir_number};
std::string target_path{"/home/ayumi/Documents/tohoku_uni/CLOVERs/"};
//detector, dir_changer,rest_commander

void switch_callback(const std_msgs::String::ConstPtr &msg, ros::Publisher &detector, ros::Publisher &dir_changer,
                     ros::Publisher rest_commander,ros::Publisher &turtlebot_power, rtabmap::Rtabmap &rtabmap,
                     rtabmap::CameraImages &camera, rtabmap::SensorData &data, rtabmap::ParametersMap &parameters){
   	if(msg->data.c_str()=="Rtabmap-reprocess finished"){
        if(data.imageRaw().empty()==1)
            nextIndex-=1;
            printf("There are no images in current folder...");
        else{
        rtabmap.process(data.imageRaw(), nextIndex);
        ++i;
            if(rtabmap.getLoopClosureId())
            {

                printf("Loop-closure detected!!");
                std_msgs::String mail;
                mail.data="Detected";
                detector.publish(mail);
                // Cleanup... save database and logs
                printf("Saving Long-Term Memory to \"rtabmap.db\"...\n");
                rtabmap.close();

                //Database is erased
                UFile::erase(databasePath);
                rtabmap.init(parameters,databasePath);

                //Tell the new location where images are stored...
                *dir_number_ptr+=1;
                std::string new_path=target_path+std::to_string(dir_number)+"database"
                //Prepare for new directory
                if (_mkdir(new_path)==1)
                    printf("Directory was successfully created!");
                else{
                    printf("There is a directory already exists. Now deleting...");
                    _rmdir(new_path);
                    _mkdir(new_path);
                }
                ros::Rate loop_rate(0.5);
                //Tell the new location where images are stored...
                std_msgs::String dir;
                dir.data=new_path;
                dir_changer.publish(dir);
                //To make image-extractor rest
                std_msgs::String rest_info;
                rest_info.data="Still rtabmap-reprocess running";
                rest_commander.publish(rest_info);
                //To shutdown turtlebot
                std_msgs::String power;
                power.data="OFF";
                turtlebot_power.publish(power);

                //Set up rtabmap for next...
                camera=new_path;
                camera.init();
                data=camera.takeImage();
                nextindex=1;
                printf("Rtabmap for next is ready...");
                loop_rate.sleep();

            }else{
                std::string new_path=target_path+std::to_string(dir_number_ptr)+"database"

                std_msgs::String rest_info;
                rest_info.data="store images";
                rest_commander.publish(rest_info);

                std_msgs::String dir;
                dir.data=new_path;
                dir_changer.publish(dir);

                std_msgs::String mail;
                mail.data="Not Detected";
                detector.publish(mail);

                printf("Nothing happened...");
                ++nextIndex;
                data = camera.takeImage();

                std_msgs::String power;
                power.data="ON";
                turtlebot_power.publish(power);

                loop_rate.sleep();
            }
        }
       }else{
           std::string new_path=target_path+std::to_string(dir_number_ptr)+"database"

            printf("detection_checker is waiting for rtabmap-reprocess...");

            std_msgs::String rest_info;
            rest_info.data="Still rtabmap-reprocess running";
            rest_commander.publish(rest_info);

            std_msgs::String dir;
            dir.data=new_path;
            dir_changer.publish(dir);

            std_msgs::String mail;
            mail.data="Detected";
            detector.publish(mail);

            std_msgs::String power;
            power.data="OFF";
            turtlebot_power.publish(power);

            loop_rate.sleep();
       }

        }


int main(){
    //==================================================================================================
    //==================================================================================================
    //==================================================================================================
    std::string path{target_path};
    rtabmap::CameraImages camera(path);
    if(!camera.init())
   {
   	printf("Camera init failed, using path \"%s\"\n", path.c_str());
   	return 1;
   }

    rtabmap::Rtabmap rtabmap;

    // Set the time threshold
    rtabmap.setTimeThreshold(700.0f); // Time threshold : 700 ms, 0 ms means no limit

    rtabmap::ParametersMap parameters;
    parameters.insert(rtabmap::ParametersPair(rtabmap::Parameters::kRtabmapLoopThr(), rtabmap::Parameters::defaultRtabmapLoopThr()));
    parameters.insert(rtabmap::ParametersPair(rtabmap::Parameters::kRGBDEnabled(), "false"));

//    int database_num{1};
//    std::string database_num_str{std::to_string(database_num)};

    std::string databasePath = "/home/ayumi/Documents/RTAB-Map/"+rtabmap::Parameters::getDefaultDatabaseName();
    // delete previous database if there's one...
    UFile::erase(databasePath);

    // Initialize rtabmap: delete/create database...
    rtabmap.init(parameters, databasePath);

    int i=0;
    rtabmap::SensorData data = camera.takeImage();
    int nextIndex = rtabmap.getLastLocationId()+1;
    //==================================================================================================
    //==================================================================================================
    //==================================================================================================

    ros::init(argc, argv, "detection_checker");
    ros::NodeHandle n;

    //To turn on-off the node that conducts rtabmap-reprocess
    ros::Publisher detector=n.advertise<std_msgs::String>("LoopClosureDetection",1000);
    //To notify the change of location where the image is stored
    ros::Publisher dir_changer=n.advertise<std_msgs::String>("dir_info",1000);
    //To turn on-off the image extractor node.
    ros::Publisher rest_commander=n.advertise<std_msgs::String>("take_rest",1000);
    //To turn on-off turtlebot3
    ros::Publisher turtlebot_power=n.advertise<std_msgs::String>("power_info",1000);
    //To wait for rtabmap-reprocess node
    ros::Subscriber sub=n.subscribe("waiting_for_reprocess",1000,switch_callback,
                                    detector, dir_changer, rest_commander, turtlebot_power,
                                    rtabmap, camera, data, parameters);
    ros::Rate loop_rate(0.5);


    return 0;
}

