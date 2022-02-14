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

    int status{0};

    Eigen::Matrix4d transformation_result;
    Eigen::Matrix4d cam2robot;
    Eigen::Matrix3d robot2cam;

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

        cam2robot << 0, -1, 0, 0,
            0, 0, -1, 0,
            1, 0, 0, 0;

        marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 10);

        points.header.frame_id = line_list.header.frame_id = "robot1/map";
        points.header.stamp = line_list.header.stamp = ros::Time::now();
        points.ns = line_list.ns = "robot1";
        points.action = line_list.action = visualization_msgs::Marker::ADD;
        points.pose.orientation.w = line_list.pose.orientation.w = 1.0;

        points.id = 0;
        line_list.id = 2;

        points.type = visualization_msgs::Marker::POINTS;
        line_list.type = visualization_msgs::Marker::LINE_LIST;

        // POINTS markers use x and y scale for width/height respectively
        points.scale.x = 0.2;
        points.scale.y = 0.2;

        // LINE_STRIP/LINE_LIST markers use only the x component of scale, for the line width
        line_list.scale.x = 0.1;

        points.color.r = 1.0f;
        points.color.a = 1.0;
        line_list.color.r = 1.0;
        line_list.color.a = 1.0;
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
                infos << each_link.information[0], each_link.information[7], each_link.information[14],
                    each_link.information[21], each_link.information[28], each_link.information[35];

                Eigen::Vector3d odom_translation(each_link.transform.translation.x, each_link.transform.translation.y, each_link.transform.translation.z);
                Eigen::Quaterniond odom_orientation(each_link.transform.rotation.w, each_link.transform.rotation.x, each_link.transform.rotation.y, each_link.transform.rotation.z);

                Eigen::Matrix4d transfer_affine;
                transfer_affine.block(0, 0, 3, 3) = odom_orientation.matrix();
                transfer_affine.block(0, 3, 3, 1) = odom_translation;

                info_dict1[fromto] = infos;
                odom_dict1[fromto] = transfer_affine;
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
                Eigen::Vector3d odom_translation(each_link.transform.translation.x, each_link.transform.translation.y, each_link.transform.translation.z);
                Eigen::Quaterniond odom_orientation(each_link.transform.rotation.w, each_link.transform.rotation.x, each_link.transform.rotation.y, each_link.transform.rotation.z);

                Eigen::Matrix4d transfer_affine;
                transfer_affine.block(0, 0, 3, 3) = odom_orientation.matrix();
                transfer_affine.block(0, 3, 3, 1) = odom_translation;

                Eigen::VectorXd infos(6);
                infos << each_link.information[0], each_link.information[7], each_link.information[14],
                    each_link.information[21], each_link.information[28], each_link.information[35];
                info_dict2[fromto] = infos;
                odom_dict2[fromto] = transfer_affine;
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
        std::cout << path_2.size() << std::endl;
        mapPath_dict_2[info_index_2] = path_2.size() - 1;
        info_index_2 += 1;
        if (status)
        {
            geometry_msgs::Point p;
            geometry_msgs::Point p_prev;
            points.points.clear();
            line_list.points.clear();
            for (size_t each{0}; each < path_2.size(); ++each)
            {
                Eigen::Vector3d p_mat(path_2[each][0], path_2[each][1], path_2[each][2]);
                p_mat = draw_rotation * p_mat + draw_t;
                p.x = p_mat.x();
                p.y = p_mat.y();
                p.z = p_mat.z();

                points.points.push_back(p);

                if (each > 0)
                {
                    Eigen::Vector3d p_prev_mat(path_2[each - 1][0], path_2[each - 1][1], path_2[each - 1][2]);
                    p_prev_mat = draw_rotation * p_prev_mat + draw_t;
                    line_list.points.push_back(p);
                    p_prev.x = p_prev_mat.x();
                    p_prev.y = p_prev_mat.y();
                    p_prev.z = p_prev_mat.z();
                    line_list.points.push_back(p_prev);
                }
            }
            marker_pub.publish(points);
            marker_pub.publish(line_list);
        }
        // lineの描画
    }

    // NOTE feature_matcherからrgbd odometryの計算結果をsubscribe
    void odom_CB(const cpp::HomogeneousArray::ConstPtr &data_)
    {

        transformation_result << data_->data[0], data_->data[1], data_->data[2], data_->data[3],
            data_->data[4], data_->data[5], data_->data[6], data_->data[7],
            data_->data[8], data_->data[9], data_->data[10], data_->data[11],
            0.0, 0.0, 0.0, 1.0;
        loop_info = data_->index2value;
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
    }

    void compute_BA(std::vector<Eigen::Vector3d> local_pcd, std::vector<Eigen::VectorXd> local_sigma, std::vector<Eigen::VectorXd> hyp_sigma,
                    std::vector<Eigen::Vector4d> local_pose, std::vector<Eigen::Vector4d> hyp_pose, std::string who_detect,
                    std::vector<int> local_ids, std::vector<int> hyp_ids)
    {

        auto initial_pose_noise = noiseModel::Diagonal::Sigmas((Vector(6) << Vector3::Constant(0.0), Vector3::Constant(0.0)).finished());
        auto measurementNoise = noiseModel::Isotropic::Sigma(2, 1.0);

        NonlinearFactorGraph graph;

        std::map<std::vector<int>, Eigen::Matrix4d> *odom_local_ptr{nullptr};
        std::map<std::vector<int>, Eigen::VectorXd> *info_local_ptr{nullptr};
        std::map<std::vector<int>, Eigen::Matrix4d> *odom_hyp_ptr{nullptr};
        std::map<std::vector<int>, Eigen::VectorXd> *info_hyp_ptr{nullptr};

        if (who_detect == "R1")
        {
            odom_local_ptr = &odom_dict1;
            info_local_ptr = &info_dict1;
            odom_hyp_ptr = &odom_dict2;
            info_hyp_ptr = &info_dict2;
        }
        else
        {
            odom_local_ptr = &odom_dict2;
            info_local_ptr = &info_dict2;
            odom_hyp_ptr = &odom_dict1;
            info_hyp_ptr = &info_dict1;
        }

        Pose3 initial_local_pose(local_pose[0]);

        graph.addPrior(Symbol('x', 0), initial_local_pose, initial_pose_noise);
        std::vector<Symbol> pose_symbol;

        for (size_t idx{0}; idx < local_ids.size(); ++idx)
        {
            // local_odom.push_back(Pose3((*odom_local_ptr)[{local_ids[idx], local_ids[idx - 1]}]));
            if (idx > 0)
            {
                if (odom_local_ptr->count({local_ids[idx], local_ids[idx - 1]}))
                {
                    Pose3 odometry((*odom_local_ptr)[{local_ids[idx], local_ids[idx - 1]}]);
                    auto odometryNoise = noiseModel::Diagonal::Sigmas((*info_local_ptr)[{local_ids[idx - 1], local_ids[idx]}]);
                    graph.emplace_shared<BetweenFactor<Pose3>>(local_ids[idx], local_ids[idx - 1], odometry, odometryNoise);
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
        for (size_t idx{local_ids.size()}; idx < local_ids.size() + hyp_ids.size() - 1; ++idx)
        {
        }

        // TODO hypの方はひとまずオドメトリによる拘束条件飲みに限定して考える。余裕があればProx loop, loop closureによる影響も考慮すると良い。
    }
    // NOTE [[x,y,z,qx,qy,qz,qw],[..]]

    std::vector<Eigen::Vector3d> turnout_point_coord(std::vector<double> point_coord)
    {

        std::vector<Eigen::Vector3d> kp_local;

        for (size_t index{1}; index < point_coord.size() + 1; ++index)
        {
            // ３つ目の要素に差し掛かった時
            if (index % 3 == 0)
            {
                Eigen::Vector3d kp_loc_r(point_coord[index - 3], point_coord[index - 2], point_coord[index - 1]);
                kp_local.push_back(kp_loc_r);
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
        initial << 0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0,
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
            // rot
            local_rot = origin_rot.transpose() * local_rot;

            Eigen::Matrix4d answer_pose;
            answer_pose.block(0, 0, 3, 3) = local_rot;
            answer_pose.block(0, 3, 3, 1) = local_xyz;
            answer_poses.push_back(answer_pose);
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
        Eigen::Vector3d translation_vec = odom_trans.block(3, 0, 3, 1);

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
            Eigen::Matrix3d ans_rot = rotation_mat * r_s[iteration];
            Eigen::Matrix4d answer;

            answer.block(0, 0, 3, 3) = ans_rot;
            answer.block(0, 3, 3, 1) = ans_trans;
            answers.push_back(answer);
        }

        return answers;
    }

    // // NOTE 共通特徴点の画像座標とカメラ座標を取得
    // void RO_CB(const cpp::FeatureArray::ConstPtr &data)
    // {

    //     std::vector<Eigen::Vector3d> kp_local;

    //     for (size_t index{1}; index < data->r_3d.size() + 1; ++index)
    //     {
    //         // ３つ目の要素に差し掛かった時
    //         if (index % 6 == 3)
    //         {
    //             Eigen::Vector3d kp_loc_r(data->r_3d[index - 3], data->r_3d[index - 2], data->r_3d[index - 1]);
    //             kp_local.push_back(kp_loc_r);
    //             // NOTE ポイントのカメラ座標
    //         }
    //     }
    // }

    int translate_index(int img_number, std::string robot_name)
    {
        if (robot_name == "R1")
            return (img_number - 1) / 2 + 1;
        else
            return img_number / 2;
    }

    Eigen::Vector3d turnout_T(Eigen::Vector3d transfer, std::string who_is_detect)
    {
        Eigen::Vector3d r1_to_r2;
        Eigen::Vector3d origin2r1;
        Eigen::Vector3d origin2r2;

        int r1_img_index, r2_img_index;
        Eigen::Vector3d transfer_1_to_2;
        if (who_is_detect == "R1")
        {
            r1_img_index = (loop_info[0] - 1) * 0.5 + 1;
            r2_img_index = loop_info[1] / 2;
            transfer_1_to_2 = transfer;
        }
        else
        {
            r1_img_index = (loop_info[1] - 1) * 0.5 + 1;
            r2_img_index = loop_info[0] / 2;
            transfer_1_to_2 = Eigen::Vector3d::Zero() - transfer;
        }
        origin2r1 << path_1[mapPath_dict[r1_img_index - 1]][0], path_1[mapPath_dict[r1_img_index - 1]][1], path_1[mapPath_dict[r1_img_index - 1]][2];

        origin2r2 << path_2[mapPath_dict_2[r2_img_index - 1]][0], path_2[mapPath_dict_2[r2_img_index - 1]][1], path_2[mapPath_dict_2[r2_img_index - 1]][2];

        Eigen::Vector3d ans_t = transfer_1_to_2 - (origin2r2 - origin2r1);
        return ans_t;
    }

    // NOTE R_o2o'=R_o2a * R_a2b * R_b2o'
    Eigen::Quaterniond turnout_R(Eigen::Matrix3d rotation_matrix, std::string who_is_detect)
    {
        Eigen::Quaterniond r1_q;
        Eigen::Quaterniond r2_q;

        int r1_img_index, r2_img_index;
        Eigen::Quaterniond rotation_1TO2;

        if (who_is_detect == "R1")
        {
            rotation_1TO2 = rotation_matrix;
            r1_img_index = (loop_info[0] - 1) * 0.5 + 1;
            r2_img_index = loop_info[1] / 2;
        }
        else
        {
            rotation_1TO2 = rotation_matrix.transpose();
            r1_img_index = (loop_info[1] - 1) * 0.5 + 1;
            r2_img_index = loop_info[0] / 2;
        }

        r1_q.x() = path_1[mapPath_dict[r1_img_index - 1]][3];
        r1_q.y() = path_1[mapPath_dict[r1_img_index - 1]][4];
        r1_q.z() = path_1[mapPath_dict[r1_img_index - 1]][5];
        r1_q.w() = path_1[mapPath_dict[r1_img_index - 1]][6];

        r2_q.x() = path_2[mapPath_dict_2[r2_img_index - 1]][3];
        r2_q.y() = path_2[mapPath_dict_2[r2_img_index - 1]][4];
        r2_q.z() = path_2[mapPath_dict_2[r2_img_index - 1]][5];
        r2_q.w() = path_2[mapPath_dict_2[r2_img_index - 1]][6];

        // NOTE クオータニオンの掛け算の方向は左側
        // Eigen::Matrix3d R2_origin2node = (r2_q.normalized()).toRotationMatrix();
        Eigen::Quaterniond q_r1_to_r2 = (r2_q.inverse()) * rotation_1TO2 * r1_q;
        return q_r1_to_r2.normalized();
    }
};

int main(int argc, char **argv)
{
    ros::init(argc, argv, "RO_estimator");
    RO_Estimator detector;

    ros::spin();

    return 0;
}
