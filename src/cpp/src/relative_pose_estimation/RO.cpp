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

#include "g2o_ba.h"

class RO_Estimator
{
private:
    // Cal3_S2::shared_ptr K{new Cal3_S2(617.5604248046, 617.3798828, 0.0, 317.55502, 244.730865)};

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
    ros::Publisher robot1_path_pub;

    visualization_msgs::Marker points, line_list;
    visualization_msgs::Marker points_2, line_list_2;

    std::string loop_info;
    // NOTE xyz / xyzw
    std::vector<std::vector<double>> path_1;
    std::vector<std::vector<double>> path_2;
    std::vector<int> valids;

    std::map<int, int> mapPath_dict;
    std::map<int, int> mapPath_dict_2;
    std::map<std::vector<int>, Eigen::VectorXd> info_dict1;
    std::map<std::vector<int>, Eigen::VectorXd> info_dict2;
    // std::map<std::vector<int>, std::vector<double>> info1_spare;
    // std::map<std::vector<int>, std::vector<double>> info2_spare;

    std::map<std::vector<int>, Eigen::Matrix4d> odom_dict1;
    std::map<std::vector<int>, Eigen::Matrix4d> odom_dict2;
    // NOTE {"R1":{1:[[x1,y1,z1],[x2,y2...]],2:...},"R2":...}

    std::map<int, std::vector<Eigen::Vector2d>> hyp_2d;
    std::map<int, std::vector<Eigen::Vector2d>> loop_2d;

    int info_index{1};
    int info_index_2{1};

    Eigen::Matrix4d transformation_result;
    Eigen::Matrix4d cam2robot;
    Eigen::Matrix3d robot2cam;

    Eigen::Matrix3d draw_rotation;
    Eigen::Vector3d draw_t;

    int status{0};

