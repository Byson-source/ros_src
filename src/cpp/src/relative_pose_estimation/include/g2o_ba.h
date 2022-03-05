#ifndef G2O_BA_H
#define G2O_BA_H

#include <iostream>
#include <stdint.h>

#include <unordered_set>

#include "g2o/core/sparse_optimizer.h"
#include "g2o/core/block_solver.h"
#include "g2o/core/solver.h"
#include "g2o/core/robust_kernel_impl.h"
#include "g2o/core/optimization_algorithm_levenberg.h"
#include "g2o/solvers/cholmod/linear_solver_cholmod.h"
#include "g2o/solvers/dense/linear_solver_dense.h"
#include "g2o/types/sba/types_six_dof_expmap.h"
//#include "g2o/math_groups/se3quat.h"
#include "g2o/solvers/structure_only/structure_only_solver.h"
#include <Eigen/Core>
#include <Eigen/Geometry>
#include "g2o/types/slam3d/edge_se3.h"

class G2o_ba
{
private:
    double pixel_noise;
    g2o::SparseOptimizer optimizer;
    std::unique_ptr<g2o::BlockSolver_6_3::LinearSolverType> linearSolver;

    double focal_length{617.5604};
    Eigen::Vector2d principal_point;
    g2o::CameraParameters *cam_params;

    int pose_id{0};
    int point_id;

    int local_num;
    // search inlier
    std::vector<g2o::EdgeProjectXYZ2UV *> project_edges;
    // std::vector<g2o::SE3Quat> odometry_edges;

public:
    G2o_ba(double pix_noise = 1) : pixel_noise(pix_noise)
    {
        principal_point << 317.55502, 244.730865;
        optimizer.setVerbose(false);
        linearSolver = g2o::make_unique<g2o::LinearSolverCholmod<g2o::BlockSolver_6_3::PoseMatrixType>>();
        g2o::OptimizationAlgorithmLevenberg *solver = new g2o::OptimizationAlgorithmLevenberg(
            g2o::make_unique<g2o::BlockSolver_6_3>(std::move(linearSolver)));
        optimizer.setAlgorithm(solver);

        cam_params = new g2o::CameraParameters(focal_length, principal_point, 0.);
        cam_params->setId(0);

        if (!optimizer.addParameter(cam_params))
        {
            assert(false);
        }
    }

    void setPose(std::vector<Eigen::Matrix4d> local_poses, std::vector<Eigen::Matrix4d> hyp_poses)
    {
        // idの順番を保持したままひとまとめにする。
        local_num = local_poses.size();
        std::vector<Eigen::Matrix4d> all_poses;
        // NOTE カメラ座標から世界座標系を見た座標を入力する。
        // for (auto val : local_poses)
        //     all_poses.push_back(val.inverse());

        // for (auto val : hyp_poses)
        //     all_poses.push_back(val.inverse());
        // all_poses = local_poses;

        for (auto pose : all_poses)
        {
            Eigen::Vector3d trans = pose.block(0, 3, 3, 1);
            Eigen::Matrix3d rot_ = pose.block(0, 0, 3, 3);
            Eigen::Quaterniond rot{rot_};

            g2o::SE3Quat pose_g2o(rot, trans);
            // poseのSymbol('x',i)みたいな感じ？？
            g2o::VertexSE3Expmap *v_se3 = new g2o::VertexSE3Expmap();
            v_se3->setId(pose_id);
            if (pose_id == 0)
            {
                v_se3->setFixed(true);
            }
            v_se3->setEstimate(pose_g2o);
            optimizer.addVertex(v_se3);
            pose_id++;
        }
        point_id = pose_id;
    }

    void setPoint_and_measurement(std::vector<Eigen::Vector3d> pcds, std::map<int, std::vector<Eigen::Vector2d>> local_2d,
                                  std::map<int, std::vector<Eigen::Vector2d>> hyp_2d)
    {
        std::vector<std::vector<Eigen::Vector2d>> all_pixels;
        for (const auto [key, val] : local_2d)
            all_pixels.push_back(val);
        for (const auto [key, val] : hyp_2d)
            all_pixels.push_back(val);

        for (size_t point_idx{0}; point_idx < pcds.size(); ++point_idx)
        {
            // set point
            g2o::VertexSBAPointXYZ *v_p = new g2o::VertexSBAPointXYZ();
            v_p->setId(point_id);
            v_p->setMarginalized(true);
            v_p->setEstimate(pcds.at(point_idx));
            optimizer.addVertex(v_p);

            // set measurement
            for (size_t j = 0; j < all_pixels.size(); ++j)
            {
                Eigen::Vector2d observed_pixel = (all_pixels.at(j)).at(point_idx);
                g2o::EdgeProjectXYZ2UV *e = new g2o::EdgeProjectXYZ2UV();
                e->setVertex(0, dynamic_cast<g2o::OptimizableGraph::Vertex *>(v_p));

                e->setVertex(1, dynamic_cast<g2o::OptimizableGraph::Vertex *>(optimizer.vertices().find(j)->second));

                e->setMeasurement(observed_pixel);
                e->information() = Eigen::Matrix2d::Identity();

                // robust kernel
                g2o::RobustKernelHuber *rk = new g2o::RobustKernelHuber;
                e->setRobustKernel(rk);

                e->setParameterId(0, 0);
                optimizer.addEdge(e);
                project_edges.push_back(e);
            }
            ++point_id;
        }
    }

    Eigen::VectorXd info2cov(Eigen::VectorXd info)
    {
        Eigen::VectorXd cov(6);
        cov << 1 / info(0), 1 / info(1), 1 / info(2), 1 / info(3), 1 / info(4), 1 / info(5);
        return cov;
    }

