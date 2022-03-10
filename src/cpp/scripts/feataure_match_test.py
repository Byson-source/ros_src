import cv2
import numpy as np
rgb_path="/home/ayumi/Documents/tohoku_uni/CLOVERs/images/all_rgb/"
feature_path="/home/ayumi/Documents/tohoku_uni/CLOVERs/images/feature_match/"

image_list=[rgb_path+"30.jpg",rgb_path+"29.jpg",rgb_path+"33.jpg",rgb_path+"30.jpg",rgb_path+"41.jpg",rgb_path+"38.jpg",rgb_path+"43.jpg",rgb_path+"40.jpg",rgb_path+"45.jpg",rgb_path+"40.jpg"]


def orbmatch(fileName1, fileName2,idx):
    img1 = cv2.imread(fileName1)
    img2 = cv2.imread(fileName2)

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
    
    knn_matches = flann.knnMatch(des1, des2, k=2)

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

    
    M, mask = cv2.findHomography(src_pts, dst_pts, cv2.RANSAC, 1)


    mask = mask.ravel().tolist()
    loc1, loc2, true_mask = [], [], []

    for index_, element in enumerate(mask):
        if element == 1:
            true_mask.append(good_matches[index_])
            loc1.append(kp1_loc[index_])
            loc2.append(kp2_loc[index_])
    img_matches = np.empty(
        (max(img1.shape[0], img2.shape[0]), img1.shape[1]+img2.shape[1], 3), dtype=np.uint8)
    img3 = cv2.drawMatches(img1, kp1, img2, kp2, true_mask, img_matches,
                            flags=cv2.DrawMatchesFlags_NOT_DRAW_SINGLE_POINTS)

    if(len(loc1) > 9):
        # img_rect = cv2.circle(
        #     img1, (int(loc1[0][0][0]), int(loc1[0][0][1])), 3, (255, 0, 255), thickness=1)
        cv2.imwrite(
            "/home/ayumi/Documents/tohoku_uni/CLOVERs/images/feature_match/"+str(idx)+".jpg", img3)
        cv2.imshow("result",img3)
        cv2.waitKey(0)

for i in range(9):
    orbmatch(image_list[i],image_list[i+1],i)