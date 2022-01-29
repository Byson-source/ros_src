
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

    void path1_CB(const nav_msgs::Path::ConstPtr &path)
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

            std::vector<double> R, t;
            pose_result.translation = t;
            pose_result.euler = R;

            rt_pub.publish(pose_result);
        }
        else
        {

            // NOTE BA
        }
    }

    // NOTE R_o2o'=R_o2a * R_a2b * R_b2o'
    Eigen::Matrix3d turnout_R(Eigen::Matrix3d rotation_matrix int, who_detect)
    {
        Eigen::Quaterniond index_q;
        Eigen::Quaterniond value_q;

        std::vector<std::vector<double>> index_path;
        std::vector<std::vector<double>> value_path;

        if (who_detect == 1)
        {
            index_path = path_1;
            value_path = path_2;
        }
        else
        {
            index_path = path_2;
            value_path = path_1;
        }

        index_q.x() = index_path[loop_info[0]][3];
        index_q.y() = index_path[loop_info[0]][4];
        index_q.z() = index_path[loop_info[0]][5];
        index_q.w() = index_path[loop_info[0]][6];

        value_q.x() = value_path[loop_info[1]][3];
        value_q.y() = value_path[loop_info[1]][4];
        value_q.z() = value_path[loop_info[1]][5];
        value_q.w() = value_path[loop_info[1]][6];

        Eigen::Matrix3d R_index = index_q.normalized().toRotationMatrix();
        Eigen::Matrix3d R_value = value_q.normalized().toRotationMatrix();

        index_q.x() = path_1
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