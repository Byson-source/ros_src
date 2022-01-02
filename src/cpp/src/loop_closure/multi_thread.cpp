#include <ros/ros.h>
#include <std_msgs/Header.h>

#include <iostream>
#include <mutex>

// std::mutex m;

void head_cb1(const std_msgs::HeaderConstPtr& msg){
    // for(int i{0};i<std::pow(10,8);++i){
    // }
    // std::lock_guard<std::mutex> lock(m);  //ロックする
    std::printf("Received1 : %d %f %f\r\n", msg->seq, msg->stamp.toSec(), ros::Time::now().toSec() - msg->stamp.toSec());
    ROS_INFO("Received1 : %d %f %f", msg->seq, msg->stamp.toSec(), ros::Time::now().toSec() - msg->stamp.toSec());
}

void head_cb2(const std_msgs::HeaderConstPtr& msg){
    // std::lock_guard<std::mutex> lock(m);  //ロックする
    std::printf("Received2 : %d %f %f\r\n", msg->seq, msg->stamp.toSec(), ros::Time::now().toSec() - msg->stamp.toSec());
    ROS_INFO("Received2 : %d %f %f", msg->seq, msg->stamp.toSec(), ros::Time::now().toSec() - msg->stamp.toSec());
}

int main (int argc, char** argv){
    ros::init(argc, argv, "spin_study");

    ros::NodeHandle nh("~");
    ros::Publisher pub;
    ros::Subscriber sub1;
    ros::Subscriber sub2;

    pub = nh.advertise<std_msgs::Header>("/header", 1000);
    sub2 = nh.subscribe("/header",10,head_cb2);
    sub1 = nh.subscribe("/header",10,head_cb1);

    // ros::AsyncSpinner spinner(1);  //spinを処理するスレッド数を引数に渡す
    // spinner.start();
    ros::Rate rate(1);
    while(ros::ok()){
        {
            // 排他制御のためにスコープを追加
            // std::lock_guard<std::mutex> lock(m);
            std_msgs::Header header;
            header.seq = 0;
            header.stamp = ros::Time::now();
            ROS_INFO("publish() : %d %f", header.seq, header.stamp.toSec());
            pub.publish(header);

            header.seq = 1;
            header.stamp = ros::Time::now();
            ROS_INFO("publish() : %d %f", header.seq, header.stamp.toSec());
            pub.publish(header);
        }
        //AsyncSpinnerを使わない場合は↓が必要
        ros::spinOnce();
        ROS_INFO("sleep()");
        rate.sleep();
    }
    // spinner.stop();

    return 0;
}