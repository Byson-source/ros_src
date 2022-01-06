// #include <ros/ros.h>
// #include <sensor_msgs/Image.h>
// #include <sensor_msgs/image_encodings.h>
// #include <cv_bridge/cv_bridge.h>

#include <librealsense2/rs.hpp>
#include <stdio.h>
#include <iostream>
#include <stdio.h>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/opencv.hpp>

#include <opengv/absolute_pose/methods.hpp>
#include <opengv/absolute_pose/CentralAbsoluteAdapter.hpp>
#include <opengv/math/cayley.hpp>

#include <vector>
#include <string>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <opencv2/core/eigen.hpp>

// ros
#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <std_msgs/Int32MultiArray.h>

using namespace std;

class Depth_listener
{
private:
    ros::NodeHandle n;
    ros::Subscriber depth_sub;

public:
    float distanceVal{0};
    float x,y;

    Depth_listener(std::string topic_name)
    {
        depth_sub = n.subscribe < sensor_msgs::Image>(topic_name,10,&Depth_listener::imgcb,this);
    }
    void imgcb(const sensor_msgs::Image::ConstPtr &msg)
    {
        try
        {
            cv_bridge::CvImageConstPtr cv_ptr;
            cv_ptr = cv_bridge::toCvShare(msg);

            //get image dimension once
            static bool runOnce = true;
            if (runOnce)
            {
                cout << "Image dimension (Row,Col): " << cv_ptr->image.rows << " x " << cv_ptr->image.cols << endl;
                runOnce = false;
            }

            // //get global max depth value
            // double max = 0.0;
            // cv::minMaxLoc(cv_ptr->image, 0, &max, 0, 0);
            // std::cout << "Max value: " << max << endl;

            // //get global min depth value
            // double min = 0.0;
            // cv::minMaxLoc(cv_ptr->image, &min, &max, 0, 0);
            // std::cout << "Min value: " << min << endl;

            // get depth value at a point
            distanceVal = cv_ptr->image.at<float>(x, y);
            std::cout << "Distance value: " << distanceVal << "m" << endl;
        }
        catch (const cv_bridge::Exception &e)
        {
            ROS_ERROR("cv_bridge exception: %s", e.what());
        }
    }
}

