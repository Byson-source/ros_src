import os
# NOTE 最大ファイル数は659
path="/home/ayumi/Documents/tohoku_uni/CLOVERs/images/all_rgb/"

# for i in range(1,34):
#     for j in range(1,21):

#         if j%2==1:
#             os.rename(path+str(20*(i-1)+j)+".jpg",path+str(2000+20*(i-1)+j)+".jpg")
#         else:
#             if(20*(i-1)+j<660):
#                 os.rename(path+str(20*(i-1)+j)+".jpg",path+str(1000+20*(i-1)+j)+".jpg")

# for i in range(1,34):
#     for j in range(1,21):
#         if j%2==0:
#             if(20*(i-1)+j<660):
#                 os.rename(path+str(1000+20*(i-1)+j)+".jpg",path+str(int((20*(i-1)+j)/2+10*(i-1)))+".jpg")


                # os.rename(path+str(10*(i-1)+j)+".jpg",path+str(int((10*(i-1)+j)/2+5))+".jpg")
                # print(int((10*(i-1)+j)/2))
                # 残り330個
                # 2017から2659
k=1
for j in range(1,331):
    os.rename(path+str(2001+2*(j-1))+".jpg",path+str(10+j+10*(k-1))+".jpg")
    if j%10==0:
        k+=1
