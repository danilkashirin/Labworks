import matplotlib.pyplot as plt
from matplotlib.ticker import FixedLocator
import numpy as np


file = open("result.txt", "r")
title = file.readline()[:-1]

custom = [int(i) for i in file.readline().split()]
std = [int(i) for i in file.readline().split()]

builds = [i * 1000000 for i in range(len(custom))]

file.close()


font = {'weight': 'bold',
        'size': 32}

builds = np.array(builds)
y_stack = np.row_stack((custom, std)) 

fig = plt.figure(figsize=(19, 10))
ax1 = fig.add_subplot(111)

ax1.plot(builds, y_stack[0,:], label='Собственный аллокатор', color='r')
ax1.plot(builds, y_stack[1,:], label='Стандартный аллокатор', color='g')

plt.title(title, font)

font = {'size': 20}
plt.xlabel("Количество добавленных объектов", font)
plt.ylabel("Время работы (миллисекунды)", font)


handles, labels = ax1.get_legend_handles_labels()
lgd = ax1.legend(handles, labels, loc='upper left')


title = title.replace(" ", "_").lower()
plt.savefig(title + '.png')
