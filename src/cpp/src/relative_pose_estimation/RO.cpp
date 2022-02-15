// TODO odometryと特徴点座標をmapに格納して整理
// TODO オドメトリ検出点付近の機体のposeを算出する関数の作成
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

#include <visualization_msgs/Marker.h>

#include <ros/ros.h>
#include <string>
#include <vector>
#include "cpp/FeatureArray.h"
#include "cpp/RO_Array.h"
#include "rtabmap_ros/MapGraph.h"
#include <math.h>
// #include "calibration.h"
// FIXME CMakeLists.txtを修正
#include "nav_msgs/Path.h"
#include "geometry_msgs/PoseStamped.h"
#include "geometry_msgs/Point.h"
#include <cpp/HomogeneousArray.h>

#include <gtsam/geometry/Point2.h>
#include <gtsam/geometry/Pose2.h>
#include <gtsam/inference/Symbol.h>
#include <gtsam/slam/ProjectionFactor.h>
#include <gtsam/slam/BetweenFactor.h>
#include <gtsam/sam/BearingRangeFactor.h>
#include <gtsam/nonlinear/NonlinearFactorGraph.h>
#include <gtsam/nonlinear/DoglegOptimizer.h>
#include <gtsam/nonlinear/LevenbergMarquardtOptimizer.h>
#include <gtsam/nonlinear/Marginals.h>
#include <gtsam/nonlinear/Values.h>

using namespace gtsam;

class RO_Estimator
{
private:
    Cal3_S2::shared_ptr K{new Cal3_S2(617.5604248046, 617.3798828, 0.0, 317.55502, 244.730865)};

    ros::NodeHandle n;
    ros::Subscriber feature_sub;
    ros::Subscriber path_sub1;
    ros::Subscriber path_sub2;
    ros::Subscriber info_sub1;
    ros::Subscriber info_sub2;
    ros::Subscriber transformation_sub;

    ros::Publisher BA_pub;
    ros::Publisher rt_pub;
    ros::Publisher marker_pub;

    visualization_msgs::Marker points, line_strip, line_list;

    std::vector<int> loop_info;
    // NOTE xyz / xyzw
    std::vector<std::vector<double>> path_1;
    std::vector<std::vector<double>> path_2;
    std::vector<int> valids;

    std::map<int, int> mapPath_dict;
    std::map<int, int> mapPath_dict_2;
    std::map<std::vector<int>, Eigen::VectorXd> info_dict1;
    std::map<std::vector<int>, Eigen::VectorXd> info_dict2;

    std::map<std::vector<int>, Eigen::Matrix4d> odom_dict1;
    std::map<std::vector<int>, Eigen::Matrix4d> odom_dict2;
    // NOTE {"R1":{1:[[x1,y1,z1],[x2,y2...]],2:...},"R2":...}

    int info_index{0};
    int info_index_2{0};

    Eigen::Matrix4d transformation_result;
    Eigen::Matrix4d cam2robot;

    Eigen::Matrix3d draw_rotation;
    Eigen::Vector3d draw_t;

    // NOTE node_map...{time;{1:[]}
    // points location

public:
    // NOTE コンストラクタ、各種設定
    RO_Estimator(void)
    {
        rt_pub = n.advertise<cpp::RO_Array>("RT_result", 50);
        // BA_pub = n.advertise

        // feature_sub = n.subscribe("features", 20, &RO_Estimator::RO_CB, this);
        path_sub1 = n.subscribe("robot1/rtabmap/mapPath", 10, &RO_Estimator::path1_CB, this);
        path_sub2 = n.subscribe("robot2/rtabmap/mapPath", 10, &RO_Estimator::path2_CB, this);
        info_sub1 = n.subscribe("robot1/rtabmap/mapGraph", 10, &RO_Estimator::info_CB1, this);
        info_sub2 = n.subscribe("robot2/rtabmap/mapGraph", 10, &RO_Estimator::info_CB2, this);
        transformation_sub = n.subscribe("odometry_result", 1000, &RO_Estimator::odom_CB, this);

        cam2robot << 0.0, 0.0, 1.0, 0.0,
            -1.0, 0.0, -1.0, 0.0,
            0.0, -1.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 1.0;
    }
    // FIXME odomの拘束条件しか考慮しない
    void info_CB1(const rtabmap_ros::MapGraph::ConstPtr &data)
    {
        info_dict1.clear();
        odom_dict1.clear();
        for (auto each_link : data->links)
        {
            if (each_link.fromId + 1 == each_link.toId)
            {
                std::vector<int> fromto{each_link.fromId, each_link.toId};
                Eigen::VectorXd infos(6);
                infos << each_link.information[21], each_link.information[28], each_link.information[35],
                    each_link.information[0], each_link.information[7], each_link.information[14];

                Eigen::Translation3d odom_translation(each_link.transform.translation.x, each_link.transform.translation.y, each_link.transform.translation.z);
                Eigen::Quaterniond odom_orientation(each_link.transform.rotation.w, each_link.transform.rotation.x, each_link.transform.rotation.y, each_link.transform.rotation.z);

                Eigen::Affine3d transfer_affine;
                transfer_affine = odom_translation * odom_orientation;

                info_dict1[fromto] = infos;
                odom_dict1[fromto] = transfer_affine.matrix();
            }
        }
    }
    // FIXME odomの拘束条件しか考慮しない

