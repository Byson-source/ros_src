import rospy
from basicros.msg import IoTSensor
def callback(info):
    print("new data coming:(%d,%s,%.2f,%2.f)" % (info.id, info.name, info.temperature, info.humidity))
def iot_sensor_callback():
    rospy.init_node('iot_sensor_subscriber',anonymous=True)
    rospy.Subscriber("iot_sensor_topic",IoTSensor,callback=callback)
    """callbackがあるのはSubscriberだけ！"""
    rospy.spin()
    """spinがあるのはSubscriberだけ！"""
if __name__ == '__main__':
    iot_sensor_callback()