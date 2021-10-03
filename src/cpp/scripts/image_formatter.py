import os
IMAGE_PATH = "/home/ayumi/Documents/tohoku_uni/CLOVERs/2-image/depth/"
i = 11
j=0
while(i <= 390):
    if not (os.path.exists(IMAGE_PATH+str(i)+".png")):
        i+=1
        j+=1
    else:
        k=i
        i-=j
        os.rename(IMAGE_PATH+str(k)+".png",IMAGE_PATH+str(i)+".png")
        i+=1
        j=0