    void info_CB2(const rtabmap_ros::MapGraph::ConstPtr &data)
    {
        info_dict2.clear();
        odom_dict2.clear();
        for (auto each_link : data->links)
        {
            if (each_link.fromId + 1 == each_link.toId)
            {
                std::vector<int> fromto{each_link.fromId, each_link.toId};
                Eigen::Translation3d odom_translation(each_link.transform.translation.x, each_link.transform.translation.y, each_link.transform.translation.z);
                Eigen::Quaterniond odom_orientation(each_link.transform.rotation.w, each_link.transform.rotation.x, each_link.transform.rotation.y, each_link.transform.rotation.z);

                Eigen::Affine3d transfer_affine;
                transfer_affine = odom_translation * odom_orientation;

                Eigen::VectorXd infos(6);
                infos << each_link.information[21], each_link.information[28], each_link.information[35],
                    each_link.information[0], each_link.information[7], each_link.information[14];
                info_dict2[fromto] = infos;
                odom_dict2[fromto] = transfer_affine.matrix();
            }
        }
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
        mapPath_dict[info_index] = path_1.size() - 1;
        info_index += 1;

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
        mapPath_dict_2[info_index_2] = path_2.size() - 1;
        info_index_2 += 1;
    }

    // NOTE feature_matcherからrgbd odometryの計算結果をsubscribe
    void odom_CB(const cpp::HomogeneousArray::ConstPtr &data_)
    {

        transformation_result << data_->data[0], data_->data[1], data_->data[2], data_->data[3],
            data_->data[4], data_->data[5], data_->data[6], data_->data[7],
            data_->data[8], data_->data[9], data_->data[10], data_->data[11],
            0.0, 0.0, 0.0, 1.0;
        valids = data_->valid_img;
        std::string who_detect = data_->who_detect;
        std::string another_one;
        if (who_detect == "R1")
            another_one = "R2";
        else
            another_one = "R1";

        transformation_result = cam2robot * transformation_result;

        std::vector<int> hyps;
        std::vector<int> locals;
        for (int idx{0}; idx < valids.size(); ++idx)
        {
            if (idx % 2 == 1)
                locals.push_back(valids[idx]);
            else
                hyps.push_back(valids[idx]);
        }

        // NOTE turnout necessary robot's poses
        std::vector<Eigen::Matrix4d> local_poses = turnout_Localpose(locals, who_detect);

        std::vector<Eigen::Matrix4d> hyp_poses = turnout_hyps_pose(transformation_result, hyps, another_one);

        std::vector<double> local_pcd = data_->r_3d;
        std::vector<Eigen::Vector3d> local_pcds = turnout_point_coord(local_pcd);
        // NOTE BA

        compute_BA(local_pcds, local_poses, hyp_poses, who_detect, locals, hyps, data_->index2value);
    }