    // NOTE node_map...{time;{1:[]}
    // points location

public:
    // NOTE コンストラクタ、各種設定
    RO_Estimator(void)
    {
        rt_pub = n.advertise<cpp::RO_Array>("RT_result", 50);
        // BA_pub = n.advertise

        feature_sub = n.subscribe("features", 20, &RO_Estimator::RO_CB, this);
        path_sub1 = n.subscribe("robot1/rtabmap/mapPath", 10, &RO_Estimator::path1_CB, this);
        path_sub2 = n.subscribe("robot2/rtabmap/mapPath", 10, &RO_Estimator::path2_CB, this);
        info_sub1 = n.subscribe("robot1/rtabmap/mapGraph", 10, &RO_Estimator::info_CB1, this);
        info_sub2 = n.subscribe("robot2/rtabmap/mapGraph", 10, &RO_Estimator::info_CB2, this);
        transformation_sub = n.subscribe("odometry_result", 1000, &RO_Estimator::odom_CB, this);

        cam2robot << 0.0, 0.0, 1.0, 0.0,
            -1.0, 0.0, 0.0, 0.0,
            0.0, -1.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 1.0;

        robot2cam = (cam2robot.block(0, 0, 3, 3)).transpose();

        marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 10);
        robot1_path_pub = n.advertise<visualization_msgs::Marker>("robot1_path", 10);
        // NOTE robot1's trajectory
        {
            points_2.header.frame_id = line_list_2.header.frame_id = "robot1/map";
            points_2.header.stamp = line_list_2.header.stamp = ros::Time::now();
            points_2.ns = line_list_2.ns = "robot1";
            points_2.action = line_list_2.action = visualization_msgs::Marker::ADD;
            points_2.pose.orientation.w = line_list_2.pose.orientation.w = 1.0;

            points_2.id = 0;
            line_list_2.id = 2;

            points_2.type = visualization_msgs::Marker::POINTS;
            line_list_2.type = visualization_msgs::Marker::LINE_LIST;

            // POINTS markers use x and y scale for width/height respectively
            points_2.scale.x = 0.2;
            points_2.scale.y = 0.2;

            // LINE_STRIP/LINE_LIST markers use only the x component of scale, for the line width
            line_list_2.scale.x = 0.1;

            points_2.color.b = 1.0f;
            points_2.color.a = 1.0;
            line_list_2.color.b = 1.0;
            line_list_2.color.a = 1.0;
        }
        // NOTE robot2's trajectory
        {
            points.header.frame_id = line_list.header.frame_id = "robot2/map";
            points.header.stamp = line_list.header.stamp = ros::Time::now();
            points.ns = line_list.ns = "robot2";
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

                std::vector<double> info_spare{each_link.information[21], each_link.information[28], each_link.information[35],
                                               each_link.information[0], each_link.information[7], each_link.information[14]};

                Eigen::Translation3d odom_translation(each_link.transform.translation.x, each_link.transform.translation.y, each_link.transform.translation.z);
                Eigen::Quaterniond odom_orientation(each_link.transform.rotation.w, each_link.transform.rotation.x, each_link.transform.rotation.y, each_link.transform.rotation.z);

                Eigen::Affine3d transfer_affine;
                transfer_affine = odom_translation * odom_orientation;

                info_dict1[fromto] = infos;
                odom_dict1[fromto] = transfer_affine.matrix();
                // info1_spare[fromto] = info_spare;
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
                std::vector<double> info_spare{each_link.information[21], each_link.information[28], each_link.information[35],
                                               each_link.information[0], each_link.information[7], each_link.information[14]};

                infos << each_link.information[21], each_link.information[28], each_link.information[35],
                    each_link.information[0], each_link.information[7], each_link.information[14];
                info_dict2[fromto] = infos;
                odom_dict2[fromto] = transfer_affine.matrix();
                // info2_spare[fromto] = info_spare;
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

        geometry_msgs::Point p;
        geometry_msgs::Point p_prev;
        points_2.points.clear();
        line_list_2.points.clear();
        for (size_t each{0}; each < path_1.size(); ++each)
        {
            Eigen::Vector3d p_mat(path_1[each][0], path_1[each][1], path_1[each][2]);
            p.x = p_mat.x();
            p.y = p_mat.y();
            p.z = p_mat.z();
            points.points.push_back(p);
            if (each > 0)
            {
                Eigen::Vector3d p_prev_mat(path_1[each - 1][0], path_1[each - 1][1], path_1[each - 1][2]);
                line_list_2.points.push_back(p);
                p_prev.x = p_prev_mat.x();
                p_prev.y = p_prev_mat.y();
                p_prev.z = p_prev_mat.z();
                line_list_2.points.push_back(p_prev);
            }
        }
        marker_pub.publish(points_2);
        marker_pub.publish(line_list_2);
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

        std::vector<int> hyps;
        std::vector<int> locals;
        for (int idx{0}; idx < valids.size(); ++idx)
        {
            if (idx % 2 == 0)
                locals.push_back(valids[idx]);
            else
                hyps.push_back(valids[idx]);
        }

        // // NOTE turnout necessary robot's poses
        std::vector<Eigen::Matrix4d> local_poses = turnout_Localpose(locals, who_detect);

        std::vector<Eigen::Matrix4d> hyp_poses = turnout_hyps_pose(transformation_result, hyps, another_one);

        std::vector<double> local_pcd = data_->r_3d;
        std::vector<Eigen::Vector3d> local_pcds = turnout_point_coord(local_pcd);
        // NOTE BA

        // compute_BA(local_pcds, Eigen::Matrix4d::Identity(), who_detect, hyp_2d, loop_2d, locals, hyps, local_poses, hyp_poses);
        // Eigen::Matrix4d goal_transformation = compute_BA(local_pcds, Eigen::Matrix4d::Identity(), who_detect, hyp_2d, loop_2d, locals, hyps, local_poses, hyp_poses);
        // -----------------------------------------------------------------------------
        // Eigen::Vector3d transfer_;
        // Eigen::Matrix3d rotation_;
        // transfer_ = goal_transformation.block(0, 3, 3, 1);
        // rotation_ = goal_transformation.block(0, 0, 3, 3);

        // rotation_ = cam2robot.block(0, 0, 3, 3) * rotation_ * (cam2robot.block(0, 0, 3, 3)).transpose();
        // transfer_ = cam2robot.block(0, 0, 3, 3) * transfer_;

        // Eigen::Affine3d ans_r = turnout_R(rotation_, who_detect, valids, transfer_);

        // status = 1;

        // cpp::RO_Array pose_result;
        // Eigen::Quaterniond for_publish_rot{ans_r.rotation()};
        // std::vector<double> R{for_publish_rot.w(), for_publish_rot.x(), for_publish_rot.y(), for_publish_rot.z()};
        // std::vector<double> t{ans_r.translation()[0], ans_r.translation()[1], ans_r.translation()[2]};

        // pose_result.translation = t;
        // pose_result.euler = R;

        // rt_pub.publish(pose_result);

        // draw_rotation = ans_r.rotation();
        // draw_t = ans_r.translation();
        // status = 1;

        hyp_2d.clear();
        loop_2d.clear();
    }

