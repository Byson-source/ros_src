import matplotlib.pyplot as plt
import math
x=[i for i in range(100)]
y=[math.sin(j*2*math.pi/32) for j in x]

plt.figure()
plt.plot(x,y)
plt.show()