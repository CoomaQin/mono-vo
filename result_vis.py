"""Example of pykitti.odometry usage."""
import itertools
import matplotlib.pyplot as plt
import numpy as np
from mpl_toolkits.mplot3d import Axes3D
from mpl_toolkits import mplot3d
import pykitti


p_f = open("./build/results.txt")
pc_f = open("./build/results_correct.txt")
gtdir = '/media/ubuntu/zoro/ubuntu/data/kitti/odometry'
sequence = '00'
dataset = pykitti.odometry(gtdir, sequence)

second_pose = dataset.poses[1]
np.set_printoptions(precision=4, suppress=True)
print('\nSequence: ' + str(dataset.sequence))
print('\nFrame range: ' + str(dataset.frames))

if second_pose[2][3] == dataset.poses[2][2][3]:
	print('second_pose[0][3] == dataset.poses[1][0][3] is equal\n')

# -----  GT_Trajectory  ---------------------------
pose_x_list = []
pose_y_list = []
pose_z_list = []

for idx in range(len(dataset.poses)):
	pose_x_list.append(dataset.poses[idx][0][3])
	pose_y_list.append(dataset.poses[idx][1][3])
	pose_z_list.append(dataset.poses[idx][2][3])

# 保存gt pos 到txt
# gt_f = open("./build/gt_pos.txt",'w')
# for i in range(len(pose_x_list)):
# 	gt_f.writelines(' '.join([str(pose_x_list[i]), 
# 								str(pose_y_list[i]), 
# 								str(pose_z_list[i])] ))
# 	gt_f.writelines("\n")

# -----  prediction_Trajectory  ---------------------------
pre_x = []
pre_y = []
for line in p_f:
	pos = line.strip().split()
	pre_x.append(float(pos[0]))
	pre_y.append(float(pos[2]))

pre_x2 = []
pre_y2 = []
for line in pc_f:
	pos = line.strip().split()
	pre_x2.append(float(pos[0]))
	pre_y2.append(float(pos[2]))

# ------ vis ---------------------
fig = plt.figure()
plt.plot(pose_x_list, pose_z_list,'g')
plt.plot(pre_x, pre_y,'r')
plt.plot(pre_x2, pre_y2,'b')
plt.legend(['Ground truth','mono odometry','mono odometry-correct'])

# 起点
plt.plot(pose_x_list[1],pose_z_list[1],'go',30)
plt.plot(pre_x[1],pre_y[1],'bo',30)
plt.savefig("./compare.png")
plt.show()


