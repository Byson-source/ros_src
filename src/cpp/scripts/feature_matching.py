import rospy
import open3d as o3d
from sensor_msgs.msg import Image as msg_Image
from sensor_msgs.msg import CameraInfo
from cv_bridge import CvBridge, CvBridgeError
import sys
import os
import numpy as np
import pyrealsense2 as rs2
import os
from rtabmap_ros.msg import Info
from std_msgs.msg import Float32MultiArray
from cpp.msg import MultiArray
from cpp.msg import FeatureArray
# NOTE 例えばr12の場合、r1がloopを検知した画像ペアのうち、r2が撮った写真上の特徴点の情報を格納している
# NOTE 写真一枚ごと
# NOTE [x1,y1,depth1,x2,y2,depth2...]

import message_filters
import cv2

# if (not hasattr(rs2, 'intrinsics')):
#     import pyrealsense2.pyrealsense2 as rs2
rgb_path = "/home/ayumi/Documents/tohoku_uni/CLOVERs/images/all_rgb/"
home = "/home/ayumi/Documents/tohoku_uni/CLOVERs/images/"
depth_img = 1
depth_img2 = 2
container = {}
bridge = CvBridge()
index = 0
intrinsics = rs2.intrinsics()

pinhole_camera_intrinsic=o3d.io.read_pinhole_camera_intrinsic("/home/ayumi/Open3D/examples/test_data/realsense.json")


def depthCB1(depth1, id):
    global depth_img

    cv2_img = bridge.imgmsg_to_cv2(depth1, depth1.encoding)
    # FIXME Maybe should change to (640,480)
    cv2_img = cv2.resize(cv2_img, dsize=(640, 480))

    cv2.imwrite("/home/ayumi/Documents/tohoku_uni/CLOVERs/images/depth/" +
                str(depth_img)+".png", cv2_img)

    container[depth_img] = cv2_img
    depth_img += 2


def depthCB2(depth2, id):
    global depth_img2

    img = bridge.imgmsg_to_cv2(depth2, depth2.encoding)
    # FIXME Maybe should change to (640,480)
    img = cv2.resize(img, dsize=(640, 480))
    # print(depth_array.shape)

    container[depth_img2] = img

    cv2.imwrite("/home/ayumi/Documents/tohoku_uni/CLOVERs/images/depth/" +
                str(depth_img2)+".png", img)

    depth_img2 += 2

# Return feature locations
# TODO パラメーターの妥当性の確認


def info_CB(data):
    global intrinsics

    intrinsics.width = data.width
    intrinsics.width = data.width
    intrinsics.height = data.height
    intrinsics.ppx = data.K[2]
    intrinsics.ppy = data.K[5]
    intrinsics.fx = data.K[0]
    intrinsics.fy = data.K[4]
    if data.distortion_model == 'plumb_bob':
        intrinsics.model = rs2.distortion.brown_conrady
    elif data.distortion_model == 'equidistant':
        intrinsics.model = rs2.distortion.kannala_brandt4
    intrinsics.coeffs = [i for i in data.D]


