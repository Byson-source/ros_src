import cv2

cap=cv2.VideoCapture(0)
i=0
while True:
    i+=1
    ret,img=cap.read()
    cv2.imshow("test_view",img)
    cv2_img = cv2.resize(img, dsize=(512, 384))
    cv2.imwrite("/home/ayumi/store/"+str(i)+".jpeg",cv2_img)
    if cv2.waitKey(1)&0xff==ord('q'):
        break
print("Terminated...")