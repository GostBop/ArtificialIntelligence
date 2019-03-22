# coding:utf-8

import numpy as np
import matplotlib.pyplot as plt 
import pdb
 
"旅行商问题 ( TSP , Traveling Salesman Problem )"
coordinates = np.array([[0, 0]])

def initData():
  global coordinates
  fr = open('../tc/qa194.txt')
  for line in fr.readlines():
    lineArr = line.strip().split()
    temp = np.array([[float(lineArr[1]), float(lineArr[2])]])
    coordinates = np.vstack((coordinates, temp))
  fr.close()
  coordinates = coordinates[1:]
  print(coordinates.shape[0])

def show_path(cl, cost, T):
  plt.clf()
  plt.scatter(coordinates[:, 1], coordinates[:, 0], s=5)
  for i in range(cl.shape[0] - 1):
    plt.plot([coordinates[cl[i]][1], coordinates[cl[i + 1]][1]], [coordinates[cl[i]][0], coordinates[cl[i + 1]][0]])
  plt.plot([coordinates[cl[0]][1], coordinates[cl[cl.shape[0] - 1]][1]], [coordinates[cl[0]][0], coordinates[cl[cl.shape[0] - 1]][0]])
  plt.title('T: ' + str(T) + ', cost: ' + str(cost))
  plt.draw()
  plt.pause(0.1)

#得到距离矩阵的函数
def getdistmat(coordinates):
    num = coordinates.shape[0] #194个坐标点
    distmat = np.zeros((194,194)) #194X194距离矩阵
    for i in range(num):
        for j in range(i,num):
            distmat[i][j] = distmat[j][i]=np.linalg.norm(coordinates[i]-coordinates[j])
    return distmat
 
def initpara():
    alpha = 0.99
    t = (1,200)
    markovlen = 150000
 
    return alpha,t,markovlen

initData()
num = coordinates.shape[0]
distmat = getdistmat(coordinates) #得到距离矩阵
 
 
solutionnew = np.arange(num)
#valuenew = np.max(num)
 
solutioncurrent = solutionnew.copy()
valuecurrent =99000  #np.max这样的源代码可能同样是因为版本问题被当做函数不能正确使用，应取一个较大值作为初始值
#print(valuecurrent)
 
solutionbest = solutionnew.copy()
valuebest = 99000 #np.max
 
alpha,t2,markovlen = initpara()
t = t2[1]

count = 0
result = [] #记录迭代过程中的最优解
for i in np.arange(markovlen):

    #下面的两交换和三角换是两种扰动方式，用于产生新解
    if np.random.rand() > 0.5:# 交换路径中的这2个节点的顺序
        # np.random.rand()产生[0, 1)区间的均匀随机数
        while True:#产生两个不同的随机数
            loc1 = np.int(np.ceil(np.random.rand()*(num-1)))
            loc2 = np.int(np.ceil(np.random.rand()*(num-1)))
            ## print(loc1,loc2)
            if loc1 != loc2:
                break
        solutionnew[loc1],solutionnew[loc2] = solutionnew[loc2],solutionnew[loc1]
    else: #三交换
        while True:
            loc1 = np.int(np.ceil(np.random.rand()*(num-1)))
            loc2 = np.int(np.ceil(np.random.rand()*(num-1))) 
            loc3 = np.int(np.ceil(np.random.rand()*(num-1)))

            if((loc1 != loc2)&(loc2 != loc3)&(loc1 != loc3)):
                break

        # 下面的三个判断语句使得loc1<loc2<loc3
        if loc1 > loc2:
            loc1,loc2 = loc2,loc1
        if loc2 > loc3:
            loc2,loc3 = loc3,loc2
        if loc1 > loc2:
            loc1,loc2 = loc2,loc1

        #下面的三行代码将[loc1,loc2)区间的数据插入到loc3之后
        tmplist = solutionnew[loc1:loc2].copy()
        solutionnew[loc1:loc3-loc2+1+loc1] = solutionnew[loc2:loc3+1].copy()
        solutionnew[loc3-loc2+1+loc1:loc3+1] = tmplist.copy()  

    valuenew = 0
    for i in range(num-1):
        valuenew += distmat[solutionnew[i]][solutionnew[i+1]]
    valuenew += distmat[solutionnew[0]][solutionnew[193]]
    # print (valuenew)
    if valuenew<valuecurrent: #接受该解
        
        #更新solutioncurrent 和solutionbest
        valuecurrent = valuenew
        solutioncurrent = solutionnew.copy()

        if valuenew < valuebest:
            valuebest = valuenew
            solutionbest = solutionnew.copy()
    result.append(valuebest)
    print (("%d") % (valuebest))
    count = count + 1
    #if count == 10:
        #show_path(solutionbest, valuebest, t)
        #count = 0

plt.clf()
plt.scatter(coordinates[:, 1], coordinates[:, 0], s=5)
for i in range(solutionbest.shape[0] - 1):
  plt.plot([coordinates[solutionbest[i]][1], coordinates[solutionbest[i + 1]][1]], [coordinates[solutionbest[i]][0], coordinates[solutionbest[i + 1]][0]])
plt.plot([coordinates[solutionbest[0]][1], coordinates[solutionbest[solutionbest.shape[0] - 1]][1]], [coordinates[solutionbest[0]][0], coordinates[solutionbest[solutionbest.shape[0] - 1]][0]])
plt.title('cost: ' + str(valuebest))

plt.show()