def orbmatch(fileName1, fileName2):
    global index
    index += 1
    img1 = cv2.imread(rgb_path+str(fileName1)+".jpg")
    img2 = cv2.imread(rgb_path+str(fileName2)+".jpg")

    # ORB検出器生成
    orb = cv2.ORB_create(3000)

    kp1, des1 = orb.detectAndCompute(img1, None)
    kp2, des2 = orb.detectAndCompute(img2, None)

    kp1_loc, kp2_loc = [], []

    FLANN_INDEX_LSH = 6
    index_params = dict(algorithm=FLANN_INDEX_LSH,
                        table_number=6,  # 12
                        key_size=14,     # 20
                        multi_probe_level=1)  # 2
    search_params = dict(checks=50)

    flann = cv2.FlannBasedMatcher(index_params, search_params)

    # マッチング
    try:
        knn_matches = flann.knnMatch(des1, des2, k=2)
    except cv2.error:
        rospy.loginfo("Can't detect enough features...")
    else:
        # マッチング結果を描画
        ratio_thresh = 0.8
        good_matches = []
        for mt in knn_matches:

            if len(mt) < 2:
                continue
            m, n = mt[0], mt[1]

            # print(type(x1))
            if m.distance < ratio_thresh * n.distance:
                good_matches.append(m)
                img1_idx = m.queryIdx
                img2_idx = m.trainIdx

                (x1, y1) = kp1[img1_idx].pt
                (x2, y2) = kp2[img2_idx].pt

                kp1_loc.append((x1, y1))
                kp2_loc.append((x2, y2))

        src_pts = np.float32(
            [kp1[m.queryIdx].pt for m in good_matches]).reshape(-1, 1, 2)
        dst_pts = np.float32(
            [kp2[m.trainIdx].pt for m in good_matches]).reshape(-1, 1, 2)

        try:
            M, mask = cv2.findHomography(src_pts, dst_pts, cv2.RANSAC, 1)
        except cv2.error:
            rospy.loginfo("Not enough features...")
        else:
            mask = mask.ravel().tolist()
            loc1, loc2, true_mask = [], [], []

            for index, element in enumerate(mask):
                if element == 1:
                    true_mask.append(good_matches[index])
                    loc1.append(kp1_loc[index])
                    loc2.append(kp2_loc[index])
            img_matches = np.empty(
                (max(img1.shape[0], img2.shape[0]), img1.shape[1]+img2.shape[1], 3), dtype=np.uint8)
            img3 = cv2.drawMatches(img1, kp1, img2, kp2, true_mask, img_matches,
                                   flags=cv2.DrawMatchesFlags_NOT_DRAW_SINGLE_POINTS)

            if(len(loc1) > 9):
                # img_rect = cv2.circle(
                #     img1, (int(loc1[0][0][0]), int(loc1[0][0][1])), 3, (255, 0, 255), thickness=1)
                cv2.imwrite(
                    "/home/ayumi/Documents/tohoku_uni/CLOVERs/images/feature_match/"+str(fileName1)+"->"+str(fileName2)+".jpg", img3)

            return loc1, loc2, 1

    return [], [], 0


