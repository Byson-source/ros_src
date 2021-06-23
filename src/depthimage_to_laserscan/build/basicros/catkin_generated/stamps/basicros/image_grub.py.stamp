import rospy
import cv2
from std_msgs.msg import String
from sensor_msgs.msg import Image
from cv_bridge import CvBridge,CvBridgeError
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

    font=cv2.FONT_HERSHEY_SIMPLEX
    cv2.putText(cv_image,'Webcam is working...',(10,350),font,1,(255,255,0),2,cv2.LINE_AA)
    cv2.imshow('Image Window',cv_image)
    cv2.waitKey(1)

def main(args):
    rospy.init_node('Image_converter',anonymous=True)
    #for tutlebot3 waffle
    #image_topic='/camera/rgb/image_row/compressed'
    #for usb_cam
    #image_topic='/usb_cam/image_raw'
    image_sub=rospy.Subscriber('/usb_cam/image_raw',Image,image_callback)
    try:
        rospy.spin()
    except KeyboardInterrupt:
        print('Shutdown')
    cv2.destroyAllWindows()

if __name__ == '__main__':
    main(sys.argv)