    // NOTE GTSAMのsigmaはroll,pitch,yaw,x,y,z! なお、2Dの場合はx,y,theta
    // Eigen::Matrix4d compute_BA(std::vector<Eigen::Vector3d> local_pcd, Eigen::Matrix4d initial_pose,
    //                            std::string who_detect,
    //                            std::map<int, std::vector<Eigen::Vector2d>> hyp_2d,
    //                            std::map<int, std::vector<Eigen::Vector2d>> loop_2d,
    //                            std::vector<int> local_ids, std::vector<int> hyp_ids,
    //                            std::vector<Eigen::Matrix4d> local_pose, std::vector<Eigen::Matrix4d> hyp_pose)
    // void compute_BA(std::vector<Eigen::Vector3d> local_pcd, Eigen::Matrix4d initial_pose,
    //                 std::string who_detect,
    //                 std::map<int, std::vector<Eigen::Vector2d>> hyp_2d,
    //                 std::map<int, std::vector<Eigen::Vector2d>> loop_2d,
    //                 std::vector<int> local_ids, std::vector<int> hyp_ids,
    //                 std::vector<Eigen::Matrix4d> local_pose, std::vector<Eigen::Matrix4d> hyp_pose)
    // {

    //     ROS_WARN("This is local pose");
    //     for (auto each : local_pose)
    //     {
    //         std::cout << each << std::endl;
    //         std::cout << "======================" << std::endl;
    //     }
    //     std::cout << std::endl;

    //     ROS_WARN("This is hyp pose");
    //     for (auto each : hyp_pose)
    //     {
    //         std::cout << each << std::endl;
    //         std::cout << "======================" << std::endl;
    //     }
    //     std::cout << std::endl;

    // for (const auto [key, index] : loop_2d)
    // {
    //     std::cout << key << std::endl;
    //     std::cout << "----------------------" << std::endl;
    //     for (auto val : index)
    //     {
    //         std::cout << val << std::endl;
    //     }
    // }
    // for (auto val : loop_2d[1])
    // {
    //     std::cout << val << std::endl;
    // }

    // std::cout << "================================" << std::endl;

    // for (auto val : hyp_2d[0])
    // {
    //     std::cout << val << std::endl;
    // }

    // auto initial_pose_noise = noiseModel::Diagonal::Sigmas((Vector(6) << Vector3::Constant(0.001), Vector3::Constant(0.001)).finished());
    // auto measurementNoise = noiseModel::Isotropic::Sigma(2, 0.3);

    // NonlinearFactorGraph graph;

    // std::map<std::vector<int>, Eigen::Matrix4d> *odom_local_ptr{nullptr};
    // std::map<std::vector<int>, Eigen::VectorXd> *info_local_ptr{nullptr};
    // std::map<std::vector<int>, Eigen::Matrix4d> *odom_hyp_ptr{nullptr};
    // std::map<std::vector<int>, Eigen::VectorXd> *info_hyp_ptr{nullptr};
    // std::string else_one;

    // if (who_detect == "R1")
    // {
    //     odom_local_ptr = &odom_dict1;
    //     info_local_ptr = &info_dict1;
    //     odom_hyp_ptr = &odom_dict2;
    //     info_hyp_ptr = &info_dict2;
    //     else_one = "R2";
    // }
    // else
    // {
    //     odom_local_ptr = &odom_dict2;
    //     info_local_ptr = &info_dict2;
    //     odom_hyp_ptr = &odom_dict1;
    //     info_hyp_ptr = &info_dict1;
    //     else_one = "R1";
    // }

