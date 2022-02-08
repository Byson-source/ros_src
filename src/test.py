import numpy as np
list=[[1,2],[2,3],[3,5]]
list_=np.array(list)
list__=np.array([[1,2],[2,1],[3,5]])
answer=list_-list__
for idx,val in enumerate(answer):
    if val.max()==0:
        print(idx)
survived_index=[idx for idx,val in enumerate(answer) if val.max()<5]

list_float=[2.1,3.2]
list_int=int(list_float)

print(list_int)