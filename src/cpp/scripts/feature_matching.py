# TODO 特徴点に該当する深度算出

# TODO 特徴点の座標と深度をPublish

import rospy
from sensor_msgs.msg import Image as msg_Image
from sensor_msgs.msg import CameraInfo
from cv_bridge import CvBridge, CvBridgeError
import sys
import os
import numpy as np
import pyrealsense2 as rs2
import os
from rtabmap_ros.msg import Info
from std_msgs.msg import Int32
from cpp.msg import MultiArray
from cpp.msg import FeatureArray
# NOTE 例えばr12の場合、r1がloopを検知した画像ペアのうち、r2が撮った写真上の特徴点の情報を格納している
# NOTE [[x1,y1,depth1],[x2,y2,depth2]...]

import message_filters
import cv2

# if (not hasattr(rs2, 'intrinsics')):
#     import pyrealsense2.pyrealsense2 as rs2
depth_path = "/home/ayumi/Documents/tohoku_uni/CLOVERs/images/depth/"
rgb_path="/home/ayumi/Documents/tohoku_uni/CLOVERs/images/all_rgb/"
depth_img=1
depth_img2=2
container1=[]
container2=[]
bridge=CvBridge()

def depthCB1(depth1, id):
    global depth_img

    cv2_img = bridge.imgmsg_to_cv2(depth1, "passthrough")
    # FIXME Maybe should change to (640,480)
    cv2_img = cv2.resize(cv2_img, dsize=(512, 384))

    cv2_img = cv2.applyColorMap(cv2.convertScaleAbs(cv2_img, alpha=0.03), cv2.COLORMAP_JET)

    # REVIEW 再開
    container1.append(cv2_img)
    cv2.imwrite(depth_path + str(depth_img) + ".png", cv2_img)

    depth_img += 2


def depthCB2(depth2, id):
    global depth_img2

    img = bridge.imgmsg_to_cv2(depth2, "passthrough")
    # FIXME Maybe should change to (640,480)
    img = cv2.resize(img, dsize=(512, 384))
    
    img = cv2.applyColorMap(cv2.convertScaleAbs(img, alpha=0.03), cv2.COLORMAP_JET)
    cv2.imwrite(depth_path + str(depth_img2) + ".png", img)
    container2.append(img)

    depth_img2 += 2

# Return feature locations
# TODO パラメーターの妥当性の確認
def orbmatch(fileName1, fileName2):
    img1 = cv2.imread(rgb_path+str(fileName1)+".jpg")
    img2 = cv2.imread(rgb_path+str(fileName2)+".jpg")
    if img1 is None:
        print('img1 open err exit')
        exit(1)
    if img2 is None:
        print('img2 open err exit')
        exit(1)

    # ORB検出器生成
    orb = cv2.ORB_create(1000)

    # ORB特徴の取得
    kp1, des1 = orb.detectAndCompute(img1,None)
    kp2, des2 = orb.detectAndCompute(img2,None)
    
    kp1_loc,kp2_loc=[],[]

    FLANN_INDEX_LSH = 6
    index_params= dict(algorithm = FLANN_INDEX_LSH,
                       table_number = 6, # 12
                       key_size = 14,     # 20
                       multi_probe_level = 1) #2
    search_params = dict(checks=50)

    flann = cv2.FlannBasedMatcher(index_params,search_params)

    # マッチング
    knn_matches = flann.knnMatch(des1,des2,k=2)

    # マッチング結果を描画
    ratio_thresh = 0.7
    good_matches = []
    for mt in knn_matches:
        
        if len(mt) < 2:
            continue
        m,n = mt[0], mt[1]
        
        # print(type(x1))
        if m.distance < ratio_thresh * n.distance:
            good_matches.append(m)
            img1_idx=m.queryIdx
            img2_idx=m.trainIdx
            
            (x1,y1)=kp1[img1_idx].pt
            (x2,y2)=kp2[img2_idx].pt
            
            kp1_loc.append((x1,y1))
            kp2_loc.append((x2,y2))
            
    src_pts = np.float32([ kp1[m.queryIdx].pt for m in good_matches ]).reshape(-1,1,2)
    dst_pts = np.float32([ kp2[m.trainIdx].pt for m in good_matches ]).reshape(-1,1,2)

    M, mask = cv2.findHomography(src_pts, dst_pts, cv2.RANSAC,1)
    mask = mask.ravel().tolist()
    loc1,loc2=[],[]
    
    for index,element in enumerate(mask):
        if element==1:
            loc1.append(kp1_loc[index])
            loc2.append(kp2_loc[index])   
            
    # loc1とloc2がRANSACでふるい分けられた特徴点の座標

    return loc1,loc2

