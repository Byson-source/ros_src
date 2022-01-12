#include <iostream>
#include <opengv/absolute_pose/methods.hpp>
#include <opengv/absolute_pose/CentralAbsoluteAdapter.hpp>
#include <opengv/math/cayley.hpp>
#include <Eigen/Core>
#include <Eigen/Geometry>

#include <ros/ros.h>
#include <string>
#include <vector>
#include "cpp/MultiArray.h"
#include "cpp/RO_Array.h"
#include <math.h>
#include "calibration.h"
// FIXME CMakeLists.txtを修正
#include <tf/transform_listener.h>

class RO_Estimator
{
private:
    ros::NodeHandle n;
    ros::Subscriber feature_sub;
    ros::Subscriber pose_sub;

    tf::TransformListener listener1;
    tf::TransformListener listener2;

public:
    RO_Estimator()
    {
        feature_sub = n.subscribe("features", 20, &RO_estimator::RO_CB, this);
        // from robot/map to robot/base_footprint
    }

    void RO_CB(const std_msgs::cpp::ConstPtr &data)
    {
        tf::StampedTransform transform1;
        tf::StampedTransform transform2;
        try
        {
            listener1.lookupTransform("/robot1/map", "/robot1/base_footprint",
                                      ros::Time(0), transform1)
                listener2.lookupTransform("/robot2/map", "/robot2/base_footprint",
                                          ros::Time(0), transform2)
        }
        catch (tf::TransformException &ex)
        {
            ROS_ERROR("%s", ex.what());
            continue;
        }
        double x1, y1, z1 = transform1.getOrigin().x(), transform1.getOrigin().y(), transform1.getOrigin().z();
        double x2, y2, z2 = transform2.getOrigin().x(), transform2.getOrigin().y(), transform2.getOrigin().z();
        double roll1, pitch1, yaw1;
        double roll2, pitch2, yaw2;
        tf::Quaternion q1, q2 = transform1.getRotation(), transform2.getRotation();
        tf::Matrix3x3(q1).getRPY(roll1, pitch1, yaw1);
        tf::Matrix3x3(q2).getRPY(roll2, pitch2, yaw2);

        Eigen::Matrix<float, 4, 4> pose1, pose2;
        pose1 << cos(roll1) * cos(pitch1), cos(roll1) * sin(pitch1) * sin(yaw1) - sin(roll1) * cos(yaw1), cos(roll1) * sin(pitch1) * cos(yaw1) + sin(roll1) * sin(yaw1), x1,
            sin(roll1) * cos(pitch1), sin(roll1) * sin(pitch1) * sin(yaw1) + cos(roll1) * cos(yaw1), sin(roll1) * sin(pitch1) * cos(yaw1) - cos(roll1) * sin(yaw1), y1,
            -sin(pitch1), cos(pitch1) * sin(yaw1), cos(pitch1) * cos(yaw1), z1,
            0, 0, 0, 1;
        pose2 << cos(roll2) * cos(pitch2), cos(roll2) * sin(pitch2) * sin(yaw2) - sin(roll2) * cos(yaw2), cos(roll2) * sin(pitch2) * cos(yaw2) + sin(roll2) * sin(yaw2), x2,
            sin(roll2) * cos(pitch2), sin(roll2) * sin(pitch2) * sin(yaw2) + cos(roll2) * cos(yaw2), sin(roll2) * sin(pitch2) * cos(yaw2) - cos(roll2) * sin(yaw2), y2,
            -sin(pitch2), cos(pitch2) * sin(yaw2), cos(pitch2) * cos(yaw2), z2,
            0, 0, 0, 1;

        // bearing vectors
        Eigen::MatrixXd cov_xx;
        Eigen::MatrixXd cov_ldld;
        points_t points;
        // FIXME pointsの座標はcalibration.hで求める
        Eigen::MatrixXd gt(3, $numberofPoints);
        // FIXME number of points,店の三次元座標

        opengv::bearingVectors_t bearingVectors;
        size_t numberCams{1};
        size_t caamCorrespondence{0};
        // FIXME number of points
        for (size_t i = 0; i < $Number of points; i++)
        {
            translation_t camOffset = Eigen::Vector3d::Zero();
            rotation_t camRotation = Eigen::Matrix3d::Identity();
            points.push_back(gt.col(i));

            point_t bodyPoint = rotation.transpose() * (gt.col(i) - position);
            // FIXME multiThreadの必要？？どこかでPoseを知る
        }
    }
};