    // Pose3 initial_local_pose(initial_pose);

    // graph.addPrior(Symbol('x', 0), initial_local_pose, initial_pose_noise);
    // std::vector<Symbol> pose_symbol;
    // int index_count{0};
    // // local

    // for (size_t idx{0}; idx < local_ids.size(); ++idx)
    // {
    //     if (idx > 0)
    //     {
    //         if (odom_local_ptr->count({translate_index(local_ids[idx - 1], who_detect), translate_index(local_ids[idx], who_detect)}))
    //         {
    //             Eigen::Matrix4d odom = (*odom_local_ptr)[{translate_index(local_ids[idx - 1], who_detect), translate_index(local_ids[idx], who_detect)}];
    //             Eigen::Matrix3d odom_rot = odom.block(0, 0, 3, 3);
    //             Eigen::Vector3d trans = odom.block(0, 3, 3, 1);
    //             Eigen::Quaterniond quat_rot{odom_rot};
    //             trans = robot2cam * trans;
    //             // Eigen::Translation<double,3> tr(odom.block(0,3,1,3).x(),odom.block(0,3,1,3).y(),odom.block(0,3,1,3).z())
    //             Eigen::Translation<double, 3> tr_(trans.x(), trans.y(), trans.z());

    //             Eigen::Affine3d answer_tr = quat_rot * tr_;

    //             // Pose3 odometry((*odom_local_ptr)[{translate_index(local_ids[idx - 1], who_detect), translate_index(local_ids[idx], who_detect)}]);
    //             // Pose3 odometry(answer_tr.matrix());
    //             // auto odometryNoise = noiseModel::Diagonal::Variances((*info_local_ptr)[{translate_index(local_ids[idx - 1], who_detect), translate_index(local_ids[idx], who_detect)}]);
    //             // graph.emplace_shared<BetweenFactor<Pose3>>(Symbol('x', index_count - 1), Symbol('x', index_count), odometry, odometryNoise);
    //         }
    //         else
    //         {
    //             ROS_WARN("One of odom constraints missing its sequential indexes...");
    //             // exit(1);
    //         }
    //     }

    //     pose_symbol.push_back(Symbol('x', index_count));

    //     for (int pixel_id{0}; pixel_id < loop_2d[0].size(); ++pixel_id)
    //     {
    //         Point2 measurement = loop_2d[index_count][pixel_id];
    //         graph.emplace_shared<GenericProjectionFactor<Pose3, Point3, Cal3_S2>>(
    //             measurement, measurementNoise, Symbol('x', index_count), Symbol('l', pixel_id), K);
    //     }
    //     index_count += 1;
    // }

    // // hyp
    // // for (size_t idx{0}; idx < hyp_ids.size(); ++idx)
    // // {
    // pose_symbol.push_back(Symbol('x', index_count));

    // for (int pixel_id{0}; pixel_id < loop_2d[0].size(); ++pixel_id)
    // {
    //     Point2 measurement_ = hyp_2d[index_count][pixel_id];
    //     // std::cout << measurement_ << std::endl;
    //     graph.emplace_shared<GenericProjectionFactor<Pose3, Point3, Cal3_S2>>(
    //         measurement_, measurementNoise, Symbol('x', index_count), Symbol('l', pixel_id), K);
    // }
    // index_count += 1;
    // // std::cout << index_count << std::endl;
    // // }

    // // // Loop Closure constraint
    // auto loopNoise = noiseModel::Diagonal::Sigmas((Vector(6) << Vector3::Constant(0.1),
    //                                                Vector3::Constant(0.1))
    //                                                   .finished());

    // graph.emplace_shared<BetweenFactor<Pose3>>(Symbol('x', 0), Symbol('x', local_ids.size()), Pose3(transformation_result), loopNoise);

    // auto pointNoise = noiseModel::Isotropic::Sigma(3, 0.5);
    // graph.addPrior(Symbol('l', 0), local_pcd[0], pointNoise);