def loop_CB(data):

    loop_dict={"R1":{"index":[],"value":[],"FeaturePair":{}},
               "R2":{"index":[],"value":[],"FeaturePair":{}}}

    for val in data.r1_index:
        loop_dict["R1"]["index"].append(val)
    for val in data.r1_value:
        loop_dict["R1"]["value"].append(val)
    for val in data.r2_index:
        loop_dict["R2"]["index"].append(val)
    for val in data.r2_index:
        loop_dict["R2"]["value"].append(val)

    for index,element in enumerate(loop_dict):
        if index=="R1":
            for iter in range(len(element["index"])):
                element["FeaturePair"]["R1"],element["FeaturePair"]["R2"]=orbmatch(element["index"][iter],
                                                                                    element["value"][iter])
        else:
            for iter in range(len(element["index"])):
                element["FeaturePair"]["R2"],element["FeaturePair"]["R1"]=orbmatch(element["index"][iter],
                                                                                    element["value"][iter])

    info=FeatureArray()
    info.r11=loop_dict["R1"]["FeaturePair"]["R1"]
    info.r12=loop_dict["R1"]["FeaturePair"]["R2"]
    info.r21=loop_dict["R2"]["FeaturePair"]["R1"]
    info.r22=loop_dict["R2"]["FeaturePair"]["R2"]

    # depth算出

# class ImageListener:
#     def __init__(self, depth_image_topic, depth_info_topic):
#         self.bridge = CvBridge()
#         self.sub = rospy.Subscriber(depth_image_topic, msg_Image, self.imageDepthCallback)
#         self.sub_info = rospy.Subscriber(depth_info_topic, CameraInfo, self.imageDepthInfoCallback)
#         confidence_topic = depth_image_topic.replace('depth', 'confidence')
#         self.sub_conf = rospy.Subscriber(confidence_topic, msg_Image, self.confidenceCallback)
#         self.intrinsics = None
#         self.pix = None
#         self.pix_grade = None

#     def imageDepthCallback(self, data):
#         try:
#             cv_image = self.bridge.imgmsg_to_cv2(data, data.encoding)
#             # pick one pixel among all the pixels with the closest range:
#             indices = np.array(np.where(cv_image == cv_image[cv_image > 0].min()))[:,0]
#             pix = (indices[1], indices[0])
#             self.pix = pix
#             line = '\rDepth at pixel(%3d, %3d): %7.1f(mm).' % (pix[0], pix[1], cv_image[pix[1], pix[0]])

#             if self.intrinsics:
#                 depth = cv_image[pix[1], pix[0]]
#                 result = rs2.rs2_deproject_pixel_to_point(self.intrinsics, [pix[0], pix[1]], depth)
#                 line += '  Coordinate: %8.2f %8.2f %8.2f.' % (result[0], result[1], result[2])
#             if (not self.pix_grade is None):
#                 line += ' Grade: %2d' % self.pix_grade
#             line += '\r'
#             sys.stdout.write(line)
#             sys.stdout.flush()

#         except CvBridgeError as e:
#             print(e)
#             return
#         except ValueError as e:
#             return

#     def confidenceCallback(self, data):
#         try:
#             cv_image = self.bridge.imgmsg_to_cv2(data, data.encoding)
#             grades = np.bitwise_and(cv_image >> 4, 0x0f)
#             if (self.pix):
#                 self.pix_grade = grades[self.pix[1], self.pix[0]]
#         except CvBridgeError as e:
#             print(e)
#             return


#     def imageDepthInfoCallback(self, cameraInfo):
#         try:
#             if self.intrinsics:
#                 return
#             self.intrinsics = rs2.intrinsics()
#             self.intrinsics.width = cameraInfo.width
#             self.intrinsics.height = cameraInfo.height
#             self.intrinsics.ppx = cameraInfo.K[2]
#             self.intrinsics.ppy = cameraInfo.K[5]
#             self.intrinsics.fx = cameraInfo.K[0]
#             self.intrinsics.fy = cameraInfo.K[4]
#             if cameraInfo.distortion_model == 'plumb_bob':
#                 self.intrinsics.model = rs2.distortion.brown_conrady
#             elif cameraInfo.distortion_model == 'equidistant':
#                 self.intrinsics.model = rs2.distortion.kannala_brandt4
#             self.intrinsics.coeffs = [i for i in cameraInfo.D]
#         except CvBridgeError as e:
#             print(e)
#             return
# def main():
#     depth_image_topic = '/camera/depth/image_rect_raw'
#     depth_info_topic = '/camera/depth/camera_info'
#     listener = ImageListener(depth_image_topic, depth_info_topic)
#     rospy.spin()
if __name__ == '__main__':
    # node_name = os.path.basename(sys.argv[0]).split('.')[0]
    rospy.init_node("depth_listener")
    # /////////////////////////////////////////////////////////////////////////
    depth_topic = "/robot1/camera/depth/image_raw"
    depth_topic2 = "/robot2/camera/depth/image_raw"

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
    # /////////////////////////////////////////////////////////////////////////

    # /////////////////////////////////////////////////////////////////////////
    loop_sub=rospy.Subscriber("result",MultiArray,loopCB);
    feature_pub=rospy.Publisher("features",FeatureArray,queue_size=10)

    # main()

    rospy.spin()
