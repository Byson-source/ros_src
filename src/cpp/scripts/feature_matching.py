import rospy
import open3d as o3d
from sensor_msgs.msg import Image as msg_Image
from sensor_msgs.msg import CameraInfo
from cv_bridge import CvBridge, CvBridgeError
import numpy as np
import pyrealsense2 as rs2
import os
from rtabmap_ros.msg import Info
from cpp.msg import MultiArray
from cpp.msg import FeatureArray
from cpp.msg import HomogeneousArray
# NOTE 例えばr12の場合、r1がloopを検知した画像ペアのうち、r2が撮った写真上の特徴点の情報を格納している
# NOTE 写真一枚ごと
# NOTE [x1,y1,depth1,x2,y2,depth2...]

import message_filters
import cv2

rgb_path = "/home/ayumi/Documents/tohoku_uni/CLOVERs/images/all_rgb/"
home = "/home/ayumi/Documents/tohoku_uni/CLOVERs/images/"
depth_img = 1
depth_img2 = 2
container = {}
bridge = CvBridge()
intrinsics = rs2.intrinsics()
pinhole_camera_intrinsic = o3d.io.read_pinhole_camera_intrinsic(
    "/home/ayumi/Open3D/examples/test_data/realsense.json")


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


def orbmatch(fileName1, fileName2, previous_features=False):
    img1 = cv2.imread(rgb_path+str(fileName1)+".jpg")
    img2 = cv2.imread(rgb_path+str(fileName2)+".jpg")

    # ORB検出器生成
    orb = cv2.ORB_create(10000)

    kp1, des1 = orb.detectAndCompute(img1, None)
    kp2, des2 = orb.detectAndCompute(img2, None)

    kp1_loc, kp2_loc = [], []

    FLANN_INDEX_LSH = 6
    index_params = dict(algorithm=FLANN_INDEX_LSH,
                        table_number=6,  # 12
                        key_size=14,     # 20
                        multi_probe_level=1)  # 2
    search_params = dict(checks=100)

    flann = cv2.FlannBasedMatcher(index_params, search_params)

    # マッチング
    try:
        knn_matches = flann.knnMatch(des1, des2, k=2)
    except cv2.error:
        rospy.loginfo("Can't detect enough features...")
    else:
        # マッチング結果を描画
        ratio_thresh = 0.7
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

                kp1_loc.append((int(x1), int(y1)))
                kp2_loc.append((int(x2), int(y2)))

        loc1_, loc2_ = [], []
        features_map = {}
        detection_index = 0
        for index_, element in enumerate(kp1_loc):
            # true_mask.append(good_matches[index_])
            first_img_kpt = list(kp1_loc[index_])
            second_img_kpt = list(kp2_loc[index_])
            # NOTEどちらか一方の特徴点の深度が0だったら抽出しない。
            if(container[fileName1][int(first_img_kpt[1]), int(first_img_kpt[0])] != 0 and
               container[fileName2][int(second_img_kpt[1]), int(second_img_kpt[0])] != 0):
                loc1_.append(first_img_kpt)
                loc2_.append(second_img_kpt)
                features_map[detection_index] = [
                    loc1_[-1], loc2_[-1]]
                detection_index += 1

        img_matches = np.empty(
            (max(img1.shape[0], img2.shape[0]), img1.shape[1]+img2.shape[1], 3), dtype=np.uint8)

        img3 = cv2.drawMatches(img1, kp1, img2, kp2, good_matches, img_matches,
                               flags=cv2.DrawMatchesFlags_NOT_DRAW_SINGLE_POINTS)

        cv2.imwrite(
            "/home/ayumi/Documents/tohoku_uni/CLOVERs/images/feature_match/"+str(fileName1)+"->"+str(fileName2)+".jpg", img3)
        if previous_features:
            return loc1_, loc2_, features_map, 1
        else:
            return loc2_, features_map, 1

        # return [], [], 0
        ############################################################################
        # NOTE RANSAC
        # src_pts = np.float32(
        #     [kp1[m.queryIdx].pt for m in good_matches]).reshape(-1, 1, 2)
        # dst_pts = np.float32(
        #     [kp2[m.trainIdx].pt for m in good_matches]).reshape(-1, 1, 2)

        # try:
        #     M, mask = cv2.findHomography(src_pts, dst_pts, cv2.RANSAC, 1)
        # except cv2.error:
        #     rospy.loginfo("Not enough features...")
        # else:
        #     mask = mask.ravel().tolist()
        #     loc1_, loc2_, true_mask = [], [], []
        #     features_map = {}
        #     detection_index = 0
        #     for index_, element in enumerate(mask):
        #         if element == 1:
        #             true_mask.append(good_matches[index_])
        #             loc1_.append(list(kp1_loc[index_]))
        #             loc2_.append(list(kp2_loc[index_]))
        #             features_map[detection_index] = [
        #                 loc1_[-1], loc2_[-1]]
        #             detection_index += 1

        #     img_matches = np.empty(
        #         (max(img1.shape[0], img2.shape[0]), img1.shape[1]+img2.shape[1], 3), dtype=np.uint8)
        #     img3 = cv2.drawMatches(img1, kp1, img2, kp2, true_mask, img_matches,
        #                            flags=cv2.DrawMatchesFlags_NOT_DRAW_SINGLE_POINTS)

        # img_rect = cv2.circle(
        #     img1, (int(loc1[0][0][0]), int(loc1[0][0][1])), 3, (255, 0, 255), thickness=1)


