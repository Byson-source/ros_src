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
        std::vector<Eigen::Matrix4d> all_poses;
        for (auto val : hyp_poses)
            local_poses.push_back(val);
        all_poses = local_poses;

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
        // idの順番を保持したままひとまとめにする。
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
            }
            ++point_id;
        }
    }

    Eigen::Matrix4d optimize(int iteration, int id)
    {
        optimizer.initializeOptimization();
        optimizer.setVerbose(true);
        optimizer.optimize(iteration);

        g2o::VertexSE3Expmap *v = dynamic_cast<g2o::VertexSE3Expmap *>(optimizer.vertex(id));
        g2o::SE3Quat pose = v->estimate();

        return pose.to_homogeneous_matrix();
    }
};

#endif
