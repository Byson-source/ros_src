#include "rtabmap/core/Rtabmap.h"
#include "rtabmap/core/CameraRGB.h"
#include <opencv2/core/core.hpp>
#include "rtabmap/utilite/UFile.h"
#include <stdio.h>
#include "std_msgs/String.h"
#include <ros/ros.h>
#include <sstream>//stringstream is included
#include <string>

std::string target_path{};
std::string *path_ptr{&target_path};

void dir_callback(const std_msgs::String::ConstPtr &msg){
    *path_ptr=msg->data.c_str;
}


void switchcallback(const std_msgs::String::ConstPtr &msg, int index){
   	if(msg->data.c_str()=="Y"){
        if(data.imageRaw().empty()==1)
            nextIndex-=1;
        rtabmap.process(data.imageRaw(), nextIndex);
        ++i;
        if(rtabmap.getLoopClosureId())
        {
            printf("Loop-closure detected!!");
            msg.data="Detected";
            chatter_publisher.publish(msg)
            // Cleanup... save database and logs
            printf("Saving Long-Term Memory to \"rtabmap.db\"...\n");
            rtabmap.close();

        }else{
            msg.data="Not detected";
            chatter_publisher.publish(msg)
            printf("Nothing happened...");
        }
        ++nextIndex;

        //Get next image
        data = camera.takeImage();
       }

   	}

}

void rtabmap_setup(){
    std::string path{target_path};

    // rtabmap::Camera is simply a convenience wrapper of OpenCV cv::VideoCapture and cv::imread
    rtabmap::CameraImages camera(path);

    // Create RTAB-Map
    rtabmap::Rtabmap rtabmap;

    // Set the time threshold
    rtabmap.setTimeThreshold(700.0f); // Time threshold : 700 ms, 0 ms means no limit

    rtabmap::ParametersMap parameters;
    parameters.insert(rtabmap::ParametersPair(rtabmap::Parameters::kRtabmapLoopThr(), rtabmap::Parameters::defaultRtabmapLoopThr()));


    parameters.insert(rtabmap::ParametersPair(rtabmap::Parameters::kRGBDEnabled(), "false"));

    std::string databasePath = "home/ayumi/Documents/RTAB-Map/"+rtabmap::Parameters::getDefaultDatabaseName();
    // delete previous database if there's one...
    UFile::erase(databasePath);

    // Initialize rtabmap: delete/create database...
    rtabmap.init(parameters, databasePath);

    int i=0;
    rtabmap::SensorData data = camera.takeImage();
    int nextIndex = rtabmap.getLastLocationId()+1;

}

int main(int argc,char **argv){
    rtabmap_setup();

    std_msgs::String msg;
    ros::init(argc, argv, "detection_checker");
    ros::NodeHandle n;
    ros::Publisher chatter_publisher=n.advertise<std_msgs::String>("LoopClosureDetection",1000);
    ros::Subscriber dir_sub=n.subscribe("dir_command",1000,dir_callback);
    ros::Subscriber sub=n.subscribe("waiting_for_reprocess",1000,switch_callback);
    ros::Rate loop_rate(0.5);


    return 0;
}

