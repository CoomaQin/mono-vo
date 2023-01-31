from os import listdir
import shutil
import linecache

import itertools
import numpy as np
import pykitti

# 选择10个路标点， 并将其对应的GT 和 pose 保存起来。

save_dir = "/home/ubuntu/slam/slambook2/ch11/close_data/"
save_pose = "/home/ubuntu/slam/slambook2/ch11/close_gt.txt"
data_dir = "/home/hq63/innov-train/tunnel_backward/"
pose_dir = "/media/ubuntu/zoro/ubuntu/data/kitti/odometry/poses/00.txt"
gtdir = '/media/ubuntu/zoro/ubuntu/data/kitti/odometry'
sequence = '00'

images = listdir(data_dir)
img_list = []
for line in images:
    img_name = line.split("/")[-1]
    img_list.append(img_name)

n = 10 # set 10 landmark
img_list.sort()
step = int(len(img_list) / n ) 
print(step)
landmark = []
for id in range(step, len(img_list), step):
    landmark.append(img_list[id].split('.')[0])
    src = data_dir + img_list[id]
    dst = save_dir + img_list[id]
    # shutil.copy(src, dst)
    # print(dst)

# -------------------------------------------------

dataset = pykitti.odometry(gtdir, sequence)
second_pose = dataset.poses[1]
np.set_printoptions(precision=4, suppress=True)

# -----  GT_Trajectory  ---------------------------
pose_x_list = []
pose_y_list = []
pose_z_list = []

for idx in range(len(dataset.poses)):
	pose_x_list.append(dataset.poses[idx][0][3])
	pose_y_list.append(dataset.poses[idx][1][3])
	pose_z_list.append(dataset.poses[idx][2][3])

 
f = open(save_pose, mode='w')
for line in landmark:
    print(pose_x_list[int(line)])
    print(pose_z_list[int(line)])
    f.writelines("%s %s\n"%(pose_x_list[int(line)], pose_z_list[int(line)]))

# 保存原始 pose文件
# f = open(save_pose, mode='w')
# for line in landmark:
#     cur = linecache.getline(pose_dir,int(line))
#     f.writelines(cur)
f.close()
