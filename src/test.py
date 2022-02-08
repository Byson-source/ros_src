import numpy as np
listdayo = [[1, 2], [2, 3], [3, 5]]
listdayo_ = [[-1, -2], [2, 3], [3, 6]]
list_ = np.asarray(listdayo)
list_2 = np.asarray(listdayo_)
list_ = list_+list_2
print(list_)
# print(merged)
# print(type(list(list_)))
# print(list_.shape)
# list__ = np.array([[1, 2], [2, 1], [3, 5]])
# subtract = np.array([1, 2]*3)
# print(subtract.reshape(3, -1))
# answer=list_-list__
# for idx,val in enumerate(answer):
#     if val.max()==0:
#         print(idx)
# survived_index=[idx for idx,val in enumerate(answer) if val.max()<5]

# list_float=[2.1,3.2]
# list_int=int(list_float)

# print(list_int)
