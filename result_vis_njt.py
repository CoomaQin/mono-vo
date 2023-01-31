import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

gt = pd.read_csv("./data/gps_101_1671674446_1671674503.csv")
gt_x = gt["lon"].to_list()
gt_y = gt["lat"].to_list()

scale_x = 0.0000005
scale_y = -0.000003
rotate_theta = 0
offset_x = gt_x[0]
offset_y = gt_y[1]
p_f = open("./data/results_101_1671674446_1671674503.txt")
pre_x = []
pre_y = []
for line in p_f:
	pos = line.strip().split()
	# x = float(pos[0])
    # y = float(pos[2])
	pre_x.append((float(pos[0]) * np.cos(rotate_theta) - float(pos[2]) * np.sin(rotate_theta)) * scale_x + offset_x)
	pre_y.append((float(pos[0]) * np.sin(rotate_theta) + float(pos[2]) * np.cos(rotate_theta)) * scale_y + offset_y)
# pre_x = [offset_x] + pre_x
# pre_y = [offset_y] + pre_y

fig = plt.figure()
plt.plot(pre_x, pre_y,'r-o',label="pred")
plt.plot(gt_x, gt_y,'b-o',label="gt")
plt.legend()
# plt.plot(pre_x[1],pre_y[1],'r*', label="pred start")
# plt.plot(gt_x[1],gt_y[1],'b*', label="gt start")
plt.savefig("./compare_101_1671674446_1671674503.png")