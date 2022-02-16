
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

#include <visualization_msgs/Marker.h>

#include <ros/ros.h>
#include <string>
#include <vector>
#include "cpp/FeatureArray.h"
#include "cpp/RO_Array.h"
#include <math.h>
// #include "calibration.h"
// FIXME CMakeLists.txtを修正
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
    ros::Publisher marker_pub;

    visualization_msgs::Marker points, line_strip, line_list;

    std::vector<int> loop_info;
    // NOTE xyz / xyzw
    std::vector<std::vector<double>> path_1;
    std::vector<std::vector<double>> path_2;

    std::map<int, int> mapPath_dict;
    std::map<int, int> mapPath_dict_2;
    std::map<std::string, std::map<std::string, std::vector<Eigen::Vector3d>>> feature_dict;
    // NOTE {"R1":{"3d":[[x1,y1,z1],[x2,y2...]],"2d":[[x1,y1,0],[x2,y2,0],....]}}...}
    int info_index{1};
    int info_index_2{1};

    int status{0};

    int status{0};

    Eigen::Matrix4d transformation_result;
    Eigen::Matrix3d cam2robot;
    Eigen::Matrix3d robot2cam;

    Eigen::Matrix3d draw_rotation;
    Eigen::Vector3d draw_t;

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

        std::map<std::string, std::vector<Eigen::Vector3d>> correspondence_2d3d;
        feature_dict["R1"] = correspondence_2d3d;
        feature_dict["R2"] = correspondence_2d3d;

        cam2robot << 0, -1, 0,
            0, 0, -1,
            1, 0, 0;

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

    void odom_CB(const cpp::HomogeneousArray::ConstPtr &data_)
    {

        transformation_result << data_->data[0], data_->data[1], data_->data[2], data_->data[3],
            data_->data[4], data_->data[5], data_->data[6], data_->data[7],
            data_->data[8], data_->data[9], data_->data[10], data_->data[11],
            0.0, 0.0, 0.0, 1.0;

        // transformation_result=cam2robot*transformation_result;

        Eigen::Vector3d transfer_;
        Eigen::Matrix3d rotation_;
        transfer_ = transformation_result.block(0, 3, 3, 1);
        rotation_ = transformation_result.block(0, 0, 3, 3);

        rotation_ = cam2robot.transpose() * rotation_ * cam2robot;
        transfer_ = cam2robot.transpose() * transfer_;

        cpp::RO_Array pose_result;
        int who_detect = data_->who_detect;
        Eigen::Vector3d ans_t = turnout_T(transfer_, who_detect);
        Eigen::Quaterniond ans_r = turnout_R(rotation_, who_detect);

        std::vector<double> R{ans_r.w(), ans_r.x(), ans_r.y(), ans_r.z()};
        std::vector<double> t{ans_t[0], ans_t[1], ans_t[2]};

        pose_result.translation = t;
        pose_result.euler = R;

        rt_pub.publish(pose_result);

        draw_rotation = ans_r.matrix();
        draw_t = ans_t;
        status = 1;
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
            r1_img_index = (loop_info[0] - 1) * 0.5 + 1;
            r2_img_index = loop_info[1] / 2;
        }
        else
        {
            rotation_1TO2 = rotation_matrix.transpose();
            r1_img_index = (loop_info[1] - 1) * 0.5 + 1;
            r2_img_index = loop_info[0] / 2;
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
};

int main(int argc, char **argv)
{
    ros::init(argc, argv, "RO_estimator");
    RO_Estimator detector;

    ros::spin();

    return 0;
}
