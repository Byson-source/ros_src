
// TODO 各ノードのposeを受けとり、t=0どうしのR,tを逆算すること、また、共分散行列も
// TODO かさみ付き平均を適用して最適なR,tも求める
#include <iostream>
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
// #include "calibration.h"
// FIXME CMakeLists.txtを修正
#include <tf/transform_listener.h>
#include "nav_msgs/Path.h"
#include "geometry_msgs/PoseStamped.h"
#include "geometry_msgs/Point.h"
#include <cpp/HomogeneousArray.h>

class RO_Estimator
{
private:
    ros::NodeHandle n;
    ros::Subscriber feature_sub;
    ros::Subscriber path_sub1;
    ros::Subscriber path_sub2;
    ros::Subscriber transformation_sub;
    ros::Publisher BA_pub;
    ros::Publisher rt_pub;

    tf::TransformListener listener;

    std::vector<int> loop_info;
    // NOTE xyz / xyzw
    std::vector<std::vector<double>> path_1;
    std::vector<std::vector<double>> path_2;

    std::map<int, int> mapPath_dict;
    std::map<int, int> mapPath_dict_2;
    std::map<std::string, std::map<std::string, std::vector<Eigen::Vector3d>>> feature_dict;
    // NOTE {"R1":{"3d":[[x1,y1,z1],[x2,y2...]],"2d":[[x1,y1,0],[x2,y2,0],....]}}...}
    int info_index{0};
    int info_index_2{0};

    Eigen::Matrix4d transformation_result;
    // Eigen::Matrix3d intrinsic_parameter;
    // Eigen::Matrix4d img_to_cam_coordinate;
    // cv::Mat camera_parameter;

    // NOTE node_map...{time;{1:[]}
    // points location

public:
    RO_Estimator(void)
    {
        rt_pub = n.advertise<cpp::RO_Array>("RT_result", 50);
        // BA_pub = n.advertise

        feature_sub = n.subscribe("features", 20, &RO_Estimator::RO_CB, this);
        path_sub1 = n.subscribe("robot1/rtabmap/mapPath", 10, &RO_Estimator::path1_CB, this);
        path_sub2 = n.subscribe("robot2/rtabmap/mapPath", 10, &RO_Estimator::path2_CB, this);
        transformation_sub = n.subscribe("odometry_result", 10, &RO_Estimator::odom_CB, this);
        // from robot/map to robot/base_footprint
        // intrinsic_parameter
        //     << 617.5604248046,
        //     0.0, 317.55502,
        //     0.0, 617.3798828, 244.730865,
        //     0.0, 0.0, 1.0;

        std::map<std::string, std::vector<Eigen::Vector3d>> correspondence_2d3d;
        feature_dict["R1"] = correspondence_2d3d;
        feature_dict["R2"] = correspondence_2d3d;

        // img_to_cam_coordinate << 0.0, 0.0, 1.0,
        //     -1.0, 0.0, 0.0,
        //     0.0, -1.0, 0.0;
    }

    void path1_CB(const nav_msgs::Path::ConstPtr &path)
    {
        path_1.clear();
        for (auto val : path->poses)
        {
            std::vector<double> pose;
            pose.push_back(val.pose.position.x);
            pose.push_back(val.pose.position.y);
            pose.push_back(val.pose.position.z);
            pose.push_back(val.pose.orientation.x);
            pose.push_back(val.pose.orientation.y);
            pose.push_back(val.pose.orientation.z);
            pose.push_back(val.pose.orientation.w);
            path_1.push_back(pose);
        }
        info_index += 1;
        mapPath_dict[info_index] = path_1.size() - 1;

        // std::vector<double> pose;
    }

