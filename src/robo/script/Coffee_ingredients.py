import rospy
from std_msgs.msg import String

def talker():
    pub = rospy.Publisher('ingredients', String, queue_size=10)
    rospy.init_node('Coffee_ingredients', anonymous=True)
    rate = rospy.Rate(10) # 10hz←一秒あたり10回ループが回る
    # while not rospy.is_shutdown():
    sugar = input('How much sugar?>>>')
    milk=input('How much milk?>>>')
    info='Milk:'+milk+'sugar:'+sugar
    while not rospy.is_shutdown():
        print(info)
        pub.publish(info)#実行,これがdataになる
        rate.sleep()

if __name__ == '__main__':
    try:
        talker()
    except rospy.ROSInterruptException:
        pass
