import numpy as np
listdayo_ = [[-1, -2], [2, 3], [3, 6]]
a = np.asarray(listdayo_)
print(np.where(a == [-1, -2])[0][0])
