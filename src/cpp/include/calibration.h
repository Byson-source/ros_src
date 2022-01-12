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

    std::vector<Eigen::Vector3d> X_s;
    std::vector<Eigen::Vector3d> pixels

        double focal;
    cv::Point2d pp;

    // bearing vectors
    std::vector<Eigen::Vector3d> v_s;
    // bearing-covariance
    Eigen::Matrix3d observe_cov;
    std::vector<Eigen::Matrix3d> covs;
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
    }

    std::vector<Eigen::Vector3d> img2cam(std::vector<cv::Point2f> kp_loc,
                                         std::vector<double> depth)
    {
        X_s.clear();
        pixels.clear();
        for (int i{0}; i < kp_loc.size(); ++i)
        {
            Eigen::Vector2d pix;
            Eigen::Vector3d pixel;
            cv::cv2eigen(kp_loc[i], pix);

            pixel[0] = pix[0];
            pixel[1] = pix[1];
            pixel[2] = depth[i];
            // Homogeneous coordinate
            Eigen::Vector3d X;
            X = inv_intrinstic.dot(pixel);
            X_s.push_back(X);
            pixels.push_back(pixel)
        }
        return X_s;
    }

    std::vector<Eigen::Vector3d> bearing_v(void)
    {
        v_s.clear();
        for (int indice{0}; indice < pixels.size(); ++indice)
        {
            Eigen::Vector3d v;
            v.z= 1;
            v.x= pixels[indice].x / pixels[indice].z, pixels[indice].x / pixels[indice].z;
            v.y= pixels[indice].y / pixels[indice].z, pixels[indice].y / pixels[indice].z;
            v_s.push_back(v.normalized());
        }
        return v_s;
    }
    // bearing vectorの共分散!
    std::vector<Eigen::Matrix3d> v_cov(void)
    {
        covs_1.clear();
        covs_2.clear();
        for (int i{0}; i < pixels_1.size(); ++i)
        {
            // Eigen::Matrix3d Xcov = inv_intrinstic.dot(observe_cov).dot(inv_intrinstic.transpose());
            Eigen::Matrix3d vcov_1 = 1 / pixels_1[i].norm() * (Eigen::Matrix3d::Identity() - v1_s[i].dot(v1_s[i].transpose()));
            Eigen::Matrix3d vcov_2 = 1 / pixels_2[i].norm() * (Eigen::Matrix3d::Identity() - v2_s[i].dot(v2_s[i].transpose()));
            covs_1.push_back(vcov_1);
            covs_2.push_back(vcov_2);
        }
        return vcov_1, vcov_2;
    }
};
#endif