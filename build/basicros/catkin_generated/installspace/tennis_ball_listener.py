import rospy
import cv2
from std_msgs.msg import String
from sensor_msgs.msg import Image
from cv_bridge import CvBridge,CvBridgeError
import numpy as np
import sys

bridge=CvBridge()

def image_callback(ros_image):
    print('got an Image')
    global bridge

    """Opencvがキャプチャした情報をROSように変換！"""
    try:
        cv_image=bridge.imgmsg_to_cv2(ros_image,'bgr8')
    except CvBridgeError as e:
        print(e)

    """ここまで"""

    cv2.imshow('view', cv_image)
    im_HSV = cv2.cvtColor(cv_image, cv2.COLOR_BGR2HSV)
    lower = np.array([0, 116, 152])
    higher = np.array([139, 255, 255])  # 雌
    img_HSV = cv2.inRange(im_HSV, lower, higher)  # 二値化が完了した状態

    cv2.imshow('HSV', img_HSV)

    Contours, _ = cv2.findContours(img_HSV, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)

    mask = np.zeros_like(img_HSV)

    if len(Contours) > 0:
        for cnt in Contours:
            area = cv2.contourArea(cnt)
            if area > 1000:
                mask = cv2.drawContours(mask, [cnt], 0, (255, 255, 255), 3)
                cv2.imshow('contours', mask)
                cv2.waitKey(1)

def main(args):
    rospy.init_node('Image_subscriber',anonymous=True)
    #for tutlebot3 waffle
    #image_topic='/camera/rgb/image_row/compressed'
    #for usb_cam
    #image_topic='/usb_cam/image_raw'
    rospy.Subscriber('sensor_msgs/Image',Image,image_callback)
    print("I'm listening...")
    try:
        rospy.spin()
    except KeyboardInterrupt:
        print('Shutdown')
    cv2.destroyAllWindows()

if __name__ == '__main__':
    main(sys.argv)