# NOTE　検出先の写真のidがかぶらないようにする。

###############################################################################

def avoid_duplication(hypothesises):
    # rospy.logerr(hypothesises)
    memory = []
    for hypothesis in hypothesises:
        if not hypothesis in memory:
            memory.append(hypothesis)
        else:
            alternative = memory[-1]+2
            memory.append(alternative)

    # rospy.logerr(memory)

    return memory


def decrease_image(list_ind, list_val):
    if len(list_ind) > 3:
        list_ind = [list_ind[i] for i in range(3)]
        list_val = [list_val[i] for i in range(3)]

    list_val = avoid_duplication(list_val)
    return list_ind, list_val


def choose_image(list_ind, list_val):
    initial_ind = list_ind[0]
    initial_val = list_val[0]

    go_past_num = 6

    for i in range(len(list_ind)):
        if i > 0:
            initial_ind -= 2*go_past_num
            initial_val -= 2*go_past_num
            list_ind[i] = initial_ind
            list_val[i] = initial_val

    return list_ind, list_val


def image_manager(list_ind, list_val):
    list_ind, list_val = decrease_image(list_ind, list_val)
    # list_ind, list_val = choose_image(list_ind, list_val)

    return list_ind, list_val
###############################################################################


def add_error(input_list, x, y):

    error = [[x, y]]*len(input_list)
    errored_list = []

    for i in range(len(input_list)):
        list_ = []
        for a, b in zip(input_list[i], error[i]):
            list_.append(a+b)
        errored_list.append(list_)
    return errored_list


def derive_duplicated_index(list1, list2, sorted_keys=None):

    error_list = []

    for i in range(4):
        error_list.append(i+1)
        error_list.append(-i-1)

    survived_index = []
    survived_local = []

    sorted_idx = {}
    sorthoge = 0
    if not sorted_keys is None:
        for sorted_index in sorted_keys:
            sorted_idx[sorthoge] = sorted_index
            sorthoge += 1
    else:
        for num_element in range(len(list1)):
            sorted_idx[sorthoge] = num_element
            sorthoge += 1

    for error_element_x in error_list:
        for error_element_y in error_list:
            list2_ = add_error(list2, error_element_x, error_element_y)

            list1_ = set(map(tuple, list1))
            list2_set = set(map(tuple, list2_))

            merged = list1_ & list2_set
            merged = list(merged)

            for feature_val in merged:
                feature_val = list(feature_val)
                index_hoge = list1.index(feature_val)
                local_hoge = list2_.index(feature_val)

                if (sorted_idx[index_hoge] not in survived_index) and (local_hoge not in survived_local):
                    survived_index.append(sorted_idx[index_hoge])
                    survived_local.append(local_hoge)
                    # 重ならないようにする
    return survived_index, survived_local