    // NOTE GTSAMのsigmaはroll,pitch,yaw,x,y,z! なお、2Dの場合はx,y,theta
    void compute_BA(std::vector<Eigen::Vector3d> local_pcd, std::vector<Eigen::Matrix4d> local_pose,
                    std::vector<Eigen::Matrix4d> hyp_pose, std::string who_detect,
                    std::vector<int> local_ids, std::vector<int> hyp_ids, std::vector<int> loop_ids)
    {

        ROS_WARN("This is point cloud coordinate");
        for (auto pcdhoge : local_pcd)
        {
            std::cout << pcdhoge << std::endl;
            std::cout << "======================" << std::endl;
        }

        ROS_WARN("This is local pose");
        for (auto each : local_pose)
        {
            std::cout << each << std::endl;
            std::cout << "======================" << std::endl;
        }
        std::cout << std::endl;

        ROS_WARN("This is hyp pose");
        for (auto each : hyp_pose)
        {
            std::cout << each << std::endl;
            std::cout << "======================" << std::endl;
        }
        std::cout << std::endl;

        ROS_WARN("Local id");
        for (auto id : local_ids)
        {
            std::cout << id << std::endl;
        }

        ROS_WARN("hyp ids");
        for (auto id : hyp_ids)
        {
            std::cout << id << std::endl;
        }

        ROS_WARN("loop id");
        for (auto id : loop_ids)
        {
            std::cout << id << std::endl;
        }

        auto initial_pose_noise = noiseModel::Diagonal::Sigmas((Vector(6) << Vector3::Constant(0.0), Vector3::Constant(0.0)).finished());
        auto measurementNoise = noiseModel::Isotropic::Sigma(2, 1.0);

        NonlinearFactorGraph graph;

        std::map<std::vector<int>, Eigen::Matrix4d> *odom_local_ptr{nullptr};
        std::map<std::vector<int>, Eigen::VectorXd> *info_local_ptr{nullptr};
        std::map<std::vector<int>, Eigen::Matrix4d> *odom_hyp_ptr{nullptr};
        std::map<std::vector<int>, Eigen::VectorXd> *info_hyp_ptr{nullptr};
        std::string else_one;

        if (who_detect == "R1")
        {
            odom_local_ptr = &odom_dict1;
            info_local_ptr = &info_dict1;
            odom_hyp_ptr = &odom_dict2;
            info_hyp_ptr = &info_dict2;
            else_one = "R2";
        }
        else
        {
            odom_local_ptr = &odom_dict2;
            info_local_ptr = &info_dict2;
            odom_hyp_ptr = &odom_dict1;
            info_hyp_ptr = &info_dict1;
            else_one = "R1";
        }

        Pose3 initial_local_pose(local_pose[0]);

        graph.addPrior(Symbol('x', 0), initial_local_pose, initial_pose_noise);
        std::vector<Symbol> pose_symbol;

        for (size_t idx{0}; idx < local_ids.size(); ++idx)
        {
            // local_odom.push_back(Pose3((*odom_local_ptr)[{local_ids[idx], local_ids[idx - 1]}]));
            if (idx > 0)
            {
                if (odom_local_ptr->count({translate_index(local_ids[idx - 1], who_detect), translate_index(local_ids[idx], who_detect)}))
                {
                    Pose3 odometry((*odom_local_ptr)[{translate_index(local_ids[idx - 1], who_detect), translate_index(local_ids[idx], who_detect)}]);
                    auto odometryNoise = noiseModel::Diagonal::Variances((*info_local_ptr)[{translate_index(local_ids[idx - 1], who_detect), translate_index(local_ids[idx], who_detect)}]);
                    graph.emplace_shared<BetweenFactor<Pose3>>(Symbol('x', idx - 1), Symbol('x', idx), odometry, odometryNoise);
                }
                else
                {
                    ROS_WARN("One of odom constraints missing its sequential indexes...");
                    exit(1);
                }
            }

            // Projection regisration
            // Pose definition
            Pose3 individual_pose(local_pose[idx]);
            PinholeCamera<Cal3_S2> camera(individual_pose, *K);
            pose_symbol.push_back(Symbol('x', idx));

            for (size_t j = 0; j < local_pcd.size(); ++j)
            {
                Point2 measurement = camera.project(local_pcd[j]);
                graph.emplace_shared<GenericProjectionFactor<Pose3, Point3, Cal3_S2>>(
                    measurement, measurementNoise, Symbol('x', idx), Symbol('l', j), K);
            }
        }

        // hyp
        std::map<int, int> index_memo;
        for (size_t idx{0}; idx < hyp_ids.size(); ++idx)
        {
            index_memo[hyp_ids[idx]] = local_ids.size() + idx;
            if (idx > 0)
            {
                Eigen::Matrix4d apart_odom;
                Eigen::VectorXd apart_info(6);
                turnout_apartid_odom_info(translate_index(hyp_ids[0], else_one), translate_index(hyp_ids[idx], else_one),
                                          apart_odom, apart_info, info_hyp_ptr, odom_hyp_ptr);
                Pose3 odometry(apart_odom);
                auto odometryNoise = noiseModel::Diagonal::Variances(apart_info);
                graph.emplace_shared<BetweenFactor<Pose3>>(Symbol('x', local_ids.size() + idx), Symbol('x', local_ids.size() + idx - 1), odometry, odometryNoise);
            }

            Pose3 individual_pose(hyp_pose[idx]);
            PinholeCamera<Cal3_S2> camera(individual_pose, *K);
            pose_symbol.push_back(Symbol('x', local_ids.size() + idx));

            for (size_t j = 0; j < local_pcd.size(); ++j)
            {
                Point2 measurement = camera.project(local_pcd[j]);
                graph.emplace_shared<GenericProjectionFactor<Pose3, Point3, Cal3_S2>>(
                    measurement, measurementNoise, Symbol('x', local_ids.size() + idx), Symbol('l', j), K);
            }
        }
        // Loop Closure constraint
        auto loopNoise = noiseModel::Diagonal::Sigmas((Vector(6) << Vector3::Constant(0.1),
                                                       Vector3::Constant(0.3))
                                                          .finished());

        graph.emplace_shared<BetweenFactor<Pose3>>(Symbol('x', loop_ids[0]), Symbol('x', index_memo[loop_ids[1]]), Pose3(transformation_result), loopNoise);

        auto pointNoise = noiseModel::Isotropic::Sigma(3, 0.1);
        graph.addPrior(Symbol('l', 0), local_pcd[0], pointNoise);

        Values initialEstimate;
        local_ids.insert(local_ids.end(), hyp_ids.begin(), hyp_ids.end());
        for (size_t j{0}; j < local_ids.size(); ++j)
            initialEstimate.insert(Symbol('x', j), Pose3(local_pose[j]));

        for (size_t j{0}; j < local_pcd.size(); ++j)
            initialEstimate.insert<Point3>(Symbol('l', j), Point3(local_pcd[j]));

        Values result = LevenbergMarquardtOptimizer(graph, initialEstimate).optimize();
        result.print("Final results:\n");
        Marginals marginals(graph, result);

        std::cout << "initial error = " << graph.error(initialEstimate) << std::endl;
        std::cout << "final error = " << graph.error(result) << std::endl;

        // TODO hypの方はひとまずオドメトリによる拘束条件のみに限定して考える。余裕があればProx loop, loop closureによる影響も考慮すると良い。
    }
    // NOTE [[x,y,z,qx,qy,qz,qw],[..]]

