import rosbag
from sensor_msgs.msg import CompressedImage
BAG_FILE="/home/ayumi/Documents/RTAB-Map/out2.bag"
OUT_BAG_FILE="/home/ayumi/Documents/RTAB-Map/out2_.bag"
ROBOT="robot2/"
with rosbag.Bag(OUT_BAG_FILE, 'w') as outbag:
	for topic, msg, t in rosbag.Bag(BAG_FILE).read_messages():
		if not topic == "tf":
			topic=ROBOT+topic
		outbag.write(topic, msg, t)