def derive_duplicated_indexes(indexes, values):
    survived_index, survived_backup = [], []
    # NOTE 枚数が多い場合減らす+大きく過去の写真を参照するようにする。
    indexes, values = image_manager(indexes, values)
    # rospy.logwarn(indexes)

    second_kpt, feature_map, good = orbmatch(indexes[0], values[0])
    sorted_index, dict_list = [], [feature_map]

    for i in range(len(indexes)):
        sorted_index.append(indexes[i])
        sorted_index.append(values[i])
        # rospy.logwarn(str(indexes[i])+"->"+str(values[i]))

    valid_img_index = [sorted_index[0], sorted_index[1]]
    for hogehoge in range(2, len(sorted_index)):
        first_kpt, second_kpt_, feature_map_, good_ = orbmatch(
            sorted_index[hogehoge-1], sorted_index[hogehoge], True)

        if hogehoge > 2:
            survived_index, local_survivor = derive_duplicated_index(
                second_kpt, first_kpt, feature_map.keys())
        else:
            survived_index, local_survivor = derive_duplicated_index(
                second_kpt, first_kpt)
        # rospy.logwarn("Survived index!")
        # rospy.logwarn("==============================")
        # rospy.logwarn(survived_index)
        # rospy.logwarn("==============================")
        # rospy.logwarn("first map")
        # rospy.logwarn("==============================")
        # rospy.logwarn(feature_map)
        # rospy.logwarn("==============================")
        # rospy.logwarn("local index")
        # rospy.logwarn("==============================")
        # rospy.logwarn(local_survivor)
        # rospy.logwarn("==============================")
        # rospy.logwarn("second map")
        # rospy.logwarn("==============================")
        # rospy.logwarn(feature_map_)

        # NOTE 共通の特徴点が見つからなかった時
        if len(survived_index) < 3:
            # NOTE ペアから抽出した特徴点の中に共通のものが見いだせない時、その次のペアを試す。
            if len(dict_list) == 1:
                if hogehoge == len(sorted_index)-2:
                    # NOTE バンドル調整不可（共通の特徴点を持つ三枚以上の画像が存在しない）
                    rospy.logerr("Can't find corresponding features...Quit...")
                    return 0, 0, False,

                rospy.logwarn(
                    "There is no common features...Trying next pairs...")
                second_kpt, feature_map, dict_list[0] = second_kpt_, feature_map_, feature_map
                continue
            # NOTE バンドル調整に移行
            survived_index = survived_backup
            break

        valid_img_index.append(sorted_index[hogehoge])
        new_map = {}
        next_second = []
        for hogeee in range(len(survived_index)):
            new_map[survived_index[hogeee]] = feature_map_[
                local_survivor[hogeee]]
            next_second.append(feature_map_[
                local_survivor[hogeee]][1])

        dict_list.append(new_map)

        second_kpt = next_second
        feature_map = new_map
        survived_backup = survived_index
    # 上位のdictの間引きを行う
    new_map = []
    second_map = []

    # print(dict_list)
    # rospy.logerr(survived_index)

    for srv in survived_index:
        new_map.append(dict_list[0][srv][0])
        second_map.append(dict_list[0][srv][1])

    pack = [new_map, second_map]

    for dictionary_index in range(1, len(dict_list)):
        new_map = []
        for srv in survived_index:
            new_map.append(dict_list[dictionary_index][srv][1])
        pack.append(new_map)
        # 最初の画像の
    return pack, valid_img_index, True


def arrange_ids(length):
    list1 = []
    list2 = []
    for val in range(length):
        if val % 2 == 0:
            list1.append(val)
        else:
            list2.append(val)
    map_id = {}
    input_keyhoge = 0
    for val in list1:
        map_id[val] = input_keyhoge
        input_keyhoge += 1

    for val in list2:
        map_id[val] = input_keyhoge
        input_keyhoge += 1

    return map_id


