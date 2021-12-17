#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encoding.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/CameraInfo.h>
#include <std_msgs/Int32.h>

using namespace sensor_msgs;
using namespace message_filters;

static const std::string OPENCV_WINDOW = "Image window";

class ImageConverter{
    private:
    int robot_number;

    int rgb_index{0};
    int depth_index{0};

    std::string rgb_path{"/home/ayumi/Documents/tohoku_uni/images/1_rgb"};
    std::string depth_path{"/home/ayumi/Documents/tohoku_uni/images/1_depth"};

    std::string all_rgb{"/home/ayumi/Documents/tohoku_uni/images/all_rgb"};
    std::string all_rgb{"/home/ayumi/Documents/tohoku_uni/images/all_depth"};

    ros::NodeHandle n;
    image_transport::ImageTransport it_;
    image_transport::Subscriber rgb_sub;
    image_transport::Subscriber depth_sub;
    // Check if there is 4 img or not
    image_transport::Publisher commander;

    public:
    ImageConverter(int num):it_(n),robot_number(num){

        rgb_sub=it_.subscribe("robot";std::to_string(robot_number)+"/camera/color/image_raw",1,
        &ImageConverter::rgbCB,this);
        depth_sub=it_.subscribe("robot";std::to_string(robot_number)+"/camera/depth/image_raw",1,
        &ImageConverter::depthCB,this);

        commander.advertise("robot";std::to_string(robot_number)+"command_RO_estimate",1);

    }

    void rgbCB(const sensor_msgs::ImageConstPtr& msg){
        rgb_index+=1;
        cv_bridge::CvImagePtr cv_ptr;
        cv_ptr=cv_bridge::toCvCopy(msg,sensor_msgs::image_encodings::BGR8);
        cv::imwrite(std::to_string(rgb_index)+".jpg",cv_ptr->image);

    }

    }

void callback(const ImageConstPtr &image, const CameraInfoConstPtr &cam_info)
{
    // Solve all of perception here...
}

int main(int argc, char **argv)
{
    ros::init(argc,argv,"image_extractor_node");
    ros::NodeHandle pnh("~");
    int robot_No;
    pnh.getParam("robot_No",robot_No);

    ImageConverter(robot_No);
    
    message_filters::Subscriber<Image> image_sub(nh, "image", 1);
    message_filters::Subscriber<CameraInfo> info_sub(nh, "camera_info", 1);
    TimeSynchronizer<Image, CameraInfo> sync(image_sub, info_sub, 10);
    sync.registerCallback(boost::bind(&callback, _1, _2));

    ros::spin();

    return 0;
}


int main(int argc,char **argv){
    ros::init(argc,argv,"image_extractor_node");
    ros::NodeHandle pnh("~");
    int robot_No;
    pnh.getParam("robot_No",robot_No);

    ImageConverter(robot_No);

    ros::spin();

    return 0;
}