    // // graph.print("Factor Graph:\n");

    // Values initialEstimate;
    // Eigen::Translation<double, 3> z_noise(0, 0, 0.001);
    // Eigen::Quaterniond no_rot{Eigen::Matrix3d::Identity()};
    // Eigen::Affine3d slight_noise = z_noise * no_rot;
    // // local_ids.insert(local_ids.end(), hyp_ids.begin(), hyp_ids.end());
    // for (size_t j{0}; j < local_ids.size(); ++j)
    // {
    //     if ((j > 0) & (local_pose[j - 1] == local_pose[j]))
    //     {
    //         local_pose[j] = slight_noise.matrix() * local_pose[j];
    //         initialEstimate.insert(Symbol('x', j), Pose3(local_pose[j]));
    //     }
    //     else
    //         initialEstimate.insert(Symbol('x', j), Pose3(local_pose[j]));
    // }

    // // for (size_t j{0}; j < hyp_ids.size(); ++j)
    // Key last_symbol;
    // for (size_t j{0}; j < 1; ++j)

    // {
    //     if ((j > 0) & (hyp_pose[j - 1] == hyp_pose[j]))
    //     {
    //         hyp_pose[j] = slight_noise.matrix() * hyp_pose[j];
    //         initialEstimate.insert(Symbol('x', j + local_ids.size()), Pose3(hyp_pose[j]));
    //     }
    //     else
    //         initialEstimate.insert(Symbol('x', j + local_ids.size()), Pose3(hyp_pose[j]));
    //     last_symbol = Symbol('x', j + local_ids.size());
    // }

    // for (size_t j{0}; j < local_pcd.size(); ++j)
    //     initialEstimate.insert<Point3>(Symbol('l', j), Point3(local_pcd[j]));

    // // boost::shared_ptr<GaussianFactorGraph> gaussian = graph.linearize(initialEstimate);
    // // VectorValues result = gaussian->optimizeDensely();

    // Values result = LevenbergMarquardtOptimizer(graph, initialEstimate).optimize();
    // // std::cout << result.at(last_symbol) << std::endl;
    // result.print("Final results:\n");
    // std::cout << result.at<Pose3>(last_symbol).matrix() << std::endl;
    // Marginals marginals(graph, result);
    // std::cout << "======================Marginals==============================" << std::endl;
    // std::cout << marginals.marginalCovariance(pose_symbol[0]) << std::endl;
    // std::cout << std::endl;
    // std::cout << marginals.marginalCovariance(pose_symbol[1]) << std::endl;

    // std::cout << "initial error = " << graph.error(initialEstimate) << std::endl;
    // std::cout << "final error = " << graph.error(result) << std::endl;

    // return result.at<Pose3>(Symbol('x', 2))
    //     .matrix();
    // TODO hypの方はひとまずオドメトリによる拘束条件のみに限定して考える。余裕があればProx loop, loop closureによる影響も考慮すると良い。
    // }
    // NOTE [[x,y,z,qx,qy,qz,qw],[..]]

    void
    turnout_apartid_odom_info(int priorID, int targetID, Eigen::Matrix4d &goal_odom, Eigen::VectorXd &goal_info,
                              std::map<std::vector<int>, std::vector<double>> *info_dict, std::map<std::vector<int>, Eigen::Matrix4d> *odom_dict)
    {
        for (int id{priorID}; id < targetID + 1; ++id)
        {
            std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
            Eigen::Matrix4d transfer_affine = (*odom_dict)[{priorID + 1, priorID}];
            goal_odom = transfer_affine * goal_odom;

            Eigen::VectorXd cov(6);
            std::vector<int> referred_id(priorID + 1, priorID);

            std::cout << priorID << std::endl;
            cov << 1 / ((*info_dict)[referred_id][0]), 1 / ((*info_dict)[referred_id][1]), 1 / ((*info_dict)[referred_id][2]),
                1 / ((*info_dict)[referred_id][3]), 1 / ((*info_dict)[referred_id][4]), 1 / ((*info_dict)[referred_id][5]);

            std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;

            goal_info = goal_info + cov;
        }
        goal_odom = goal_odom * cam2robot.transpose();
        goal_info << 1 / (goal_info(0)), 1 / (goal_info(1)), 1 / (goal_info(2)),
            1 / (goal_info(3)), 1 / (goal_info(4)), 1 / (goal_info(5));
    }

