#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>//stringstream is included
#include <unistd.h>
#include <time.h>

int main(int argc, char **argv)
{
    ros::init(argc, argv, "talker_node");
    ros::NodeHandle n;
    //rospy.init_node("node_name",anonymous=True)
    ros::Publisher chatter_publisher=n.advertise<std_msgs::String>("chatter",1000);
    ros::Rate loop_rate(1);
    ros::Rate loop(0.1);
    // NOTE 一秒ごとにトピックを送信する回数!

    int count{0};
    clock_t start=clock();
    while (ros::ok())
    {
        std_msgs::String msg;
        std::stringstream ss;
        ss<<"Hello World "<<count;
        msg.data=ss.str();//The content in ss
        ROS_INFO("[Talker] I published %s\n",msg.data.c_str());
        chatter_publisher.publish(msg);

        ros::spinOnce();
        loop_rate.sleep();
        // NOTE 正確にはこのfrequencyを維持しようとする
        std_msgs::String msg2;
        std::stringstream ss2;
        ss2<<"Yahooooooooooooooo "<<count;
        msg2.data=ss2.str();//The content in ss
        chatter_publisher.publish(msg2);

        loop.sleep();
        count++;
    }
    return 0;
}
