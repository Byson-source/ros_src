#include <ros/ros.h>

#include <Eigen/Core>
#include <Eigen/Geometry>

#include <string>
#include <vector>

#include <open3d/Open3D.h>
#include <sensor_msgs/PointCloud2.h>

#include "open3d_conversions/open3d_conversions.h"
#include "cpp/RO_Array.h"
#include <ros/package.h>

using namespace open3d;

class SubscriberExample
{

private:
    std::shared_ptr<open3d::geometry::PointCloud> pcd = std::make_shared<open3d::geometry::PointCloud>();
    ros::NodeHandle nh;
    ros::Subscriber cloud_sub1;
    ros::Subscriber cloud_sub2;

    std::vector<double> t;
    std::vector<double> rpy;

    ros::Subscriber transform_sub;
    ros::Subscriber rt_sub;

    ros::Subscriber path_sub1;
    ros::Subscriber path_sub2;

    open3d::visualization::Visualizer vis_1;
    open3d::visualization::Visualizer vis_2;

    std::vector<Eigen::Vector4d> nodes_1;
    std::vector<Eigen::Vector4d> nodes_2;

    std::vector<Eigen::Vector2i> edges_1;
    std::vector<Eigen::Vector2i> edges_2;

    Eigen::Matrix4d transformation;

    std::vector<int> status;

public:
    // open3d::geometry::PointCloud pcd;

    SubscriberExample(void)
    {
        cloud_sub1 = nh.subscribe("robot1/rtabmap/cloud_map", 10, &SubscriberExample::cloud_cb1, this);
        cloud_sub2 = nh.subscribe("robot2/rtabmap/cloud_map", 10, &SubscriberExample::cloud_cb2, this);
        rt_sub = nh.subscribe("RT_result", 10, &SubscriberExample::cloudAssembler, this);

        path_sub1 = nh.subscribe("robot1/rtabmap/mapPath", 2, &SubscriberExample::pathAssembler1, this);
        path_sub1 = nh.subscribe("robot1/rtabmap/mapPath", 2, &SubscriberExample::pathAssembler2, this);

        // transform_sub = nh.subscribe("transform_info", 10, &SubscriberExample::cloud_cb2);
        // open3d::utility::SetVerbosityLevel(utility::VerbosityLevel::Debug);
        pcd = pcd->VoxelDownSample(0.02);
        vis_1.CreateVisualizerWindow();
        vis_2.CreateVisualizerWindow();
    }

    void pathAssembler1(const nav_msgs::Path::ConstPtr &path)
    {
        nodes_1.clear();
        for (auto val : path->poses)
        {
            Eigen::Vector4d pose(val.pose.position.x, val.pose.position.y, val.pose.position.z, 1.0d);
            nodes_1.push_back(pose);
        }
        for (int iteration{0}; iteration < (nodes_1.size() - edges_1.size()); ++iteration)
        {
            Eigen::Vector2i indice(edges_1.size() + 1, edges_1.size());
            edges_1.push_back(indice);
        }
    }

    void pathAssembler2(const nav_msgs::Path::ConstPtr &path)
    {
        nodes_2.clear();
        for (auto val : path->poses)
        {
            Eigen::Vector3d pose(val.pose.position.x, val.pose.position.y, val.pose.position.z, 1.0d);
            nodes_2.push_back(pose);
        }
    }

    void cloudAssembler(const cpp::RO_Array::ConstPtr &transform)
    {
        status.push_back(1);
        Eigen::Quaterniond rotation(transform->euler[0], transform->euler[1], transform->euler[2], transform->euler[3]);
        Eigen::Translation<double, 3> translation(transform->translation[0], transform->translation[1], transform->translation[2]);
        Eigen::Affine3d affine = translation * rotation;

        transformation = affine.matrix();
        status.push_back(1);
    }

    void cloud_cb1(const sensor_msgs::PointCloud2::ConstPtr &cloud_data)
    {

        open3d::geometry::PointCloud *pcd_ = pcd.get();
        open3d_conversions::rosToOpen3d(cloud_data, *pcd_);
        if (status.back() == 0)
            status.push_back(0);
        else
        {
            if (status[status.size() - 2] == 0)
            {
                vis_2.DestroyVisualizerWindow();
                status.push_back(999);
            }
        }
        vis_1.AddGeometry(pcd);
        vis_1.UpdateGeometry(pcd);
        vis_1.PollEvents();
        vis_1.UpdateRender();
        vis_1.RemoveGeometry(pcd);
        // Do something with the Open3D pointcloud
    }

    void cloud_cb2(const sensor_msgs::PointCloud2::ConstPtr &cloud_data)
    {
        open3d::geometry::PointCloud *pcd_ = pcd.get();
        open3d_conversions::rosToOpen3d(cloud_data, *pcd_);
        if (status.back() == 0)
        {
            status.push_back(0);
            vis_2.AddGeometry(pcd);
            vis_2.UpdateGeometry(pcd);
            vis_2.PollEvents();
            vis_2.UpdateRender();
            vis_2.RemoveGeometry(pcd);
        }
        else
        {
            if (status.back() == 0)
            {
                vis_2.DestroyVisualizerWindow();
                status.push_back(999);
            }
            pcd->Transform(transformation);

            vis_1.AddGeometry(pcd);
            vis_1.UpdateGeometry(pcd);
            vis_1.PollEvents();
            vis_1.UpdateRender();
            vis_1.RemoveGeometry(pcd);
        }

        // Do something with the Open3D pointcloud
    }
};

int main(int argc, char **argv)
{
    ros::init(argc, argv, "open3d_conversions_ex_sub");
    SubscriberExample subscriberExample;
    ros::spin();
}