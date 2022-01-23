#ifndef CALIBRATION_H
#define CALIBRATION_H
#include <iostream>
#include <vector>
#include <Eigen/Core>
#include <Eigen/Geometry>

#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/eigen.hpp>
#include <opengv/absolute_pose/methods.hpp>
#include <opengv/math/cayley.hpp>
#include "experiment_helpers.hpp"
#include "random_generators.hpp"
#include <opengv/math/cayley.hpp>

class Calibration
{
private:
    Eigen::Matrix3d intrinstic_parameter;
    Eigen::Matrix3d inv_intrinstic;
    Eigen::Matrix3d img_to_cam_coordinate;

    opengv::points_t X_s;
    std::vector<Eigen::Vector3d> cam_coords;

    double focal{525.0};
    double height{480};
    double width{640};

    cv::Point2d pp;

    // bearing vectors
    opengv::bearingVectors_t bearingVectors;
    // bearing vectorの深く不確実性のリスト
    opengv::cov3_mats_t vcovs;
    // 画像上の不確実性
    opengv::cov3_mat_t image_cov;
    // カメラ座標での不確実性
    opengv::cov3_mat_t observe_cam_cov;
    // FIXME

public:
    Calibration(void)
    {
        // 全ての画像座標を格納
        // NOTE this is pixel size
        intrinstic_parameter << 525.0, 0.0, 319.5,
            0.0, 525.0, 239.5,
            0.0, 0.0, 1.0;
        inv_intrinstic = intrinstic_parameter.inverse();

        img_to_cam_coordinate << 0.0, 0.0, 1.0,
            -1.0, 0.0, 0.0,
            0.0, -1.0, 0.0;
        // NOTE gazeboと全く同じ右手座標系

        image_cov = Eigen::Matrix3d::Identity();
        // FIXME covを決めること!
        observe_cam_cov = inv_intrinstic * image_cov * inv_intrinstic.transpose();
    }

    opengv::points_t img2cam(std::vector<Eigen::Vector3d> kp_loc,
                             std::vector<Eigen::Vector3d> kp_loc_other)
    {
        // 世界座標から見た点の座標を求める
        X_s.clear();
        bearingVectors.clear();
        for (long unsigned int i{0}; i < kp_loc.size(); ++i)
        {
            Eigen::Vector3d X(kp_loc[i].x() - width / 2, kp_loc[i].y() - height / 2, kp_loc[i].z() / 1000);
            Eigen::Vector3d pixel_(kp_loc_other[i].x() - width / 2, kp_loc_other[i].y() - height / 2, kp_loc_other[i].z() / 1000);

            X = img_to_cam_coordinate * X;
            pixel_ = img_to_cam_coordinate * pixel_;

            X.y() = X.x() / focal * X.y();
            X.z() = X.x() / focal * X.z();

            pixel_.y() = pixel_.x() / focal * pixel_.y();
            pixel_.z() = pixel_.x() / focal * pixel_.z();
            Eigen::Vector3d cam_coord;
            cam_coord = pixel_;
            // bearing vector
            pixel_ = cam_coord.normalized();

            X_s.push_back(X);
            // for bearing vector
            bearingVectors.push_back(pixel_);
            cam_coords.push_back(cam_coord);
        }
        return X_s;
    }
    // FIXME

    opengv::bearingVectors_t bearing_v(void)
    {

        return bearingVectors;
    }

    // bearing vectorの共分散!

    opengv::cov3_mats_t v_cov(void)
    {
        vcovs.clear();
        for (long unsigned int i{0}; i < cam_coords.size(); ++i)
        {
            Eigen::Matrix3d jacobian = 1 / cam_coords[i].norm() * (Eigen::Matrix3d::Identity() - bearingVectors[i] * bearingVectors[i].transpose());
            opengv::cov3_mat_t vcov = jacobian * observe_cam_cov * jacobian.transpose();
            // ↑論文参照！
            vcovs.push_back(vcov);
        }
        return vcovs;
    }
};
#endif