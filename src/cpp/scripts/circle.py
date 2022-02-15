import cv2
homepath = "/home/ayumi/Documents/tohoku_uni/CLOVERs/images/all_rgb/"
img1 = cv2.imread(homepath+"37.jpg")
img2 = cv2.imread(homepath+"46.jpg")

img1 = cv2.circle(img1, (290, 269), 15,
                  (255, 0, 0), thickness=3, lineType=cv2.LINE_AA)
img2 = cv2.circle(img2, (372, 271), 15,
                  (255, 0, 0), thickness=3, lineType=cv2.LINE_AA)
cv2.imshow("result", img1)
cv2.imshow("result2", img2)
cv2.waitKey(0)
