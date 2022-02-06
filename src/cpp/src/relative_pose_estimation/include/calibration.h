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

class Calibration
{
private:
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
        for (long unsigned int i{0}; i < kp_loc.size(); ++i)
        {
            opengv::point_t X, pixel_;

            X = img_to_cam_coordinate * kp_loc[i];
            pixel_ = img_to_cam_coordinate * kp_loc_other[i];

            Eigen::Vector3d cam_coord;
            cam_coord = pixel_;
            // bearing vector
            pixel_ = cam_coord.normalized();
            X_s.push_back(X);
            // for bearing vector
            bearingVectors.push_back(pixel_);
            cam_coords.push_back(cam_coord);

            if (i < 3)
            {
                std::cout << X << std::endl;
                std::cout << cam_coord << std::endl;
            }
        }
        return X_s;
    }
    // FIXME
};
#endif