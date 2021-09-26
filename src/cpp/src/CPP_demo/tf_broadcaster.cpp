#include <ros/ros.h>
#include <tf/transform_broadcaster.h>

int main(int argc, char **argv)
{
    ros::init(argc, argv, "frame_a_to_frame_b_broadcaster");
    ros::NodeHandle node;
    tf::TransformBroadcaster br;
    tf::Transform transform;

    ros::Rate rate(10.0);

    while (node.ok())
    {
        transform.setOrigin(tf::Vector3(1.0, 2.0, 3.0));
        transform.setRotation(tf::Quaternion(0, 0, 0, 1));
        br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "frame_b", "frame_a"));
        rate.sleep();
    }
    //    tf::Quaternion q;
    //  14   q.setRPY(0, 0, msg->theta);

    return 0;
}
