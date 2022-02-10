import numpy as np
listdayo_ = [[-1, -2], [2, 3], [3, 6], [None, None]]
print(listdayo_.index([None, None]))
a = np.asarray(listdayo_)
print(np.where(a == [-1, -2])[0][0])
