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
#include <nav_msgs/Path.h>

using namespace open3d;

class SubscriberExample
{

private:
    std::shared_ptr<open3d::geometry::PointCloud> pcd_1 = std::make_shared<open3d::geometry::PointCloud>();
    std::shared_ptr<open3d::geometry::PointCloud> pcd_2 = std::make_shared<open3d::geometry::PointCloud>();
    ros::NodeHandle nh;
    ros::Subscriber cloud_sub1;
    ros::Subscriber cloud_sub2;

    std::vector<double> t;
    std::vector<double> rpy;
    ros::Subscriber rt_sub;

    ros::Subscriber path_sub1;
    ros::Subscriber path_sub2;

    open3d::visualization::Visualizer *vis_1{new open3d::visualization::Visualizer()};

    std::vector<Eigen::Vector3d> nodes_1;
    std::vector<Eigen::Vector3d> nodes_2;
    std::vector<Eigen::Vector3d> colors_1;
    std::vector<Eigen::Vector3d> colors_2;

    std::vector<Eigen::Vector2i> edges_1;
    std::vector<Eigen::Vector2i> edges_2;

    Eigen::Matrix4d transformation;
    int check{0};

public:
    // open3d::geometry::PointCloud pcd_1;

    SubscriberExample(void)
    {
        cloud_sub1 = nh.subscribe("robot2/rtabmap/cloud_map", 10, &SubscriberExample::cloud_cb1, this);
        cloud_sub2 = nh.subscribe("robot1/rtabmap/cloud_map", 10, &SubscriberExample::cloud_cb2, this);
        rt_sub = nh.subscribe("RT_result", 10, &SubscriberExample::cloudAssembler, this);

        path_sub1 = nh.subscribe("robot2/rtabmap/mapPath", 2, &SubscriberExample::pathAssembler1, this);
        path_sub2 = nh.subscribe("robot1/rtabmap/mapPath", 2, &SubscriberExample::pathAssembler2, this);

        pcd_1 = pcd_1->VoxelDownSample(0.02);
        pcd_2 = pcd_2->VoxelDownSample(0.02);

        vis_1->CreateVisualizerWindow();
    }

    void pathAssembler1(const nav_msgs::Path::ConstPtr &path)
    {
        nodes_1.clear();
        edges_1.clear();
        colors_1.clear();
        unsigned long int iteration{0};
        for (auto val : path->poses)
        {
            iteration += 1;
            Eigen::Vector3d pose(val.pose.position.x, val.pose.position.y, val.pose.position.z);
            nodes_1.push_back(pose);
            if (iteration > 1)
            {
                Eigen::Vector2i indice(nodes_1.size() - 1, nodes_1.size());
                edges_1.push_back(indice);
                if (iteration == path->poses.size())
                {
                    Eigen::Vector3d first(1.0d, 1.0d, 1.0d);
                    colors_1.push_back(first);
                }
                else
                    colors_1.push_back(Eigen::Vector3d::UnitX());
            }
        }
    }

    void pathAssembler2(const nav_msgs::Path::ConstPtr &path)
    {
        nodes_2.clear();
        edges_2.clear();
        colors_2.clear();
        unsigned long int iteration{0};
        for (auto val : path->poses)
        {
            iteration += 1;
            Eigen::Vector3d pose(val.pose.position.x, val.pose.position.y, val.pose.position.z);
            nodes_2.push_back(pose);
            if (iteration > 1)
            {
                Eigen::Vector2i indice(nodes_2.size() - 1, nodes_2.size());
                edges_2.push_back(indice);
                if (iteration == path->poses.size())
                {
                    Eigen::Vector3d first(1.0d, 1.0d, 1.0d);
                    colors_2.push_back(first);
                }
                else
                    colors_2.push_back(Eigen::Vector3d::UnitY());
            }
        }
    }

    void cloudAssembler(const cpp::RO_Array::ConstPtr &transform)
    {
        Eigen::Quaterniond rotation(transform->euler[0], transform->euler[1], transform->euler[2], transform->euler[3]);
        Eigen::Translation<double, 3> translation(transform->translation[0], transform->translation[1], transform->translation[2]);
        Eigen::Affine3d affine = translation * rotation;

        // check += 1;
        if (check == 1)
        {
            transformation = affine.matrix();
        }
    }

    void cloud_cb1(const sensor_msgs::PointCloud2::ConstPtr &cloud_data)
    {

        if (nodes_1.size() > 5)
        {
            open3d::geometry::PointCloud *pcd_ = pcd_1.get();
            open3d_conversions::rosToOpen3d(cloud_data, *pcd_);
        }
    }

    void cloud_cb2(const sensor_msgs::PointCloud2::ConstPtr &cloud_data)
    {
        if (nodes_2.size() > 5)
        {
            open3d::geometry::PointCloud *pcd_ = pcd_2.get();
            open3d_conversions::rosToOpen3d(cloud_data, *pcd_);

            std::shared_ptr<open3d::geometry::LineSet> line_1 = std::make_shared<open3d::geometry::LineSet>(nodes_1, edges_1);
            std::shared_ptr<open3d::geometry::LineSet> line_2 = std::make_shared<open3d::geometry::LineSet>(nodes_2, edges_2);
            line_1->colors_ = colors_1;
            line_2->colors_ = colors_2;
            if (check > 0)
            {
                pcd_2->Transform(transformation);
                line_2->Transform(transformation);

                vis_1->AddGeometry(pcd_1);
                vis_1->AddGeometry(pcd_2);
                vis_1->AddGeometry(line_1);
                vis_1->AddGeometry(line_2);

                vis_1->UpdateGeometry(pcd_1);
                vis_1->UpdateGeometry(pcd_2);
                vis_1->UpdateGeometry(line_1);
                vis_1->UpdateGeometry(line_2);

                vis_1->PollEvents();
                vis_1->UpdateRender();
                vis_1->ClearGeometries();
                vis_1->UpdateGeometry();
            }
            else
            {
                vis_1->AddGeometry(pcd_2);
                // vis_1->AddGeometry(line_2);

                vis_1->UpdateGeometry(pcd_2);
                // vis_1->UpdateGeometry(line_2);

                vis_1->PollEvents();
                vis_1->UpdateRender();
                vis_1->UpdateGeometry();
                vis_1->ClearGeometries();
            }
        }
    }

    // Do something with the Open3D pointcloud
};

int main(int argc, char **argv)
{
    ros::init(argc, argv, "open3d_conversions_ex_sub");
    SubscriberExample subscriberExample;
    ros::spin();
}