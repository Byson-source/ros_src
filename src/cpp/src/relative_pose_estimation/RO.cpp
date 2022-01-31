
// TODO 各ノードのposeを受けとり、t=0どうしのR,tを逆算すること、また、共分散行列も
// TODO かさみ付き平均を適用して最適なR,tも求める
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
#include "nav_msgs/Path.h"
#include "geometry_msgs/PoseStamped.h"
#include "geometry_msgs/Point.h"

class RO_Estimator
{
private:
    ros::NodeHandle n;
    ros::Subscriber feature_sub;
    ros::Publisher rt_pub;
    ros::Publisher path_pub1;
    ros::Publisher path_pub2;
    ros::Subscriber path_sub1;
    // ros::Subscriber path_sub2;

    tf::TransformListener listener;

    std::vector<int> loop_info;
    // NOTE xyz / xyzw
    std::vector<std::vector<double>> path_1;
    std::vector<std::vector<double>> path_2;

    // NOTE node_map...{time;{1:[]}
    // points location

    Calibration camera;

public:
    RO_Estimator(void)
    {
        rt_pub = n.advertise<cpp::RO_Array>("RT_result", 50);
        path_pub1 = n.advertise<geometry_msgs::PoseStamped>("path_r1", 10);
        path_pub2 = n.advertise<geometry_msgs::PoseStamped>("path_r2", 10);

        feature_sub = n.subscribe("features", 20, &RO_Estimator::RO_CB, this);
        path_sub1 = n.subscribe("robot1/rtabmap/mapPath", 10, &RO_Estimator::path1_CB, this);
        // path_sub2 = n.subscribe("robot2/rtabmap/mapPath", 10, &RO_Estimator::path2_CB, this);
        // from robot/map to robot/base_footprint
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

        // std::vector<double> pose;
    }

    void RO_CB(const cpp::FeatureArray::ConstPtr &data)
    {
        if ((data->r1.size() > 27) && (data->signal == 0))
        // NOTE特徴点が10個以上ないとだめ
        {

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

            opengv::transformation_t result = RO_Estimator::mlpnp(who_detect, kp_loc_r1_s, kp_loc_r2_s);

            Eigen::Matrix3d R_ = result.block(0, 0, 3, 3);
            Eigen::Vector3d t_ = result.block(0, 3, 3, 1);

            cpp::RO_Array pose_result;
            Eigen::Vector3d ans_r = turnout_R(R_, who_detect);
            Eigen::Vector3d ans_t = turnout_T(t_, who_detect);

            std::vector<double> R, t;

            for (int index{0}; index < 3; ++index)
            {
                R.push_back(ans_r[index]);
                t.push_back(ans_t[index]);
            }
            pose_result.translation = R;
            pose_result.euler = t;

            rt_pub.publish(pose_result);
        }
        else
        {

            // NOTE BA
        }
    }

    // NOTE R_o2o'=R_o2a * R_a2b * R_b2o'
    Eigen::Vector3d turnout_R(Eigen::Matrix3d rotation_matrix, int who_detect)
    {
        Eigen::Quaterniond r1_q;
        Eigen::Quaterniond r2_q;

        int r1_img_index, r2_img_index;
        Eigen::Matrix3d rotation_1TO2;

        if (who_detect == 1)
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

        r1_q.x() = path_1[r1_img_index][3];
        r1_q.y() = path_1[r1_img_index][4];
        r1_q.z() = path_1[r1_img_index][5];
        r1_q.w() = path_1[r1_img_index][6];

        r2_q.x() = path_2[r2_img_index][3];
        r2_q.y() = path_2[r2_img_index][4];
        r2_q.z() = path_2[r2_img_index][5];
        r2_q.w() = path_2[r2_img_index][6];

        Eigen::Matrix3d R1_origin2node = r1_q.normalized().toRotationMatrix();
        Eigen::Matrix3d R2_origin2node = r2_q.normalized().toRotationMatrix();
        Eigen::Vector3d ans_R = (R1_origin2node * rotation_1TO2 * (R2_origin2node.transpose())).eulerAngles(2, 1, 0);
        ans_R << ans_R.z(), ans_R.y(), ans_R.x();
        return ans_R;
    }

    Eigen::Vector3d turnout_T(Eigen::Vector3d transfer, int who_detect)
    {
        Eigen::Vector3d r1_to_r2;
        Eigen::Vector3d origin2r1;
        Eigen::Vector3d origin2r2;

        int r1_img_index, r2_img_index;
        Eigen::Vector3d transfer_1_to_2;
        if (who_detect == 1)
        {
            r1_img_index = loop_info[0];
            r2_img_index = loop_info[1];
            transfer_1_to_2 = transfer;
        }
        else
        {
            r1_img_index = loop_info[1];
            r2_img_index = loop_info[0];
            transfer_1_to_2 = (-1) * transfer;
        }

        origin2r1 << path_1[r1_img_index][0], path_1[r1_img_index][1], path_1[r1_img_index][2];
        origin2r2 << path_2[r2_img_index][0], path_2[r2_img_index][1], path_2[r2_img_index][2];

        Eigen::Vector3d ans_t = transfer_1_to_2 - (origin2r2 - origin2r1);
        return ans_t;
    }
    // FIXME 前処理が必要
    opengv::transformation_t
    mlpnp(int who_detect, std::vector<Eigen::Vector3d> kp_loc_r1, std::vector<Eigen::Vector3d> kp_loc_r2)
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