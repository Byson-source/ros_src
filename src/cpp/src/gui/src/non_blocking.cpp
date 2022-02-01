#include <iostream>
#include <memory>
#include <thread>

#include <Eigen/Core>
#include <Eigen/Geometry>

#include "open3d/Open3D.h"

int main(int argc, char **argv)
{
    using namespace std;

    open3d::utility::SetVerbosityLevel(open3d::utility::VerbosityLevel::Debug);
    auto cloud_ptr_1 = std::make_shared<open3d::geometry::PointCloud>();
    auto cloud_ptr_2 = std::make_shared<open3d::geometry::PointCloud>();
    open3d::io::ReadPointCloud("/home/ayumi/Open3D/examples/test_data/ICP/cloud_bin_0.pcd", *cloud_ptr_1);
    open3d::io::ReadPointCloud("/home/ayumi/Open3D/examples/test_data/ICP/cloud_bin_1.pcd", *cloud_ptr_2);

    cloud_ptr_1 = cloud_ptr_1->VoxelDownSample(0.02);
    cloud_ptr_2 = cloud_ptr_2->VoxelDownSample(0.02);

    Eigen::Matrix4d trans;

    trans << 0.862, 0.011, -0.507, 0.0,
        -0.139, 0.967, -0.215, 0.7,
        0.487, 0.255, 0.835, -1.4,
        0.0, 0.0, 0.0, 1.0;
    cloud_ptr_1->Transform(trans);

    Eigen::Matrix4d flip;
    flip << 1, 0, 0, 0,
        0, -1, 0, 0,
        0, 0, -1, 0,
        0, 0, 0, 1;

    cloud_ptr_1->Transform(flip);
    cloud_ptr_2->Transform(flip);

    open3d::visualization::Visualizer vis;
    vis.CreateVisualizerWindow();
    vis.AddGeometry(cloud_ptr_1);
    vis.AddGeometry(cloud_ptr_2);
    double threshold{0.05};
    size_t icp_iteration{100};
    bool save_image{0};

    Eigen::Translation<double, 3> trans_1(0.0, 1.0, 1.0);
    Eigen::MatrixXd rot = Eigen::MatrixXd::Identity();
    for (int i{0}; i < icp_iteration; ++i)
    {
        trans_1.x() += 0.3;
        Eigen::Affine3d affine = trans_1 * rot;

        Eigen::Matrix4d transform = affine.matrix();

        cloud_ptr_1->Transform(transform);

        vis.UpdateGeometry(*cloud_ptr_1);
        vis.PollEvents();
        vis.UpdateRender();
        // vis.RemoveGeometry();
    }
    vis.DestroyWindow();
}