import rospy
from sensor_msgs.msg import Image as msg_Image
from sensor_msgs.msg import CameraInfo
from cv_bridge import CvBridge, CvBridgeError
import sys
import os
import numpy as np
import cv2
import pyrealsense2 as rs2
if (not hasattr(rs2, 'intrinsics')):
    import pyrealsense2.pyrealsense2 as rs2


class ImageListener:
    def __init__(self, depth_image_topic, depth_info_topic):
        self.bridge = CvBridge()
        self.sub = rospy.Subscriber(
            depth_image_topic, msg_Image, self.imageDepthCallback)
        self.sub_info = rospy.Subscriber(
            depth_info_topic, CameraInfo, self.imageDepthInfoCallback)
        self.intrinsics = None
        self.pix = None
        self.pix_grade = None

    def imageDepthCallback(self, data):
        try:
            cv_image = self.bridge.imgmsg_to_cv2(data, data.encoding)
            # pick one pixel among all the pixels with the closest range:
            indices = np.array(
                np.where(cv_image == cv_image[cv_image > 0].max()))[:, 0]
            pix = (indices[1], indices[0])
            self.pix = pix
            print('\rDepth at pixel(%3d, %3d): %7.1f(mm).' % (
                pix[0], pix[1], cv_image[pix[1], pix[0]]))

            cv_image = cv2.circle(cv_image, (pix[0], pix[1]), 15,
                                  (255, 0, 0), thickness=3, lineType=cv2.LINE_AA)
            cv2.imshow("result", cv_image)
            cv2.waitKey(1)

            if self.intrinsics:
                depth = cv_image[pix[1], pix[0]]
                result = rs2.rs2_deproject_pixel_to_point(
                    self.intrinsics, [pix[0], pix[1]], depth)
                print('  Coordinate: %8.2f %8.2f %8.2f.' % (
                    result[0], result[1], result[2]))

            sys.stdout.flush()

        except CvBridgeError as e:
            print(e)
            return
        except ValueError as e:
            return

    def imageDepthInfoCallback(self, cameraInfo):
        try:
            if self.intrinsics:
                return
            self.intrinsics = rs2.intrinsics()
            self.intrinsics.width = cameraInfo.width
            self.intrinsics.height = cameraInfo.height
            self.intrinsics.ppx = cameraInfo.K[2]
            self.intrinsics.ppy = cameraInfo.K[5]
            self.intrinsics.fx = cameraInfo.K[0]
            self.intrinsics.fy = cameraInfo.K[4]
            if cameraInfo.distortion_model == 'plumb_bob':
                self.intrinsics.model = rs2.distortion.brown_conrady
            elif cameraInfo.distortion_model == 'equidistant':
                self.intrinsics.model = rs2.distortion.kannala_brandt4
            self.intrinsics.coeffs = [i for i in cameraInfo.D]
        except CvBridgeError as e:
            print(e)
            return


def main():
    depth_image_topic = 'camera/depth/image_rect_raw'
    depth_info_topic = 'camera/depth/camera_info'

    listener = ImageListener(depth_image_topic, depth_info_topic)
    rospy.spin()


if __name__ == '__main__':
    node_name = os.path.basename(sys.argv[0]).split('.')[0]
    rospy.init_node(node_name)
    main()
