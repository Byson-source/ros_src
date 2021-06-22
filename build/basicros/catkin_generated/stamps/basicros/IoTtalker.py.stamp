import rospy
from basicros.msg import IoTSensor
import random

pub=rospy.Publisher('iot_sensor_topic',IoTSensor,queue_size=10)
rospy.init_node('iot_sensor_publish_node',anonymous=True)
"""一秒間に何個のメッセージをPublishするか？"""
rate=rospy.Rate(1)
i=0

while not rospy.is_shutdown():
    iot_sensor=IoTSensor()
    iot_sensor.id=1
    iot_sensor.name='iot_01'
    iot_sensor.temperature=20
    iot_sensor.humidity=33.41+random.random()
    print('I publish:')
    pub.loginfo(iot_sensor)
    pub.publish(iot_sensor)
    rate.sleep()
    """↑は絶対に必要！"""
    i+=1