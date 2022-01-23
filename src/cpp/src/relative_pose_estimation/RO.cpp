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

    tf::TransformListener listener;

    std::vector<Eigen::Matrix<float, 4, 4>> poses_1;
    std::vector<Eigen::Matrix<float, 4, 4>> poses_2;

    std::vector<int> loop_info;

    // points location

    Calibration camera;

public:
    RO_Estimator(void)
    {
        feature_sub = n.subscribe("features", 20, &RO_Estimator::RO_CB, this);
        // from robot/map to robot/base_footprint
    }

    void RO_CB(const cpp::FeatureArray::ConstPtr &data)
    {
        if ((data->r1.size() > 27) && (data->signal == 0))
        // NOTE特徴点が10個以上ないとだめ
        {

            tf::StampedTransform transform_map2odom;
            tf::StampedTransform transform_odom2base;
            try
            {
                if (data->who_detect == 1)
                {
                    // listener.waitForTransform("/robot1/map", "/robot1/odom", ros::Time(0), ros::Duration(3.0));
                    listener.lookupTransform("/robot1/map", "/robot1/odom",
                                             ros::Time(0), transform_map2odom);
                    // listener.waitForTransform("/robot1/map", "/robot1/odom", ros::Time(0), ros::Duration(3.0));
                    listener.lookupTransform("/robot1/odom", "/robot1/base_footprint",
                                             ros::Time(0), transform_odom2base);
                }
                else
                {
                    // listener.waitForTransform("/robot2/map", "/robot2/odom", ros::Time(0), ros::Duration(3.0));
                    listener.lookupTransform("/robot2/map", "/robot2/odom", ros::Time(0), transform_map2odom);
                    listener.lookupTransform("/robot2/odom", "/robot2/base_footprint", ros::Time(0), transform_odom2base);
                }
            }
            catch (tf::TransformException &ex)
            {
                ROS_ERROR("%s", ex.what());
                exit(1);
            }

            double x = transform_map2odom.getOrigin().x() + transform_odom2base.getOrigin().x();
            double y = transform_map2odom.getOrigin().y() + transform_odom2base.getOrigin().y();
            double z = transform_map2odom.getOrigin().z() + transform_odom2base.getOrigin().z();
            tf::Quaternion q = transform_map2odom.getRotation() + transform_odom2base.getRotation();

            double roll, pitch, yaw;
            tf::Matrix3x3(q).getRPY(roll, pitch, yaw);

            Eigen::Matrix<float, 4, 4> pose;
            pose << cos(roll) * cos(pitch), cos(roll) * sin(pitch) * sin(yaw) - sin(roll) * cos(yaw), cos(roll) * sin(pitch) * cos(yaw) + sin(roll) * sin(yaw), x,
                sin(roll) * cos(pitch), sin(roll) * sin(pitch) * sin(yaw) + cos(roll) * cos(yaw), sin(roll) * sin(pitch) * cos(yaw) - cos(roll) * sin(yaw), y,
                -sin(pitch), cos(pitch) * sin(yaw), cos(pitch) * cos(yaw), z,
                0, 0, 0, 1;

            int who_detect = data->who_detect;
            loop_info = data->index2value;
            std::vector<Eigen::Vector3d> kp_loc_r1_s;
            std::vector<Eigen::Vector3d> kp_loc_r2_s;

            for (size_t index{1}; index < data->r1.size() + 1; ++index)
            {
                // ３つ目の要素に差し掛かった時
                if (index % 3 == 0)
                {
                    Eigen::Vector3d kp_loc_r1(data->r1[index - 3], data->r1[index - 2], data->r1[index - 1]);
                    Eigen::Vector3d kp_loc_r2(data->r2[index - 3], data->r2[index - 2], data->r2[index - 1]);
                    kp_loc_r1_s.push_back(kp_loc_r1);
                    kp_loc_r2_s.push_back(kp_loc_r2);
                }
            }

            poses_1.push_back(pose);
            RO_Estimator::mlpnp(who_detect, kp_loc_r1_s, kp_loc_r2_s);
            // NOTE mlpnp
        }
        else
        {

            // NOTE BA
        }
    }
    // FIXME 前処理が必要
    opengv::transformation_t mlpnp(int who_detect, std::vector<Eigen::Vector3d> kp_loc_r1, std::vector<Eigen::Vector3d> kp_loc_r2)
    {
        // bearing vectors
        // 3Dポイント集
        std::cout << loop_info[0] << "->" << loop_info[1] << std::endl;
        opengv::points_t points;
        if (who_detect == 1)
            points = camera.img2cam(kp_loc_r1, kp_loc_r2);
        else
            points = camera.img2cam(kp_loc_r2, kp_loc_r1);

        opengv::bearingVectors_t bearingVectors;
        bearingVectors = camera.bearing_v();

        opengv::cov3_mats_t bearing_covs;
        bearing_covs = camera.v_cov();

        Eigen::MatrixXd cov_xx;
        Eigen::MatrixXd cov_ldld;

        opengv::absolute_pose::CentralAbsoluteAdapter adapter(bearingVectors,
                                                              points,
                                                              bearing_covs);

        size_t iterations = 50;
        opengv::transformation_t mlpnp_transformation;
        for (size_t i{0}; i < iterations; i++)
            mlpnp_transformation = opengv::absolute_pose::mlpnp(adapter, cov_xx, cov_ldld);

        std::cout << mlpnp_transformation << std::endl;
        // std::cout << "cov is..." << std::endl;
        // std::cout << cov_xx << std::endl;

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