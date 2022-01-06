using namespace cv;
using namespace std;
#include <iostream>

int main(int argc, char **argv)
{
    Mat img_1, img_2;
    Mat R_f, t_f; //the final rotation and tranlation vectors containing the

    double scale = 1.00;
    char filename1[200];
    char filename2[200];
    sprintf(filename1, "/home/ayumi/Documents/tohoku_uni/CLOVERs/c-slam/relative_pose_estimation/image/1.jpg");
    // %06は数を6回繰り返させる
    // output:/home/avisingh/Datasets/KITTI_VO/00/image_2/000000.png
    sprintf(filename2, "/home/ayumi/Documents/tohoku_uni/CLOVERs/c-slam/relative_pose_estimation/image/2.jpg");

    //read the first two frames from the dataset
    Mat img_1_c = imread(filename1);
    Mat img_2_c = imread(filename2);

    if (!img_1_c.data || !img_2_c.data)
    {
        std::cout << " --(!) Error reading images " << std::endl;
        return -1;
    }

    // we work with grayscale images
    cvtColor(img_1_c, img_1, COLOR_BGR2GRAY);
    cvtColor(img_2_c, img_2, COLOR_BGR2GRAY);

    // feature detection, tracking
    vector<Point2f> points1, points2; //特徴点の座標

    featureDetection(img_1, points1); //detect features in img_1
    vector<uchar> status;
    featureTracking(img_1, img_2, points1, points2, status); //track those features to img_2



    //TODO: add a fucntion to load these values directly from KITTI's calib files
    // WARNING: different sequences in the KITTI VO dataset have different intrinsic/extrinsic parameters
    double focal = 525.0;
    cv::Point2d pp(319.5, 239.5);
    //recovering the pose and the essential matrix
    Mat E, R, t, mask;

    cout << points1 << endl;
    cout << points2 << endl;
    E = findEssentialMat(points2, points1, focal, pp, RANSAC, 0.999, 1.0, mask);
    // 対応点の情報を渡して基本行列から回転行列とtranslation vectorを算出
    recoverPose(E, points2, points1, R, t, focal, pp, mask);
    // R,tが出力？？void関数

    Mat prevImage = img_2;
    Mat currImage;


    cout << R << endl;
    cout << t << endl;

    

    return 0;
}