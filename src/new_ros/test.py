import matplotlib.pyplot as plt
y=[0.2]
for i in range(1000):
    if y[-1] <= 0.01:
        y.append(y[-1]-1.9799/500)
    else:
        y.append(y[i]-0.09899/500)
    if y[-1]<=0:
        break
print(len(y))
x=[i for i in range(964)]
plt.plot(x,y)
plt.show()

