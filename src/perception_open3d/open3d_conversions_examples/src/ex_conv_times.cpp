// Copyright 2020 Autonomous Robots Lab, University of Nevada, Reno

// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0

// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// Open3D
#include <open3d/Open3D.h>

// ROS
#include <ros/ros.h>
#include <ros/package.h>
#include <sensor_msgs/PointCloud2.h>

// open3d_conversions
#include "open3d_conversions/open3d_conversions.h"

// C++
#include <string>
#include <iostream>

class ConversionTimesExample
{
private:
  ros::NodeHandle nh_;
  ros::Publisher pub_;

public:
  std::string path;
  sensor_msgs::PointCloud2 ros_pc2;
  open3d::geometry::PointCloud test_o3d_pc;
  open3d::geometry::PointCloud o3d_pc;

  ConversionTimesExample(ros::NodeHandle& nh, std::string path) : nh_(nh)
  {
    open3d::io::ReadPointCloud(path, o3d_pc);
    pub_ = nh_.advertise<sensor_msgs::PointCloud2>("pointcloud", 1);
    calculateConversionTime(o3d_pc, ros_pc2);
  }

  virtual ~ConversionTimesExample()
  {
  }

  void calculateConversionTime(const open3d::geometry::PointCloud& pointcloud, sensor_msgs::PointCloud2& ros_pointcloud)
  {
    // Conversion time from Open3D PointCloud to sensor_msgs::PointCloud2
    std::clock_t tic = std::clock();
    open3d_conversions::open3dToRos(o3d_pc, ros_pc2, "o3d_frame");
    std::cout << "Conversion time (open3dToRos) : " << float(std::clock() - tic) / CLOCKS_PER_SEC * 1000.0 << " ms"
              << std::endl;

    // Conversion time from sensor_msgs::PointCloud2 to Open3D PointCloud
    const sensor_msgs::PointCloud2ConstPtr& ros_pc2_ptr = boost::make_shared<sensor_msgs::PointCloud2>(ros_pc2);
    std::clock_t toc = std::clock();
    open3d_conversions::rosToOpen3d(ros_pc2_ptr, test_o3d_pc);
    std::cout << "Conversion time (rosToOpen3d) : " << float(std::clock() - toc) / CLOCKS_PER_SEC * 1000.0 << " ms \n"
              << std::endl;
    std::cout << "Number of points: " << o3d_pc.points_.size() << std::endl;

    // Visualize result using RViz
    ros_pc2.header.stamp = ros::Time::now();
    pub_.publish(ros_pc2);
  }
};

int main(int argc, char** argv)
{
  ros::init(argc, argv, "open3d_conversions_ex_conv_times");
  ros::NodeHandle nh;
  ros::Rate loop_rate(2);
  std::string path;
  if (argc == 2)
  {
    path = (std::string)argv[1];
  }
  else
  {
    std::string path_pkg = ros::package::getPath("open3d_conversions_examples");
    path = path_pkg + "/data/fragment.pcd";
  }
  while (ros::ok())
  {
    ConversionTimesExample conversionTimesExample(nh, path);
    ros::spinOnce();
    loop_rate.sleep();
  }
}