    Eigen::VectorXd cov2info(Eigen::VectorXd cov)
    {
        Eigen::VectorXd info(6);
        info << 1 / cov(0), 1 / cov(1), 1 / cov(2), 1 / cov(3), 1 / cov(4), 1 / cov(5);
        return info;
    }

    void dict_from_vec(std::vector<Eigen::Affine3d> &odom_vec,
                       std::vector<Eigen::MatrixXd> &info_vec,
                       std::vector<int> &succeed,
                       // NOTE 連番じゃない時のindexを記憶しておく。
                       std::map<std::vector<int>, Eigen::Affine3d> odom_dict,
                       std::map<std::vector<int>, Eigen::VectorXd> info_dict,
                       std::vector<int> pose_ids)
    {
        for (int pose_idx{0}; pose_idx < pose_ids.size() - 1; ++pose_idx)
        {
            Eigen::Affine3d appended_odom;
            appended_odom.matrix() = Eigen::Matrix4d::Identity();
            Eigen::VectorXd appended_cov(6);

            // FIXME 現在のhypが一つ前のhypよりも小さい時、無視する。これも考慮できるようにしておくこと。
            if (pose_ids.at(pose_idx + 1) < pose_ids.at(pose_idx))
                succeed.push_back(0);

            for (int idx_begin{pose_ids.at(pose_idx)}; idx_begin < pose_ids.at(pose_idx + 1); ++idx_begin)
            {
                // NOTE 時々、連番じゃないときがある。
                if (odom_dict.count({idx_begin, idx_begin + 1}) != 0)
                {
                    // odom
                    appended_odom = appended_odom * odom_dict[{idx_begin, idx_begin + 1}];
                    // info
                    appended_cov += info2cov(info_dict[{idx_begin, idx_begin + 1}]);
                    if (idx_begin == pose_ids.at(pose_idx + 1) - 1)
                        succeed.push_back(1);
                }
                else
                {
                    succeed.push_back(0);
                    appended_cov << 1.0, 1.0, 1.0, 1.0, 1.0, 1.0;
                    std::cout << idx_begin << "->" << idx_begin + 1 << std::endl;
                    ROS_WARN("There is a lacked sequence. One of odom constaints is ignored.");
                    break;
                }
            }

            odom_vec.push_back(appended_odom);

            Eigen::MatrixXd appended_cov_matrix(6, 6);
            appended_cov_matrix = cov2info(appended_cov).asDiagonal();
            info_vec.push_back(appended_cov_matrix);
        }
    }
    // local->hyp
    void set_odometry_constraint(std::map<std::vector<int>, Eigen::Affine3d> odom_dict,
                                 std::map<std::vector<int>, Eigen::VectorXd> info_dict,
                                 std::vector<int> pose_ids,
                                 std::string which)
    {
        std::vector<Eigen::Affine3d> odom_vec;
        std::vector<Eigen::MatrixXd> info_vec;
        std::vector<int> succeed;

        dict_from_vec(odom_vec, info_vec, succeed, odom_dict, info_dict, pose_ids);

        int initial_id;
        if (which == "hyp")
            initial_id = local_num;
        else
            initial_id = 0;

        std::cout << "succeed size is " << succeed.size() << std::endl;
        std::cout << pose_ids.size() - 1 << std::endl;

        for (size_t pose_between{0}; pose_between < pose_ids.size() - 1; ++pose_between)
        {
            if (succeed.at(pose_between) == 1)
            {
                g2o::EdgeSE3 *odom_constraint = new g2o::EdgeSE3();

                odom_constraint->vertices()[0] = optimizer.vertex(initial_id);
                odom_constraint->vertices()[1] = optimizer.vertex(initial_id + 1);

                // conert affine3d -> isometry3d
                Eigen::Isometry3d transform;
                std::cout << odom_vec[pose_between].matrix() << std::endl;
                transform.translation() = odom_vec[pose_between].translation();
                transform.linear() = odom_vec[pose_between].rotation();

                odom_constraint->setMeasurement(transform);
                odom_constraint->setInformation(info_vec[pose_between]);
                optimizer.addEdge(odom_constraint);
            }
            initial_id += 1;

            // odometry_edges.push_back(odom_constraint)
        }
    }

    void optimize(int iteration, std::vector<int> local_pose, std::vector<int> hyp_pose)
    {
        optimizer.initializeOptimization();
        optimizer.setVerbose(true);
        optimizer.optimize(iteration);

        for (size_t vertex_id{0}; vertex_id < local_pose.size() + hyp_pose.size(); ++vertex_id)
        {
            g2o::VertexSE3Expmap *v = dynamic_cast<g2o::VertexSE3Expmap *>(optimizer.vertex(vertex_id));
            g2o::SE3Quat pose = v->estimate();
            std::cout << "------------------------------------" << std::endl;
            std::cout << vertex_id << std::endl;
            std::cout << "------------------------------------" << std::endl;
            std::cout << pose.to_homogeneous_matrix() << std::endl;
            std::cout << "====================================" << std::endl;
        }

        int project_inliers, odom_inliers;

        for (auto e : project_edges)
        {
            e->computeError();
            if (e->chi2() <= 1)
                project_inliers++;
        }

        // for (auto e : odometry_edges)
        // {
        //     e->computeError();
        //     if (e->chi2() <= 1)
        //         odom_inliers++;
        // }

        std::cout << "project inliers in totals: " << project_inliers << "/" << (point_id - pose_id) * 2 << std::endl;
        // std::cout << "odom inliers in totals: " << odom_inliers << "/" << odometry_edges.size() << std::endl;

        // return pose.to_homogeneous_matrix();
    }
};

#endif
