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
#include <librealsense2/rs.hpp>

class Calibration
{
private:
    rs2_intrinsics intr;

    // float coeff[5];

    Eigen::Matrix3d intrinstic_parameter;
    Eigen::Matrix3d inv_intrinstic;
    Eigen::Matrix3d img_to_cam_coordinate;

    opengv::points_t X_s;
    std::vector<Eigen::Vector3d> cam_coords;

    double focal{617.4};
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
        for (int i{0}; i < 5; ++i)
            intr.coeffs[i] = 0;
        intr.fx = 617.56042;
        intr.fy = 617.3798828;
        intr.height = 480;
        intr.width = 640;
        intr.ppx = 317.55502;
        intr.ppy = 244.730865;
        // 全ての画像座標を格納
        // NOTE this is pixel size

        intrinstic_parameter << 617.5604248046, 0.0, 317.55502,
            0.0, 617.3798828, 244.730865,
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
        // kp_loc = kp_loc.cast<float>();
        // kp_loc_other = kp_loc_other.cast<float>();
        for (long unsigned int i{0}; i < kp_loc.size(); ++i)
        {
            float upixel[2]{kp_loc[i].cast<float>().x(), kp_loc[i].cast<float>().y()};
            float upoint[3]; // From point (in 3D)

            float vpixel[2]{kp_loc_other[i].cast<float>().x(), kp_loc_other[i].cast<float>().y()}; // To pixel
            float vpoint[3];

            rs2_deproject_pixel_to_point(upoint, &intr, upixel, kp_loc[i].cast<float>().z());
            rs2_deproject_pixel_to_point(vpoint, &intr, vpixel, kp_loc_other[i].cast<float>().z());

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