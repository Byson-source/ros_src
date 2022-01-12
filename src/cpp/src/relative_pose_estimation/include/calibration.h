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

    opengv::points_t X_s;
    std::vector<Eigen::Vector3d> pixels;

    double focal;

    cv::Point2d pp;

    // bearing vectors
    opengv::bearingVectors_t v_s;
    // bearing vectorの深く不確実性のリスト
    opengv::cov3_mats_t vcovs;
    // 画像上の不確実性
    opengv::cov3_mat_t image_cov;
    // カメラ座標での不確実性
    opengv::cov3_mat_t observe_cam_cov;

    // std::vector<Eigen::Matrix3d> covs_2;

public:
    // Calibration(std::vector<cv::Point2f>　kp1_loc, std::vector<cv::Point2f> kp2_loc,
    //             double focal_val, cv::Point2d pp_val,
    //             std::vector<double> depth_1, std::vector<double> depth_2) : focal{focal_val}, pp{pp_val}
    Calibration(double focal_val, cv::Point2d pp_val) : focal{focal_val}, pp{pp_val}
    {
        // 全ての画像座標を格納

        intrinstic_parameter << focal, 0.0, pp.x,
            0.0, focal, pp.y,
            0.0, 0.0, 1.0;
        inv_intrinstic = intrinstic_parameter.inverse();

        image_cov = Eigen::Matrix3d::Identity();
        observe_cam_cov = inv_intrinstic * image_cov * inv_intrinstic.transpose();
    }

    opengv::points_t img2cam(std::vector<cv::Point3f> kp_loc,
                             std::vector<cv::Point3f> kp_loc_other)
    {
        X_s.clear();
        pixels.clear();
        for (int i{0}; i < kp_loc.size(); ++i)
        {
            Eigen::Vector3d pix;
            cv::cv2eigen(kp_loc[i], pix);
            // Homogeneous coordinate
            Eigen::Vector3d X;
            X = inv_intrinstic * pix;
            X_s.push_back(X);
        }
        pixels = kp_loc_other;
        return X_s;
    }

    opengv::bearingVectors_t bearing_v(void)
    {
        v_s.clear();
        for (int indice{0}; indice < pixels.size(); ++indice)
        {
            Eigen::Vector3d v;
            v.z = 1;
            v.x = pixels[indice].x / pixels[indice].z;
            v.y = pixels[indice].y / pixels[indice].z;
            // ↑x（MLPNPの論文を参照！！)
            v_s.push_back(v.normalized());
        }
        return v_s;
    }

    // bearing vectorの共分散!

    opengv::cov3_mats_t v_cov(void)
    {
        covs.clear();
        for (int i{0}; i < pixels.size(); ++i)
        {
            Eigen::Matrix3d jacobian = 1 / pixels[i].norm() * (Eigen::Matrix3d::Identity() - v_s[i] * v_s[i].transpose());
            opengv::cov3_mat_t vcov = jacobian * observe_cam_cov * jacobian.transpose();
            // ↑論文参照！
            vcovs.push_back(vcov);
        }
        return vcovs
    }
};
#endif