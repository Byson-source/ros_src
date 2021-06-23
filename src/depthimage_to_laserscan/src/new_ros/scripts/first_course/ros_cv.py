import rospy
import cv2
from std_msgs.msg import string
from sensor_msgs.msg import Image
from cv_bridge import CvBridge,CvBridgeError
import sys

bridge=CvBridge()

def image_callback(ros_image):
    print('got an Image')
    global bridge

    try:
        cv_image=bridge.imgmsg_to_cv2(ros_image,'bgr8')
    except CvBridgeError as e:
        print(e)

    (rows,cols,channels)=cv_image.shape
    cv2.imshow('test',cv_image)
    cv2.waitKey(1)

if __name__ == '__main__':
    rospy.init_node('img')
    rospy.Subscriber('usb_cam/image_raw','sensor_msgs/Image',image_callback)
