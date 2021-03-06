#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <pcl_ros/transforms.h>

#include <cmath>

#include <Eigen/Core>
#include <Eigen/Geometry>

#include <string>
#include <vector>

#include <sensor_msgs/PointCloud2.h>
#include <sensor_msgs/point_cloud_conversion.h>

#include "cpp/RO_Array.h"
#include <ros/package.h>
#include <nav_msgs/Path.h>

class SubscriberExample
{

private:
    ros::NodeHandle nh;
    ros::Subscriber cloud_sub2;
    ros::Publisher cloud_pub;

    // tf2_ros::TransformBroadcaster dynamic_tr;
    // ros::Timer timer;

    std::vector<double> t;
    std::vector<double> rpy;
    ros::Subscriber rt_sub;

    Eigen::Matrix4d transformation;
    int status{0};

public:
    SubscriberExample(void)
    {
        cloud_sub2 = nh.subscribe("robot2/rtabmap/cloud_map", 10, &SubscriberExample::cloud_cb2, this);
        rt_sub = nh.subscribe("RT_result", 10, &SubscriberExample::cloudAssembler, this);

        cloud_pub = nh.advertise<sensor_msgs::PointCloud2>("transformed_cloud", 10);

        // path_sub1 = nh.subscribe("robot2/rtabmap/mapPath", 2, &SubscriberExample::pathAssembler1, this);
        // path_sub2 = nh.subscribe("robot1/rtabmap/mapPath", 2, &SubscriberExample::pathAssembler2, this);
    }

    void cloudAssembler(const cpp::RO_Array::ConstPtr &transform)
    {
        Eigen::Quaterniond rotation(transform->euler[0], transform->euler[1], transform->euler[2], transform->euler[3]);
        Eigen::Translation<double, 3> translation(transform->translation[0], transform->translation[1], transform->translation[2]);
        Eigen::Affine3d affine = translation * rotation;
        transformation = affine.matrix();
        // Eigen::Matrix3d rotation_test = transformation.block(0, 0, 3, 3);

        // Eigen::Vector3d euler = rotation_test.eulerAngles(0, 1, 2);
        // std::cout << euler << std::endl;
        status = 1;
    }

    void cloud_cb2(sensor_msgs::PointCloud2 cloud_data)
    {
        if (status == 1)
        {
            sensor_msgs::PointCloud2 pcl2_transformed;
            pcl_ros::transformPointCloud(transformation.cast<float>(), cloud_data, pcl2_transformed);
            pcl2_transformed.header.frame_id = "robot1/map";
            cloud_pub.publish(pcl2_transformed);
        }
    }
};

int main(int argc, char **argv)
{
    ros::init(argc, argv, "gui");
    SubscriberExample subscriberExample;
    ros::spin();
}