def loop_CB(data):

    loop_dict = {"R1": {"R1": {}, "R2": {}},
                 "R2": {"R1": {}, "R2": {}}}
    # NOTE {"R1":{1:[x1,y1,d1,x2,y2,d2,...],2:...} ,"R2":{1:[x1,y1,d1,x2,y2,d2,...],2:[]...}}
    # NOTE R1とR2
    referred, referred_hyp = 0, 0
    for index, element in loop_dict.items():
        print(index)
        i = 0

        # 各検知の写真の枚数
        if index == "R1":

            element["num"] = len(data.r1_index)
            if element["num"] > 0:
                referred = data.r1_index[int(element["num"]/2)]
                referred_hyp = data.r1_value[int(element["num"]/2)]
        else:
            element["num"] = len(data.r2_index)
            if element["num"] > 0:
                referred = data.r2_index[int(element["num"]/2)]
                referred_hyp = data.r2_value[int(element["num"]/2)]
        # NOTE 1ペア毎にpublishする

        for iter in range(element["num"]):
            element["R1"][iter+1], element["R2"][iter+1] = [], []
            i += 1
            indice1, indice2, good = 0, 0, 0
            r1_feature, r2_feature = [], []

            info = FeatureArray()
            if index == "R1":
                # filter
                # TODO 座標軸の定義を見直すこと.この場合は画像の左上
                r1_feature, r2_feature, good = orbmatch(
                    data.r1_index[iter], data.r1_value[iter])
                indice1, indice2 = data.r1_index[iter], data.r1_value[iter]
                info.index2value = [indice1, indice2]
                info.who_detect = 1
            else:
                r2_feature, r1_feature, good = orbmatch(
                    data.r2_index[iter], data.r2_value[iter])
                # indice1,indice2="value","index"
                indice2, indice1 = data.r2_index[iter], data.r2_value[iter]
                info.index2value = [indice2, indice1]
                info.who_detect = 2

            r1_coord, r2_coord = [], []
            # NOTE feature iterations
            # NOTE loopの画像だけで、BAを行うのか、他の画像も巻き込むのか
            if(good):
                # rospy.loginfo("This is No."+str(indice1))
                for index in range(len(r1_feature)):
                    if(container[indice1][int(r1_feature[index][1]), int(r1_feature[index][0])] != 0 and
                       container[indice2][int(r2_feature[index][1]), int(r2_feature[index][0])] != 0):

                        #    NOTE camera coordinateをpublishする

                        depth_r1 = container[indice1][int(
                            r1_feature[index][1]), int(r1_feature[index][0])]
                        depth_r2 = container[indice2][int(
                            r2_feature[index][1]), int(r2_feature[index][0])]

                        result_1 = rs2.rs2_deproject_pixel_to_point(
                            intrinsics, [int(r1_feature[index][0]), int(r1_feature[index][1])], depth_r1)
                        result_2 = rs2.rs2_deproject_pixel_to_point(
                            intrinsics, [int(r2_feature[index][0]), int(r2_feature[index][1])], depth_r2)

                        r1_coord.append(int(r1_feature[index][0]))
                        r1_coord.append(int(r1_feature[index][1]))
                        r1_coord.append(0)
                        r2_coord.append(int(r2_feature[index][0]))
                        r2_coord.append(int(r2_feature[index][1]))
                        r2_coord.append(0)

                        for k in range(3):
                            element["R1"][iter+1].append(result_1[k])
                            element["R2"][iter+1].append(result_2[k])

                info.signal = 0
                info.r1 = element["R1"][iter+1]
                info.r2 = element["R2"][iter+1]

                info.r1_imgcoord = r1_coord
                info.r2_imgcoord = r2_coord

                feature_pub.publish(info)
        # Loop sequence終了

    source_color = o3d.io.read_image(
        home+"all_rgb/"+str(referred)+".jpg")
    source_depth = o3d.io.read_image(
        home+"depth/"+str(referred)+".png")
    target_color = o3d.io.read_image(
        home+"all_rgb/"+str(referred_hyp)+".jpg")
    target_depth = o3d.io.read_image(
        home+"depth/"+str(referred_hyp)+".png")

    source_rgbd_image = o3d.geometry.RGBDImage.create_from_color_and_depth(
        source_color, source_depth)
    target_rgbd_image = o3d.geometry.RGBDImage.create_from_color_and_depth(
        target_color, target_depth)

    option = o3d.pipelines.odometry.OdometryOption()
    # option.max_depth=10
    odo_init = np.identity(4)

    [success_hybrid_term, trans_hybrid_term,
    info] = o3d.pipelines.odometry.compute_rgbd_odometry(
    source_rgbd_image, target_rgbd_image,
    pinhole_camera_intrinsic, odo_init,
    o3d.pipelines.odometry.RGBDOdometryJacobianFromHybridTerm(), option)

    odom_result=[]

    if not success_hybrid_term:
        rospy.loginfo("Can not compute RGBD Odometry...")
    else:
        print(trans_hybrid_term)
        for value in trans_hybrid_term:
            odom_result=[value_ for value_ in value]
        answer=Float32MultiArray()
        answer.data=odom_result
        odometry_pub.publish(answer)

if __name__ == '__main__':
    # node_name = os.path.basename(sys.argv[0]).split('.')[0]
    rospy.init_node("depth_listener")

    # /////////////////////////////////////////////////////////////////////////
    depth_topic = "/robot1/camera/aligned_depth_to_color/image_raw"
    depth_topic2 = "/robot2/camera/aligned_depth_to_color/image_raw"

    ID_topic = "/robot1/rtabmap/info"
    ID_topic2 = "/robot2/rtabmap/info"

    depth_sub = message_filters.Subscriber(depth_topic, msg_Image)
    depth_sub2 = message_filters.Subscriber(depth_topic2, msg_Image)

    myid_sub = message_filters.Subscriber(ID_topic, Info)
    myid_sub2 = message_filters.Subscriber(ID_topic2, Info)

    ts = message_filters.ApproximateTimeSynchronizer(
        [depth_sub, myid_sub], 10, 0.1, allow_headerless=False)
    ts.registerCallback(depthCB1)

    ts2 = message_filters.ApproximateTimeSynchronizer(
        [depth_sub2, myid_sub2], 10, 0.1, allow_headerless=False)
    ts2.registerCallback(depthCB2)

    cameraInfo_sub = rospy.Subscriber(
        "/robot1/camera/aligned_depth_to_color/camera_info", CameraInfo, info_CB)
    # /////////////////////////////////////////////////////////////////////////

    # /////////////////////////////////////////////////////////////////////////
    loop_sub = rospy.Subscriber("result", MultiArray, loop_CB)
    feature_pub = rospy.Publisher("features", FeatureArray, queue_size=10)
    odometry_pub=rospy.Publisher("odometry_result",Float32MultiArray,queue_size=10)

    rospy.spin()
