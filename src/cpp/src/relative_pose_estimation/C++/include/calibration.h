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

class Calibration
{
private:
    // 画像座標
    std::vector<Eigen::Vector3d> pixels_1, pixels_2;

    Eigen::Matrix3d intrinstic_parameter;
    Eigen::Matrix3d inv_intrinstic;
    double focal;
    cv::Point2d pp;

    // bearing vectors
    std::vector<Eigen::Vector3d> v1_s, v2_s;
    // bearing-covariance
    Eigen::Matrix3d observe_cov;
    std::vector<Eigen::Matrix3d> covs_1;
    std::vector<Eigen::Matrix3d> covs_2;

public:
    // Calibration(std::vector<cv::Point2f>　kp1_loc, std::vector<cv::Point2f> kp2_loc,
    //             double focal_val, cv::Point2d pp_val,
    //             std::vector<double> depth_1, std::vector<double> depth_2) : focal{focal_val}, pp{pp_val}
    Calibration(double focal_val,cv::Point2d pp_val): focal{focal_val}, pp{pp_val}
    {
        // 全ての画像座標を格納
        
        intrinstic_parameter << focal, 0.0, pp.x,
            0.0, focal, pp.y,
            0.0, 0.0, 1.0;
        inv_intrinstic = intrinstic_parameter.inverse();
    }
    std::vector<Eigen::Vector3d> img2cam(std::vector<cv::Point2f>　kp1_loc, 
    std::vector<cv::Point2f> kp2_loc,
    std::vector<double> depth_1, std::vector<double> depth_2)
    {
        pixels_1.clear();pixels_2.clear();X1_s.clear();X2_s.clear();
        for (int i{0}; i < kp1_loc.size(); ++i)
        {
            Eigen::Vector2d pix1, pix2;
            Eigen::Vector3d pixel_1, pixel_2;
            cv::cv2eigen(kp1_loc[i], pix1);
            cv::cv2eigen(kp2_loc[i], pix2);

            pixel_1[0] = pix1[0];
            pixel_1[1] = pix1[1];
            pixel_2[0] = pix2[0];
            pixel_2[1] = pix2[1];
            pixel_1[2], pixel_2[2] = depth_1[i], depth_2[i];
            // Homogeneous coordinate
            pixels_1.push_back(pixel_1);
            pixels_2.push_back(pixel_2);
        }

        std::vector<Eigen::Vector3d> X1_s, X2_s;
        for (int index{0}; index < pixels_1.size(); ++index)
        {
            Eigen::Vector3d X1, X2;
            X1, X2 = inv_intrinstic.dot(pixels_1[index]), inv_intrinstic.dot(pixels_2[index]);
            X1_s.push_back(X1);
            X2_s.push_back(X2);
        }
        return X1_s, X2_s;
    }

    std::vector<Eigen::Vector3d> bearing_v(void)
    {
        v1_s.clear();v2_s.clear();
        for (int indice{0}; indice < pixels_1.size(); ++indice)
        {
            Eigen::Vector3d v1, v2;
            v1.z, v2.z = 1, 1;
            v1.x, v2.x = pixels_1[indice].x / pixels_1[indice].z, pixels_2[indice].x / pixels_2[indice].z;
            v1.y, v2.y = pixels_1[indice].y / pixels_1[indice].z, pixels_2[indice].y / pixels_2[indice].z;
            v1_s.push_back(v1.normalized());
            v2_s.push_back(v2.normalized());
        }
        return v1_s, v2_s;
    }
    // bearing vectorの共分散!
    std::vector<Eigen::Matrix3d> v_cov(void)
    {
        covs_1.clear();covs_2.clear();
        for (int i{0}; i < pixels_1.size(); ++i)
        {
            // Eigen::Matrix3d Xcov = inv_intrinstic.dot(observe_cov).dot(inv_intrinstic.transpose());
            Eigen::Matrix3d vcov_1 = 1 / pixels_1[i].norm() * (Eigen::Matrix3d::Identity() - v1_s[i].dot(v1_s[i].transpose()));
            Eigen::Matrix3d vcov_2 = 1 / pixels_2[i].norm() * (Eigen::Matrix3d::Identity() - v2_s[i].dot(v2_s[i].transpose()));
            covs_1.push_back(vcov_1);covs_2.push_back(vcov_2);
        }
        return vcov_1,vcov_2;
    }
};
#endif