int main(int argc, char **argv)
{
    // ros::init(argc,argv,"depth_listener");
    // Depth_listener sub("camera/depth/image_raw");
    // Sub=Depth_listener()

    cv::Mat img_1 = cv::imread("/home/ayumi/Documents/tohoku_uni/CLOVERs/c-slam/relative_pose_estimation/image/1.jpg");
    cv::Mat img_2 = cv::imread("/home/ayumi/Documents/tohoku_uni/CLOVERs/c-slam/relative_pose_estimation/image/2.jpg");

    cv::imshow("result",img_1);

    if (!img_1.data || !img_2.data)
    {
        cout << " --(!) Error reading images " << endl;
        return -1;
    }

    // -- Step 1: Detect the keypoints using SURF Detector, compute the descriptors
    vector<cv::KeyPoint> keypoints_1;
    vector<cv::KeyPoint> keypoints_2;

    cv::Mat descriptor1;
    cv::Mat descriptor2;

    vector<cv::Point2f> kp1_loc;
    vector<cv::Point2f> kp2_loc;

    cv::Ptr<cv::FeatureDetector> detector = cv::ORB::create(1000);

    // find the keypoints and descriptors with ORB
    // keypoints
    detector->detect(img_1, keypoints_1);
    detector->detect(img_2, keypoints_2);

    // descriptors
    cv::Ptr<cv::DescriptorExtractor> extractor = cv::ORB::create(1000);
    extractor->compute(img_1, keypoints_1, descriptor1);
    extractor->compute(img_2, keypoints_2, descriptor2);

    // Flann needs the descriptors to be of type CV_32F
    descriptor1.convertTo(descriptor1, CV_32F);
    descriptor2.convertTo(descriptor2, CV_32F);

    //-- Step 2: Matching descriptor vectors using FLANN matcher
    cv::FlannBasedMatcher matcher;
    std::vector<cv::DMatch> matches;
    matcher.match(descriptor1, descriptor2, matches);

    double max_dist = 0;
    double min_dist = 100;

    //-- Quick calculation of max and min distances between keypoints
    for (int i = 0; i < descriptor1.rows; i++)
    {
        double dist = matches[i].distance;
        if (dist < min_dist)
            min_dist = dist;
        if (dist > max_dist)
            max_dist = dist;
    }

    printf("-- Max dist : %f \n", max_dist);
    printf("-- Min dist : %f \n", min_dist);

    const double threshold = 2 * min_dist;
    // RANSAC使わないほうが良くね？？

    std::vector<cv::DMatch> good_matches;

    for (int i = 0; i < matches.size(); i++)
    {
        if (matches[i].distance <= threshold)
        // max関数は2つの値から大きい方を返す
        {
            good_matches.push_back(matches[i]);
        }
    }

    for (int i = 0; i < (int)good_matches.size(); i++)
    {
        printf("-- Good Match [%d] Keypoint 1: %d  -- Keypoint 2: %d  \n", i,
               good_matches[i].queryIdx, good_matches[i].trainIdx);

        cv::Point2f point1_loc(keypoints_1[good_matches[i].queryIdx].pt.x,
                               keypoints_1[good_matches[i].queryIdx].pt.y);
        cv::Point2f point2_loc(keypoints_2[good_matches[i].queryIdx].pt.x,
                               keypoints_2[good_matches[i].queryIdx].pt.y);
        kp1_loc.push_back(point1_loc);
        kp2_loc.push_back(point2_loc);
    }

    // RANSAC
    // 含めるべきかどうかは再考するべき
    // cv::Mat masks;
    // cv::Mat H=cv::findHomography(kp1_loc,kp2_loc,masks,cv::RANSAC,1);

    // vector<cv::DMatch> inlierMatch;

    // cout<<masks.rows<<endl;
    // for(size_t i=0;i<masks.rows;++i){
    //     uchar *inlier=masks.ptr<uchar>(i);
    //     if(inlier[0]==1){
    //         inlierMatch.push_back(good_matches[i]);
    //     }
    // }

    cv::Mat img_matches;
    cv::drawMatches(img_1, keypoints_1, img_2, keypoints_2,
                    good_matches, img_matches);

    //TODO: add a fucntion to load these values directly from KITTI's calib files
    // WARNING: different sequences in the KITTI VO dataset have different intrinsic/extrinsic parameters
    double focal = 525.0;
    cv::Point2d pp(319.5, 239.5);
    //recovering the pose and the essential matrix
    cv::Mat E, R, t, mask;

    // cv::imshow("result",img_matches);
    // cv::waitKey();

    //     transformation_t mlpnp_transformation;
    //   gettimeofday(&tic, 0);
    //   for (size_t i = 0; i < iterations; i++)
    // 	  mlpnp_transformation = absolute_pose::mlpnp(adapter);
    //   gettimeofday(&toc, 0);



    // opengv::bearingVectors_t bearingVectors;
    // // Eigen::Vector3dのvector
    // opengv::points_t points;
    // // points=Eigen::Vector3d+vector
    // opengv::rotations_t rotation{Eigen::Matrix3d::Identity()};
    // // rotation=Eigen::Matrix3d+vector;
    // opengv::cov3_mats_t emptyCovariances;
    // // emptyCovariances Eigen::Matrix3d+vector
    // opengv::transformation_t mlpnp_transformation;
    // // Eigen::matrix,double,3,4>

    // opengv::absolute_pose::CentralAbsoluteAdapter adapter(
    //     bearingVectors,
    //     points,
    //     rotation,
    //     emptyCovariances);

    // size_t iterations{50};
    // Eigen::MatrixXd cov_xx;
    // Eigen::MatrixXd cov_ldld;
    // for (size_t i{0}; i < iterations; i++)
    //     mlpnp_transformation = opengv::absolute_pose::mlpnp(adapter,cov_xx,cov_ldld);

    // Eigen::Matrix3d R_=mlpnp_transformation.block(0,0,3,3);
    // // 座標(0,0)から３×3の大きさの行列を取り出す。
    // Eigen::Vector3d T_=mlpnp_transformation.block(3,1,1,3);
    // ros::spin();

    return 0;
}