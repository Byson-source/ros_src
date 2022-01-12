#include <iostream>
#include <opengv/absolute_pose/methods.hpp>
#include <opengv/absolute_pose/CentralAbsoluteAdapter.hpp>
#include <opengv/math/cayley.hpp>
#include <Eigen/Core>
#include <Eigen/Geometry>

#include <ros/ros.h>
#include <string>
#include <vector>
#include "cpp/MultiArray.h"
#include "cpp/RO_Array.h"
#include "tf2_msgs/TFMessage.h"

class RO_Estimator
{
private:
    ros::NodeHandle n;
    ros::Subscriber feature_sub;
    ros::Subscriber pose_sub;

    Eigen::Matrix<float,3,4> pose;

public:
    RO_Estimator()
    {
        feature_sub=n.subscribe("features",20,&RO_estimator::RO_CB,this);
        // from robot/map to robot/base_footprint
        pose_sub=n.subscribe("tf",20,&RO_estimator::poseCB,this);

    }

    void RO_CB(const std_msgs::cpp::ConstPtr &data){
        // bearing vectors
        Eigen::MatrixXd cov_xx;
        Eigen::MatrixXd cov_ldld;
        points_t points;
        // FIXME pointsの座標はcalibration.hで求める
        Eigen::MatrixXd gt(3,$numberofPoints);
        // FIXME number of points,店の三次元座標

        opengv::bearingVectors_t bearingVectors;
        size_t numberCams{1};
        size_t caamCorrespondence{0};
        // FIXME number of points
        for( size_t i = 0; i < $Number of points; i++ ){
            translation_t camOffset =Eigen::Vector3d::Zero();
            rotation_t camRotation=Eigen::Matrix3d::Identity();
            points.push_back(gt.col(i));

            point_t bodyPoint = rotation.transpose()*(gt.col(i) - position);
            // FIXME multiThreadの必要？？どこかでPoseを知る
        }

        void poseCB(const tf2_msgs::TFMessage &data){
            
        }

        
    }



    
};