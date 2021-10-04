import rosbag
from sensor_msgs.msg import CompressedImage
BAG_FILE="/home/ayumi/Documents/RTAB-Map/map2.bag"
OUT_BAG_FILE="/home/ayumi/Documents/RTAB-Map/out_map2.bag"
with rosbag.Bag(OUT_BAG_FILE, 'w') as outbag:
	for topic, msg, t in rosbag.Bag(BAG_FILE).read_messages():
		if topic == "data_throttled_image_depth/compressedDepth":
			msg.format = msg.format + " png"
			print("Updated compressed format! \"%s\"" % (msg.format))
		outbag.write(topic, msg, t)