    void turnout_apartid_odom_info(int priorID, int targetID, Eigen::Matrix4d &goal_odom, Eigen::VectorXd &goal_info,
                                   std::map<std::vector<int>, Eigen::VectorXd> *info_dict, std::map<std::vector<int>, Eigen::Matrix4d> *odom_dict)
    {
        for (int id{priorID}; id < targetID + 1; ++id)
        {

            Eigen::Matrix4d transfer_affine = (*odom_dict)[{priorID + 1, priorID}];
            goal_odom = transfer_affine * goal_odom;

            Eigen::VectorXd cov(6);
            cov << 1 / ((*info_dict)[{priorID + 1, priorID}][0]), 1 / ((*info_dict)[{priorID + 1, priorID}][1]), 1 / ((*info_dict)[{priorID + 1, priorID}][2]),
                1 / ((*info_dict)[{priorID + 1, priorID}][3]), 1 / ((*info_dict)[{priorID + 1, priorID}][4]), 1 / ((*info_dict)[{priorID + 1, priorID}][5]);

            goal_info = goal_info + cov;
        }
        goal_info << 1 / (goal_info[0]), 1 / (goal_info[1]), 1 / (goal_info[2]),
            1 / (goal_info[3]), 1 / (goal_info[4]), 1 / (goal_info[5]);
    }

    std::vector<Eigen::Vector3d> turnout_point_coord(std::vector<double> point_coord)
    {

        std::vector<Eigen::Vector3d> kp_local;

        for (size_t index{1}; index < point_coord.size() + 1; ++index)
        {
            // ３つ目の要素に差し掛かった時
            if (index % 3 == 0)
            {
                Eigen::Vector4d kp_loc_r(point_coord[index - 3] / 1000, point_coord[index - 2] / 1000, point_coord[index - 1] / 1000, 1.0);
                kp_loc_r = cam2robot * kp_loc_r;
                Eigen::Vector3d input_loc(kp_loc_r[0], kp_loc_r[1], kp_loc_r[2]);
                kp_local.push_back(input_loc);
                // NOTE ポイントのカメラ座標
            }
        }

        return kp_local;
    }

