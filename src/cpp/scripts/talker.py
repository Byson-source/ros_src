import cv2
home = "/home/ayumi/Documents/tohoku_uni/CLOVERs/images/all_rgb/"
img = cv2.imread(home+"41"+".jpg")
img = cv2.circle(img, (294, 144), 15, (255, 0, 0),
                 thickness=3, lineType=cv2.LINE_AA)
cv2.imshow("confirm", img)


cv2.waitKey(0)
