import numpy as np
import cv2
import time


lk_params = dict(winSize  = (15, 15),
                maxLevel = 2,
                criteria = (cv2.TERM_CRITERIA_EPS | cv2.TERM_CRITERIA_COUNT, 10, 0.03))

feature_params = dict(maxCorners = 20,
                    qualityLevel = 0.3,
                    minDistance = 10,
                    blockSize = 7 )
rgb_path="/home/ayumi/Documents/tohoku_uni/CLOVERs/images/all_rgb/"
feature_path="/home/ayumi/Documents/tohoku_uni/CLOVERs/images/feature_match/"

image_list=[rgb_path+"30.jpg",rgb_path+"29.jpg",rgb_path+"33.jpg",rgb_path+"30.jpg",rgb_path+"41.jpg",rgb_path+"38.jpg",rgb_path+"43.jpg",rgb_path+"40.jpg",rgb_path+"45.jpg",rgb_path+"40.jpg"]


trajectory_len = 40
trajectories = []
frame_idx = 1
T1=time.time()

while frame_idx<10:
    img_=cv2.imread(image_list[frame_idx])
    frame_gray = cv2.cvtColor(img_, cv2.COLOR_BGR2GRAY)
    img=img_.copy()

    
    mask = np.zeros_like(frame_gray)
    mask[:] = 255

    # Lastest point in latest trajectory
    for x, y in [np.int32(trajectory[-1]) for trajectory in trajectories]:
        cv2.circle(mask, (x, y), 5, 0, -1)

    # Detect the good features to track
    p = cv2.goodFeaturesToTrack(frame_gray, mask = mask, **feature_params)
    if p is not None:
        # If good features can be tracked - add that to the trajectories
        for x, y in np.float32(p).reshape(-1, 2):
            trajectories.append([(x, y)])
        # print(trajectories[0][-1])
    if frame_idx>1:

        img0, img1 = prev_gray,frame_gray
    else:
        img0,img1=frame_gray,frame_gray
    p0 = np.float32([trajectory[-1] for trajectory in trajectories]).reshape(-1, 1, 2)
    p1, _st, _err = cv2.calcOpticalFlowPyrLK(img0, img1, p0, None, **lk_params)
    p0r, _st, _err = cv2.calcOpticalFlowPyrLK(img1, img0, p1, None, **lk_params)
    d = abs(p0-p0r).reshape(-1, 2).max(-1)
    good = d < 10
    # NOTE これで同一の点かどうかを区別する

    new_trajectories = []

    # Get all the trajectories
    iteration=0
    for trajectory, (x, y), good_flag in zip(trajectories, p1.reshape(-1, 2), good):
        if not good_flag:
            continue
        trajectory.append((x, y))
        iteration+=1
        if len(trajectory) > trajectory_len:
            del trajectory[0]
        new_trajectories.append(trajectory)
        # Newest detected point
        cv2.circle(img, (int(x), int(y)), 2, (0, 0, 255), -1)
    print(iteration)

    trajectories = new_trajectories

    prev_gray=frame_gray
    cv2.imwrite(feature_path+str(frame_idx)+".jpg",img)
    frame_idx+=1

    # Draw all the trajectories
T2=time.time()
print(T2-T1)


# Update interval - When to update and detect new features
    