    std::vector<Eigen::Vector3d> turnout_point_coord(std::vector<double> point_coord)
    {

        std::vector<Eigen::Vector3d> kp_local;

        // ROS_INFO("Next stage");
        for (size_t index{1}; index < point_coord.size() + 1; ++index)
        {
            // ３つ目の要素に差し掛かった時
            if (index % 3 == 0)
            {
                Eigen::Vector4d kp_loc_r(point_coord[index - 3] / 1000, point_coord[index - 2] / 1000, point_coord[index - 1] / 1000, 1.0);
                // std::cout << kp_loc_r << std::endl;
                // NOTE GTSAMはcameraのcoordinate frameなのでそのままにしておく
                // kp_loc_r = cam2robot * kp_loc_r;
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
        // std::cout << origin_xyz << std::endl;
        Eigen::Quaterniond origin_quat((*path_ptr)[path_idx][6], (*path_ptr)[path_idx][3], (*path_ptr)[path_idx][4], (*path_ptr)[path_idx][5]);
        Eigen::Matrix3d origin_rot = origin_quat.matrix();
        // std::cout << origin_rot << std::endl;

        for (int id{1}; id < local_pairs.size(); ++id)
        {
            int local_id{translate_index(local_pairs[id], who_detect)};
            int local_path_idx{(*mapPath_dict_ptr)[local_id - 1]};
            Eigen::Vector3d local_xyz((*path_ptr)[local_path_idx][0], (*path_ptr)[local_path_idx][1], (*path_ptr)[local_path_idx][2]);
            Eigen::Quaterniond local_quat((*path_ptr)[local_path_idx][6], (*path_ptr)[local_path_idx][3], (*path_ptr)[local_path_idx][4], (*path_ptr)[local_path_idx][5]);
            Eigen::Matrix3d local_rot = local_quat.matrix();

            // rot
            local_rot = origin_rot.transpose() * local_rot;
            local_rot = robot2cam * local_rot * robot2cam.transpose();

            local_xyz = robot2cam * (local_xyz - origin_xyz);
            Eigen::Translation3d local_xyz_(local_xyz.x(), local_xyz.y(), local_xyz.z());
            Eigen::Quaterniond hoge{local_rot};

            Eigen::Affine3d answer_pose = local_xyz_ * hoge;

            // NOTE gtsamのcoordinate franeに座標変換

            answer_poses.push_back(answer_pose.matrix());
        }
        return answer_poses;
    }

    std::vector<Eigen::Matrix4d> turnout_hyps_pose(Eigen::Matrix4d odom_trans, std::vector<int> hyp_pairs, std::string robot_name)
    {
        std::vector<Eigen::Matrix4d> hyps_local_poses = turnout_Localpose(hyp_pairs, robot_name);
        for (auto hyps_pose : hyps_local_poses)
        {
            // std::cout << hyps_pose << std::endl;
        }
        std::vector<Eigen::Vector3d> t_s;
        std::vector<Eigen::Matrix3d> r_s;
        std::vector<Eigen::Matrix4d> answers;

        Eigen::Matrix3d rotation_mat = odom_trans.block(0, 0, 3, 3);

        Eigen::Vector3d translation_vec = odom_trans.block(0, 3, 3, 1);

        for (auto each : hyps_local_poses)
        {
            Eigen::Vector3d each_translation = each.block(0, 3, 3, 1);
            Eigen::Matrix3d each_rot = each.block(0, 0, 3, 3);
            // std::cout << each_rot << std::endl;

            t_s.push_back(each_translation);
            r_s.push_back(each_rot);
        }
        // std::cout << "====================================================" << std::endl;
        for (int iteration{0}; iteration < t_s.size(); ++iteration)
        {
            Eigen::Vector3d ans_trans = rotation_mat * t_s[iteration] + translation_vec;
            Eigen::Translation3d ans_trans_(ans_trans.x(), ans_trans.y(), ans_trans.z());

            Eigen::Matrix3d ans_rot = rotation_mat * r_s[iteration];

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

    void RO_CB(const cpp::FeatureArray::ConstPtr &data)
    {
        int id = data->id;
        loop_info = data->me;
        std::vector<Eigen::Vector2d> img_coord;

        for (size_t index{1}; index < data->img_coord.size() + 1; ++index)
        {
            if (index % 3 == 0)
            {
                // NOTE ポイントのカメラ座標
                Eigen::Vector2d r_coord(data->img_coord[index - 3], data->img_coord[index - 2]);
                img_coord.push_back(r_coord);
            }
        }

        if (loop_info == "hyp")
            hyp_2d[id] = img_coord;
        else
            loop_2d[id] = img_coord;
    }

    void turnout_T(std::string who_is_detect, std::vector<int> valid,
                   Eigen::Translation3d &translation_1, Eigen::Translation3d &translation_2)
    {
        int r1_img_index, r2_img_index;
        if (who_is_detect == "R1")
        {
            r1_img_index = (valid[0] - 1) * 0.5 + 1;
            r2_img_index = valid[1] / 2;
        }
        else
        {
            r1_img_index = (valid[1] - 1) * 0.5 + 1;
            r2_img_index = valid[0] / 2;
        }

        Eigen::Translation<double, 3> origin2r1(path_1[mapPath_dict[r1_img_index]][0], path_1[mapPath_dict[r1_img_index]][1], path_1[mapPath_dict[r1_img_index]][2]);
        Eigen::Translation<double, 3> origin2r2(path_2[mapPath_dict_2[r2_img_index]][0], path_2[mapPath_dict_2[r2_img_index]][1], path_2[mapPath_dict_2[r2_img_index]][2]);

        translation_1 = origin2r1;
        translation_2 = origin2r2;
    }

    Eigen::Affine3d turnout_R(Eigen::Matrix3d rotation_matrix, std::string who_is_detect, std::vector<int> valid,
                              Eigen::Vector3d transfer)
    {
        Eigen::Translation3d origin2r1, origin2r2;
        Eigen::Translation3d transfer_aff(transfer.x(), transfer.y(), transfer.z());
        turnout_T(who_is_detect, valid, origin2r1, origin2r2);

        int r1_img_index, r2_img_index;
        Eigen::Quaterniond rotation_1TO2{rotation_matrix};
        Eigen::Affine3d relative_measurement = rotation_1TO2 * transfer_aff;

        if (who_is_detect == "R1")
        {
            r1_img_index = (valid[0] - 1) * 0.5 + 1;
            r2_img_index = valid[1] / 2;
        }
        else
        {
            relative_measurement = relative_measurement.inverse();
            r1_img_index = (valid[1] - 1) * 0.5 + 1;
            r2_img_index = valid[0] / 2;
        }

        Eigen::Quaterniond r1_q;
        Eigen::Quaterniond r2_q;
        r1_q.x() = path_1[mapPath_dict[r1_img_index]][3];
        r1_q.y() = path_1[mapPath_dict[r1_img_index]][4];
        r1_q.z() = path_1[mapPath_dict[r1_img_index]][5];
        r1_q.w() = path_1[mapPath_dict[r1_img_index]][6];

        r2_q.x() = path_2[mapPath_dict_2[r2_img_index]][3];
        r2_q.y() = path_2[mapPath_dict_2[r2_img_index]][4];
        r2_q.z() = path_2[mapPath_dict_2[r2_img_index]][5];
        r2_q.w() = path_2[mapPath_dict_2[r2_img_index]][6];

        Eigen::Affine3d origin_to_r1 = origin2r1 * r1_q;
        Eigen::Affine3d origin_to_r2 = origin2r2 * r2_q;
        relative_measurement = origin_to_r1 * relative_measurement * (origin_to_r2.inverse());

        return relative_measurement;
    }
};

int main(int argc, char **argv)
{
    ros::init(argc, argv, "RO_estimator");
    RO_Estimator detector;

    ros::spin();

    return 0;
}
