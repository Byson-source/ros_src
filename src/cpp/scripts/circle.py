import cv2
homepath = "/home/ayumi/Documents/tohoku_uni/CLOVERs/images/all_rgb/"
img1 = cv2.imread(homepath+"198.jpg")
img2 = cv2.imread(homepath+"200.jpg")

img1 = cv2.circle(img1, (281, 158), 15,
                  (255, 0, 0), thickness=3, lineType=cv2.LINE_AA)
img2 = cv2.circle(img2, (378, 151), 15,
                  (255, 0, 0), thickness=3, lineType=cv2.LINE_AA)
cv2.imshow("result", img1)
cv2.imshow("result2", img2)
cv2.waitKey(0)
