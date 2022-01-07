# TODO loopノードからloop pair情報を受け取る
# TODO feature matching+RANSAC
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
from std_msgs.msg import Int32MultiArray

import message_filters
import cv2

# if (not hasattr(rs2, 'intrinsics')):
#     import pyrealsense2.pyrealsense2 as rs2
depth_path = "/home/ayumi/Documents/tohoku_uni/CLOVERs/images/depth/"
depth_img=1
depth_img2=2
container1=[]
container2=[]
bridge=CvBridge()
loop_dict={}

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

def indexCB(data):

def valueCB(data)



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
    loop_index_sub=rospy.Subscriber("index_array",Int32MultiArray,indexCB);
    loop_value_sub=rospy.Subscriber("val_array",Int32MultiArray,valueCB);

    # main()

    rospy.spin()