    std::vector<Eigen::Matrix4d> turnout_Localpose(std::vector<int> local_pairs, std::string who_detect)
    {
        std::map<int, int> *mapPath_dict_ptr{nullptr};
        std::vector<std::vector<double>> *path_ptr{nullptr};
        if (who_detect == "R1")
        {
            mapPath_dict_ptr = &mapPath_dict;
            path_ptr = &path_1;
        }
        else
        {
            mapPath_dict_ptr = &mapPath_dict_2;
            path_ptr = &path_2;
        }

        std::vector<Eigen::Matrix4d> answer_poses;
        Eigen::Matrix4d initial;
        initial << 1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1;
        answer_poses.push_back(initial);

        int local_origin_id{translate_index(local_pairs[0], who_detect)};
        int path_idx{(*mapPath_dict_ptr)[local_origin_id - 1]};

        Eigen::Vector3d origin_xyz((*path_ptr)[path_idx][0], (*path_ptr)[path_idx][1], (*path_ptr)[path_idx][2]);
        Eigen::Quaterniond origin_quat((*path_ptr)[path_idx][6], (*path_ptr)[path_idx][3], (*path_ptr)[path_idx][4], (*path_ptr)[path_idx][5]);
        Eigen::Matrix3d origin_rot = origin_quat.matrix();

        for (int id{1}; id < local_pairs.size(); ++id)
        {
            int local_id{translate_index(local_pairs[id], who_detect)};
            int local_path_idx{(*mapPath_dict_ptr)[local_id - 1]};
            Eigen::Vector3d local_xyz((*path_ptr)[local_path_idx][0], (*path_ptr)[local_path_idx][1], (*path_ptr)[local_path_idx][2]);
            Eigen::Quaterniond local_quat((*path_ptr)[local_path_idx][6], (*path_ptr)[local_path_idx][3], (*path_ptr)[local_path_idx][4], (*path_ptr)[local_path_idx][5]);
            Eigen::Matrix3d local_rot = local_quat.matrix();
            // x,y,z
            local_xyz = local_xyz - origin_xyz;
            Eigen::Translation3d local_xyz_(local_xyz.x(), local_xyz.y(), local_xyz.z());
            // rot
            local_rot = origin_rot.transpose() * local_rot;
            Eigen::Quaterniond hoge{local_rot};

            Eigen::Affine3d answer_pose = local_xyz_ * hoge;

            answer_poses.push_back(answer_pose.matrix());
        }
        return answer_poses;
    }

    std::vector<Eigen::Matrix4d> turnout_hyps_pose(Eigen::Matrix4d odom_trans, std::vector<int> hyp_pairs, std::string robot_name)
    {
        std::vector<Eigen::Matrix4d> hyps_local_poses = turnout_Localpose(hyp_pairs, robot_name);
        std::vector<Eigen::Vector3d> t_s;
        std::vector<Eigen::Matrix3d> r_s;
        std::vector<Eigen::Matrix4d> answers;

        Eigen::Matrix3d rotation_mat = odom_trans.block(0, 0, 3, 3);
        Eigen::Vector3d translation_vec = odom_trans.block(0, 3, 3, 1);

        for (auto each : hyps_local_poses)
        {
            Eigen::Vector3d each_translation = each.block(0, 3, 3, 1);
            Eigen::Matrix3d each_rot = each.block(0, 0, 3, 3);

            t_s.push_back(each_translation);
            r_s.push_back(each_rot);
        }

        for (int iteration{0}; iteration < t_s.size(); ++iteration)
        {
            Eigen::Vector3d ans_trans = rotation_mat * t_s[iteration] + translation_vec;
            Eigen::Translation3d ans_trans_(ans_trans.x(), ans_trans.y(), ans_trans.z());
            // Eigen::Matrix3d ans_rot = rotation_mat * r_s[iteration];
            Eigen::Matrix3d ans_rot = Eigen::Matrix3d::Identity();
            Eigen::Quaterniond ans_rot_{ans_rot};
            Eigen::Affine3d answer = ans_trans_ * ans_rot_;

            answers.push_back(answer.matrix());
        }

        return answers;
    }

    // NOTE all_rgbのindexから各々のrtabmapのidへ！
    int translate_index(int img_number, std::string robot_name)
    {
        if (robot_name == "R1")
            return (img_number - 1) / 2 + 1;
        else
            return img_number / 2;
    }
};

int main(int argc, char **argv)
{
    ros::init(argc, argv, "RO_estimator");
    RO_Estimator detector;

    ros::spin();

    return 0;
}
