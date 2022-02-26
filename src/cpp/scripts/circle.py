import cv2
homepath = "/home/ayumi/Documents/tohoku_uni/CLOVERs/images/all_rgb/"
img1 = cv2.imread(homepath+"184.jpg")
img2 = cv2.imread(homepath+"121.jpg")
img3 = cv2.imread(homepath+"186.jpg")
img4 = cv2.imread(homepath+"123.jpg")
img5 = cv2.imread(homepath+"190.jpg")
img6 = cv2.imread(homepath+"125.jpg")

img1 = cv2.circle(img1, (563, 94), 15,
                  (255, 0, 0), thickness=3, lineType=cv2.LINE_AA)
img2 = cv2.circle(img2, (420, 100), 15,
                  (255, 0, 0), thickness=3, lineType=cv2.LINE_AA)
img3 = cv2.circle(img3, (581, 89), 15,
                  (255, 0, 0), thickness=3, lineType=cv2.LINE_AA)
img4 = cv2.circle(img4, (435, 82), 15,
                  (255, 0, 0), thickness=3, lineType=cv2.LINE_AA)
img5 = cv2.circle(img5, (578, 73), 15,
                  (255, 0, 0), thickness=3, lineType=cv2.LINE_AA)
img6 = cv2.circle(img6, (373, 53), 15,
                  (255, 0, 0), thickness=3, lineType=cv2.LINE_AA)
cv2.imshow("result", img1)
cv2.imshow("result2", img2)
cv2.imshow("result3", img3)
cv2.imshow("result4", img4)
cv2.imshow("result5", img5)
cv2.imshow("result6", img6)
cv2.waitKey(0)
