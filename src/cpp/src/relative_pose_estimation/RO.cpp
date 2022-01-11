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

class RO_Estimator
{
private:
    ros::NodeHandle n;
    ros::Subscriber feature_sub;

public:
    RO_Estimator()
    {
        feature_sub=n.subscribe("features",20,&RO_estimator::RO_CB,this);

    }

    void RO_CB(const std_msgs::cpp::ConstPtr &data){
        
    }



    
};