    void path2_CB(const nav_msgs::Path::ConstPtr &path)
    {
        path_2.clear();
        for (auto val : path->poses)
        {
            std::vector<double> pose;
            pose.push_back(val.pose.position.x);
            pose.push_back(val.pose.position.y);
            pose.push_back(val.pose.position.z);
            pose.push_back(val.pose.orientation.x);
            pose.push_back(val.pose.orientation.y);
            pose.push_back(val.pose.orientation.z);
            pose.push_back(val.pose.orientation.w);
            path_2.push_back(pose);
        }
        info_index_2 += 1;
        mapPath_dict_2[info_index_2] = path_2.size() - 1;

        // std::vector<double> pose;
    }

    void odom_CB(const cpp::HomogeneousArray::ConstPtr &data)
    {
        int number{0};
        for (int row{0}; row < 4; ++row)
        {
            for (int column{0}; column < 4; ++column)
            {
                number += 1;
                transformation_result(row, column) = data->data[number];
            }
        }

        Eigen::Vector3d transfer_;
        Eigen::Matrix3d rotation_;
        transfer_ = transformation_result.block(3, 0, 3, 1);
        rotation_ = transformation_result.block(0, 0, 3, 3);

        cpp::RO_Array pose_result;
        int who_detect = data->who_detect;
        Eigen::Vector3d ans_t = turnout_T(transfer_, who_detect);
        Eigen::Quaterniond ans_r = turnout_R(rotation_, who_detect);

        std::vector<double> R{ans_r.w(), ans_r.x(), ans_r.y(), ans_r.z()};
        std::vector<double> t{ans_t[0], ans_t[1], ans_t[2]};

        pose_result.translation = t;
        pose_result.euler = R;

        rt_pub.publish(pose_result);
    }

    void RO_CB(const cpp::FeatureArray::ConstPtr &data)
    {
        if ((data->r1.size() > 30) && (data->signal == 0))
        // NOTE特徴点が10個以上ないとだめ
        {

            int who_detect = data->who_detect;
            loop_info = data->index2value;
            std::vector<Eigen::Vector3d> img_coord_1;
            std::vector<Eigen::Vector3d> img_coord_2;

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
                    // NOTE ポイントのカメラ座標
                    Eigen::Vector3d r1_coord(data->r1_imgcoord[index - 3], data->r1_imgcoord[index - 2], 0);
                    Eigen::Vector3d r2_coord(data->r2_imgcoord[index - 3], data->r2_imgcoord[index - 2], 0);
                    img_coord_1.push_back(r1_coord);
                    img_coord_2.push_back(r2_coord);
                }
            }

            // if (who_detect == 1)
            // {
            //     feature_dict["R1"]["3d"].push_back(kp_loc_)
            // }

            // if (who_detect == 1)
            //     result = RO_Estimator::pnp(kp_loc_r1_s, img_coord_2);
            // else
            //     result = RO_Estimator::pnp(kp_loc_r2_s, img_coord_1);

            // Eigen::Matrix4   d result = RO_Estimator::pnp(who_detect, kp_loc_r1_s, kp_loc_r2_s);

            // Eigen::Matrix3d R_ = result.block(0, 0, 3, 3);
            // Eigen::Vector3d rpy = R_.eulerAngles(0, 1, 2);
            // Eigen::Vector3d t_ = result.block(0, 3, 3, 1);
            // std::cout << "=============================" << std::endl;
            // std::cout << t_ << std::endl;
            // std::cout << std::endl;

            // cpp::RO_Array pose_result;
            // Eigen::Vector3d ans_t = turnout_T(t_, who_detect);
            // Eigen::Quaterniond ans_r = turnout_R(R_, who_detect);

            // std::vector<double> R{ans_r.w(), ans_r.x(), ans_r.y(), ans_r.z()};
            // std::vector<double> t{ans_t[0], ans_t[1], ans_t[2]};

            // pose_result.translation = t;
            // pose_result.euler = R;