def loop_CB(data):

    loop_dict = {"R1": {"R1": {}, "R2": {}},
                 "R2": {"R1": {}, "R2": {}}}
    # NOTE {"R1":{1:[x1,y1,d1,x2,y2,d2,...],2:...} ,"R2":{1:[x1,y1,d1,x2,y2,d2,...],2:[]...}}
    # NOTE R1とR2
    for index, element in loop_dict.items():
        referred, referred_hyp = 0, 0
        good_ = 0
        feature_map, valid_img = [], []
        # 各検知の写真の枚数

        if index == "R1":
            element["num"] = len(data.r1_index)
            if element["num"] > 1:
                # if(data.r1_index[-1] > 10) and (data.r1_value[-1] > 10):
                referred = data.r1_index[0]
                referred_hyp = data.r1_value[0]
                feature_map, valid_img, good_ = derive_duplicated_indexes(
                    data.r1_index, data.r1_value)
                # else:
                #     rospy.logwarn("Indexes are too young... quit")
                #     good_ = 0
        else:
            element["num"] = len(data.r2_index)
            if element["num"] > 1:
                # if(data.r2_index[-1] > 10) and (data.r2_value[-1] > 10):
                referred = data.r2_index[0]
                referred_hyp = data.r2_value[0]
                feature_map, valid_img, good_ = derive_duplicated_indexes(
                    data.r2_index, data.r2_value)
                # else:
                #     rospy.logwarn("Indexes are too young... quit")
                #     good_ = 0

        answer = HomogeneousArray()
        info = FeatureArray()
        rospy.loginfo("valid img is")
        rospy.logwarn(valid_img)

        if index == "R1":
            answer.who_detect = "R1"
        else:
            answer.who_detect = "R2"

        if good_ and element["num"] > 1:
            kpt_map = arrange_ids(len(feature_map))
            # rospy.logwarn(kpt_map)
            # element["R1"][iter], element["R2"][iter] = [], []
            for kpt in range(len(feature_map)):
                whether_success = 1
                # r_feature = []
                # for each_kpt in feature_map[kpt]:
                #     r_feature.append(each_kpt)
                r_feature = feature_map[kpt]
                img_coord = []
                # rospy.loginfo(r_feature)

                for point in range(len(r_feature)):
                    # elif(container[indice][int(r_feature[point][1]), int(r_feature[point][0])] != 0):

                    img_coord.append(int(r_feature[point][0]))
                    img_coord.append(int(r_feature[point][1]))
                    img_coord.append(0)

                # rospy.logerr(point_coord)
                # 何枚目なのか
                rospy.logwarn(img_coord)

                info.img_coord = img_coord
                if kpt % 2 == 0:
                    info.me = "loop"
                else:
                    info.me = "hyp"

                info.id = kpt_map[kpt]
                feature_pub.publish(info)

            # NOTE 点群が三つ以上観測されたら
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

            odom_result = []

            if not success_hybrid_term:
                rospy.loginfo("Can not compute RGBD Odometry...")
            else:
                print(trans_hybrid_term)
                for value in trans_hybrid_term:
                    for value_ in value:
                        odom_result.append(value_)
                answer.data = odom_result
                answer.valid_img = valid_img

                point_coord = []
                for initial_kpt in feature_map[0]:
                    depth_r = container[valid_img[0]][int(
                        initial_kpt[1]), int(initial_kpt[0])]

                    result = rs2.rs2_deproject_pixel_to_point(
                        intrinsics, [int(initial_kpt[0]), int(initial_kpt[1])], depth_r)

                    for k in range(3):
                        point_coord.append(result[k])
                answer.r_3d = point_coord

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
    odometry_pub = rospy.Publisher(
        "odometry_result", HomogeneousArray, queue_size=10)

    rospy.spin()
