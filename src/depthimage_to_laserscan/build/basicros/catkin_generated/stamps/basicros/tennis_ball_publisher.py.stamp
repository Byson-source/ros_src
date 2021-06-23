import rospy
import sys
import numpy as np
from cv_bridge import CvBridge,CvBridgeError
import cv2
from sensor_msgs.msg import Image

MOVIE_PATH='/home/ayumi/catkin_ws/src/basicros/ros_practice/dataset/tennis-ball-video.mp4'
bridge=CvBridge()
def main(args):
    global bridge
    cap = cv2.VideoCapture(MOVIE_PATH)
    rospy.init_node('tennis_ball_image', anonymous=True)
    pub = rospy.Publisher('sensor_msgs/Image', Image, queue_size=10)
    rate = rospy.Rate(10)  # 10hz

    while True:
        ret,img_src=cap.read()
        if ret:
            cv2.waitKey(1)
            """Opencvがキャプチャした情報をROSように変換！"""
            try:
                ros_image = bridge.cv2_to_imgmsg(img_src, 'bgr8')
            except CvBridgeError as e:
                print(e)

            """ここまで"""

            pub.publish(ros_image)
            print("I'M publishing...")

            rate.sleep()
        else:
            break

if __name__ == '__main__':
    main(sys.argv)
