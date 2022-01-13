#include <iostream>
#include <opengv/absolute_pose/methods.hpp>
#include <opengv/absolute_pose/CentralAbsoluteAdapter.hpp>
#include <opengv/math/cayley.hpp>
#include <iomanip>
#include <Eigen/Core>
#include <Eigen/Geometry>

#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/eigen.hpp>

#include <ros/ros.h>
#include <string>
#include <vector>
#include "cpp/FeatureArray.h"
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

    std::vector<Eigen::Matrix<float, 4, 4>> poses_1;
    std::vector<Eigen::Matrix<float, 4, 4>> poses_2;

    // points location

    Calibration camera;

public:
    RO_Estimator()
    {
        feature_sub = n.subscribe("features", 20, &RO_Estimator::RO_CB, this);
        // from robot/map to robot/base_footprint
    }

    void RO_CB(const cpp::FeatureArray::ConstPtr &data)
    {
        tf::StampedTransform transform1;
        tf::StampedTransform transform2;
        try
        {
            listener1.lookupTransform("/robot1/map", "/robot1/base_footprint",
                                      ros::Time(0), transform1);
            listener2.lookupTransform("/robot2/map", "/robot2/base_footprint",
                                      ros::Time(0), transform2);
        }
        catch (tf::TransformException &ex)
        {
            ROS_ERROR("%s", ex.what());
            exit(1);
        }

        double x1 = transform1.getOrigin().x();
        double y1 = transform1.getOrigin().y();
        double z1 = transform1.getOrigin().z();
        double x2 = transform2.getOrigin().x();
        double y2 = transform2.getOrigin().y();
        double z2 = transform2.getOrigin().z();
        tf::Quaternion q1 = transform1.getRotation();
        tf::Quaternion q2 = transform2.getRotation();

        double roll1, pitch1, yaw1;
        double roll2, pitch2, yaw2;
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

        int who_detect = data->who_detect;
        int signal = data->signal;
        std::vector<cv::Point3f> kp_loc_r1_s;
        std::vector<cv::Point3f> kp_loc_r2_s;

        for (size_t index{1}; index < data->r1.size() + 1; ++index)
        {
            // ３つ目の要素に差し掛かった時
            if (index % 3 == 0)
            {
                cv::Point3f kp_loc_r1(data->r1[index - 2], data->r1[index - 1], data->r1[index]);
                cv::Point3f kp_loc_r2(data->r2[index - 2], data->r2[index - 1], data->r2[index]);
                kp_loc_r1_s.push_back(kp_loc_r1);
                kp_loc_r2_s.push_back(kp_loc_r2);
            }
        }

        if (signal == 0)
        {
            poses_1.push_back(pose1);
            poses_2.push_back(pose2);
            RO_Estimator::mlpnp(who_detect, kp_loc_r1_s, kp_loc_r2_s);
            // NOTE mlpnp
        }
        else
        {

                        // NOTE BA
        }
    }
    // FIXME 前処理が必要
    opengv::transformation_t mlpnp(int who_detect, std::vector<cv::Point3f> kp_loc_r1, std::vector<cv::Point3f> kp_loc_r2)
    {
        // bearing vectors
        // 3Dポイント集
        opengv::points_t points;
        if (who_detect == 1)
            points = camera.img2cam(kp_loc_r1, kp_loc_r2);
        else
            points = camera.img2cam(kp_loc_r2, kp_loc_r1);

        // Bearing Vectors
        opengv::bearingVectors_t bearingVectors;
        bearingVectors = camera.bearing_v();

        opengv::cov3_mats_t bearing_covs;
        bearing_covs = camera.v_cov();

        Eigen::MatrixXd cov_xx;
        Eigen::MatrixXd cov_ldld;

        opengv::absolute_pose::CentralAbsoluteAdapter adapter(bearingVectors,
                                                              points,
                                                              bearing_covs);

        int iterations = 50;
        opengv::transformation_t mlpnp_transformation;
        for (size_t i{0}; i < iterations; i++)
            mlpnp_transformation = opengv::absolute_pose::mlpnp(adapter, cov_xx, cov_ldld);

        std::cout << "transformation is..." << std::endl;
        std::cout << mlpnp_transformation << std::endl;
        std::cout << "cov is..." << std::endl;
        std::cout << cov_xx << std::endl;

        return mlpnp_transformation;
    }
};

int main(int argc, char **argv)
{
    ros::init(argc, argv, "RO_estimator");
    RO_Estimator detector;

    ros::spin();

    return 0;
}