            // rt_pub.publish(pose_result);
        }
        else
        {

            // NOTE BA
        }
    }

    Eigen::Vector3d turnout_T(Eigen::Vector3d transfer, int who_is_detect)
    {
        Eigen::Vector3d r1_to_r2;
        Eigen::Vector3d origin2r1;
        Eigen::Vector3d origin2r2;

        int r1_img_index, r2_img_index;
        Eigen::Vector3d transfer_1_to_2;
        if (who_is_detect == 1)
        {
            r1_img_index = loop_info[0];
            r2_img_index = loop_info[1];
            transfer_1_to_2 = transfer;
        }
        else
        {
            r1_img_index = loop_info[1];
            r2_img_index = loop_info[0];
            transfer_1_to_2 = Eigen::Vector3d::Ones() - transfer;
        }

        origin2r1 << path_1[mapPath_dict[r1_img_index]][0], path_1[mapPath_dict[r1_img_index]][1], path_1[mapPath_dict[r1_img_index]][2];
        origin2r2 << path_2[mapPath_dict_2[r2_img_index]][0], path_2[mapPath_dict_2[r2_img_index]][1], path_2[mapPath_dict_2[r2_img_index]][2];

        Eigen::Vector3d ans_t = transfer_1_to_2 - (origin2r2 - origin2r1);
        return ans_t;
    }

    // NOTE R_o2o'=R_o2a * R_a2b * R_b2o'
    Eigen::Quaterniond turnout_R(Eigen::Matrix3d rotation_matrix, int who_is_detect)
    {
        Eigen::Quaterniond r1_q;
        Eigen::Quaterniond r2_q;

        int r1_img_index, r2_img_index;
        Eigen::Quaterniond rotation_1TO2;

        if (who_is_detect == 1)
        {
            rotation_1TO2 = rotation_matrix;
            r1_img_index = loop_info[0];
            r2_img_index = loop_info[1];
        }
        else
        {
            rotation_1TO2 = rotation_matrix.transpose();
            r1_img_index = loop_info[1];
            r2_img_index = loop_info[0];
        }

        r1_q.x() = path_1[mapPath_dict[r1_img_index]][3];
        r1_q.y() = path_1[mapPath_dict[r1_img_index]][4];
        r1_q.z() = path_1[mapPath_dict[r1_img_index]][5];
        r1_q.w() = path_1[mapPath_dict[r1_img_index]][6];

        r2_q.x() = path_2[mapPath_dict_2[r2_img_index]][3];
        r2_q.y() = path_2[mapPath_dict_2[r2_img_index]][4];
        r2_q.z() = path_2[mapPath_dict_2[r2_img_index]][5];
        r2_q.w() = path_2[mapPath_dict_2[r2_img_index]][6];

        // NOTE クオータニオンの掛け算の方向は左側
        // Eigen::Matrix3d R2_origin2node = (r2_q.normalized()).toRotationMatrix();
        Eigen::Quaterniond q_r1_to_r2 = (r2_q.inverse()) * rotation_1TO2 * r1_q;
        return q_r1_to_r2.normalized();
    }

    // FIXME 前処理が必要
    // Eigen::Matrix4d pnp(std::vector<cv::Point3d> kp_loc,
    //                     std::vector<cv::Point2d> coord)
    // {
    //     // bearing vectors
    //     // 3Dポイント集
    //     std::cout << loop_info[0] << "->" << loop_info[1] << std::endl;

    //     cv::Mat rotation_vector;
    //     cv::Mat translation_vector;
    //     cv::Mat dist_coeffs = cv::Mat::zeros(4, 1, cv::DataType<double>::type);

    //     cv::solvePnP(kp_loc, coord, camera_parameter, dist_coeffs, rotation_vector, translation_vector);

    //     Eigen::Vector3d translation;
    //     Eigen::Matrix3d rotation;
    //     cv::cv2eigen(rotation_vector, rotation);
    //     cv::cv2eigen(translation_vector, translation);

    //     rotation = rotation.transpose();
    //     translation = rotation * (-translation);

    //     Eigen::Matrix4d transformation;
    //     transformation.block(0, 0, 3, 3) = rotation;
    //     transformation.block(0, 3, 3, 1) = translation;

    //     return transformation;
    // }
};

int main(int argc, char **argv)
{
    ros::init(argc, argv, "RO_estimator");
    RO_Estimator detector;

    ros::spin();

    return 0;
}