#include <ros/ros.h>

#include <Eigen/Core>
#include <Eigen/Geometry>

#include <string>
#include <vector>

#include <open3d/Open3D.h>
#include <sensor_msgs/PointCloud2.h>

#include "open3d_conversions/open3d_conversions.h"
#include <ros/package.h>

using namespace open3d;

class SubscriberExample
{
private:
    auto cloud_ptr = std::make_shared<geometry::PointCloud>();
    ros::NodeHandle nh;
    ros::Subscriber cloud_sub1;
    ros::Subscriber cloud_sub2;

public:
    open3d::geometry::PointCloud pcd;

    SubscriberExample(void);
    {
        cloud_sub1 = nh.subscribe("robot1/rtabmap/cloud_map", 10, &SubscriberExample::cloud_cb1, this);
        cloud_sub2 = nh.subscribe("robot2/rtabmap/cloud_map", 10, &SubscriberExample::cloud_cb2, this);
    }

    void cloud_cb1(const sensor_msgs::PointCloud2ConstPtr &cloud_data)
    {
        open3d::geometry::PointCloud pcd;
        open3d_conversions::rosToOpen3d(cloud_data, pcd);

        // Do something with the Open3D pointcloud
    }

    void cloud_cb2(const sensor_msgs::PointCloud2ConstPtr &cloud_data)
    {
        open3d::geometry::PointCloud pcd;
        open3d_conversions::rosToOpen3d(cloud_data, pcd);

        // Do something with the Open3D pointcloud
    }
};

int main(int argc, char **argv)
{
    ros::init(argc, argv, "open3d_conversions_ex_sub");
    SubscriberExample subscriberExample;
    ros::spin();
}