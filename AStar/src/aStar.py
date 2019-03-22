# -*- coding: UTF-8 -*-
import copy
import re
import datetime
import matplotlib.pyplot as plt
from pylab import mpl
mpl.rcParams['font.sans-serif'] = ['FangSong']  # 指定默认字体

BLOCK = []  # 给定状态
GOAL = []  # 目标状态

# 4个方向
direction = [[0, -1], [1, 0], [0, 1], [-1, 0]]

# OPEN表
OPEN = {}

# CLOSED表
CLOSED = {}

# OPEN表的节点数
OPEN_NUMBER = []

# CLOSED表的点的评估函数值
CLOSED_ESTIMATION = []

# 节点的总数
SUM_NODE_NUM = []

# 单调性
DIFFERENCE = []


# 状态节点
class State(object):
    def __init__(self, gn=0, hn=0, state=None, hash_value=None, par=None):
        '''
        初始化
        :param gn: gn是初始化到现在的距离
        :param hn: 启发距离
        :param state: 节点存储的状态
        :param hash_value: 哈希值，用于判重
        :param par: 父节点指针
        '''
        self.gn = gn
        self.hn = hn
        self.fn = self.gn + self.hn
        self.child = []  # 孩子节点
        self.par = par  # 父节点
        self.state = state  # 局面状态
        self.hash_value = hash_value  # 哈希值

    def __lt__(self, other):  # 用于堆的比较，返回距离最小的
        return self.fn < other.fn

    def __eq__(self, other):  # 相等的判断
        return self.hash_value == other.hash_value

    def __ne__(self, other):  # 不等的判断
        return not self.__eq__(other)


# 判断是否可解
def solvable():
    src = []
    dst = []
    src_num = 0
    dst_num = 0
    num = 0
    for i in range(len(BLOCK)):
        for j in range(len(BLOCK)):
            if BLOCK[i][j] != 0:
                src.append(BLOCK[i][j])
                num += 1
            if GOAL[i][j] != 0:
                dst.append(GOAL[i][j])
    for i in range(num):
        for j in range(i, num):
            if src[i] > src[j]:
                src_num += 1
            if dst[i] > dst[j]:
                dst_num += 1
    print src_num
    print dst_num
    return src_num % 2 == dst_num % 2


def f1(cur_state, end_state):
    '''
    :param cur_state: 当前状态
    :param end_state: 结束状态
    :return:
    '''
    dist = 0
    N = len(cur_state)
    for i in range(N):
        for j in range(N):
            if cur_state[i][j] == end_state[i][j]:
                continue
            if cur_state[i][j] == 0:
                continue
            dist += 1
    return dist


def manhattan_dis(cur_state, end_state):
    '''
    计算曼哈顿距离
    :param cur_state: 当前状态
    :param end_state: 结束状态
    :return: 到目的状态的曼哈顿距离
    '''
    dist = 0
    N = len(cur_state)
    for i in range(N):
        for j in range(N):
            if cur_state[i][j] == end_state[i][j]:
                continue
            num = cur_state[i][j]
            if num != 0:
                x = (num - 1) / N  # 理论纵坐标
                y = num - N * x - 1  # 理论的横坐标
                dist += (abs(x - i) + abs(y - j))
    return dist


def update_child(hash_value):
    '''
    :param hash_value: 要更新的节点的hash值
    :return:
    '''
    if hash_value in CLOSED:
        if len(CLOSED[hash_value].child) != 0:
            for h in CLOSED[hash_value].child:
                if h in CLOSED:
                    CLOSED[h].gn = CLOSED[hash_value].gn + 1
                    CLOSED[h].fn = CLOSED[h].gn + CLOSED[h].hn
                    update_child(h)
                else:
                    OPEN[h].gn = CLOSED[hash_value].gn + 1
                    OPEN[h].fn = OPEN[h].gn + OPEN[h].hn
                    update_child(h)
    elif hash_value in OPEN:
        return


def generate_child(cur_node, end_node, hash_set, dis_fn):
    '''
    生成子节点函数
    :param cur_node:  当前节点
    :param end_node:  最终状态节点
    :param hash_set:  哈希表，用于判重
    :param dis_fn: 距离函数
    :return: None
    '''
    num = len(cur_node.state)
    CLOSED_ESTIMATION.append(cur_node.fn)
    CLOSED[cur_node.hash_value] = OPEN[cur_node.hash_value]
    del OPEN[cur_node.hash_value]
    for i in range(0, num):
        for j in range(0, num):
            if cur_node.state[i][j] != 0:
                continue
            for d in direction:  # 四个偏移方向
                x = i + d[0]
                y = j + d[1]
                if x < 0 or x >= num or y < 0 or y >= num:  # 越界了
                    continue
                state = copy.deepcopy(cur_node.state)  # 复制父节点的状态
                state[i][j], state[x][y] = state[x][y], state[i][j]  # 交换位置
                h = hash(str(state))  # 哈希时要先转换成字符串
                if h in hash_set:  # 重复了
                    if h in OPEN:
                        if OPEN[h].gn > cur_node.gn + 1:
                            OPEN[h].par = cur_node.hash_value
                            OPEN[h].gn = cur_node.gn + 1
                            OPEN[h].fn = OPEN[h].gn + OPEN[h].hn
                    elif h in CLOSED:
                        if CLOSED[h].gn > cur_node.gn:
                            CLOSED[h].par = cur_node.hash_value
                            CLOSED[h].gn = cur_node.gn + 1
                            CLOSED[h].fn = CLOSED[h].gn + CLOSED[h].hn
                            update_child(h)

                else:
                    hash_set.add(h)  # 加入哈希表
                    gn = cur_node.gn + 1  # 已经走的距离函数
                    hn = dis_fn(state, end_node.state)  # 启发的距离函数
                    node = State(gn, hn, state, h, cur_node.hash_value)  # 新建节点
                    cur_node.child.append(h)  # 加入到孩子队列
                    DIFFERENCE.append(node.fn + 1 - cur_node.fn)
                    OPEN[h] = node  # 加入到堆中


def show_block(block):
    print("---------------")
    for b in block:
        print(b)


def print_path(node):
    '''
    输出路径
    :param node: 最终的节点
    :return: None
    '''
    num = node.fn
    stack = []  # 模拟栈
    while node.par is not None:
        stack.append(node)
        node = CLOSED[node.par]
    stack.append(node)
    print '输出OPEN表中在最佳路径的节点'
    while len(stack) != 0:
        t = stack.pop()
        show_block(t.state)
        print '节点深度gn为: %d' % t.gn
        print '评估函数的值为：%d' % t.fn
    return num


def A_start(start, end, distance_fn, generate_child_fn):
    '''
    A*算法
    :param start: 起始状态
    :param end: 终止状态
    :param distance_fn: 距离函数，可以使用自定义的
    :param generate_child_fn: 产生孩子节点的函数
    :return: None
    '''
    root = State(0, 0, start, hash(str(BLOCK)), None)  # 根节点
    end_state = State(0, 0, end, hash(str(GOAL)), None)  # 最后的节点
    root.hn = manhattan_dis(root.state, end_state.state)
    root.fn = manhattan_dis(root.state, end_state.state)
    if root == end_state:
        print("start == end !")

    OPEN[root.hash_value] = root

    node_hash_set = set()  # 存储节点的哈希值
    node_hash_set.add(root.hash_value)
    x = 0
    while len(OPEN) != 0:
        x += 1
        OPEN_NUMBER.append(len(OPEN))
        SUM_NODE_NUM.append(len(OPEN) + len(CLOSED))
        top = OPEN[min(OPEN, key=OPEN.get)]
        print '评估函数值最小的节点, 评估函数的值为：%d' % top.fn
        show_block(top.state)
        if top == end_state:  # 结束后直接输出路径
            plt.figure(1)
            plt.plot(range(x), OPEN_NUMBER)
            plt.xlabel(u"次数")
            plt.ylabel(u"OPEN表的节点数")
            plt.show()
            plt.figure(2)
            plt.plot(range(x), SUM_NODE_NUM)
            plt.xlabel(u'次数')
            plt.ylabel(u'节点的总数')
            plt.show()
            return print_path(top)
        # 产生孩子节点，孩子节点加入OPEN表
        generate_child_fn(cur_node=top, end_node=end_state, hash_set=node_hash_set,
                          dis_fn=distance_fn)
    print("No road !")  # 没有路径
    return -1


def read_block(block, line, N):
    '''
    读取一行数据作为原始状态
    :param block: 原始状态
    :param line: 一行数据
    :param N: 数据的总数
    :return: None
    '''
    pattern = re.compile(r'\d+')  # 正则表达式提取数据
    res = re.findall(pattern, line)
    t = 0
    tmp = []
    for i in res:
        t += 1
        tmp.append(int(i))
        if t == N:
            t = 0
            block.append(tmp)
            tmp = []


if __name__ == '__main__':
    try:
        file = open("./infile.txt", "r")
    except IOError:
        print("can not open file infile.txt !")
        exit(1)

    f = open("../tc/infile.txt")
    NUMBER = int(f.readline()[-2])
    n = 1
    for i in range(NUMBER):
        l = []
        for j in range(NUMBER):
            l.append(n)
            n += 1
        GOAL.append(l)
    GOAL[NUMBER - 1][NUMBER - 1] = 0

    for line in f:  # 读取每一行数据
        OPEN.clear()
        CLOSED.clear()
        BLOCK = []
        read_block(BLOCK, line, NUMBER)
        start_t = datetime.datetime.now()
        # length = A_start(BLOCK, GOAL, manhattan_dis, generate_child)
        if solvable():
            length = A_start(BLOCK, GOAL, f1, generate_child)
            end_t = datetime.datetime.now()
            if length != -1:
                print('length = ', length)
                print('time = ', (end_t - start_t).total_seconds(), "s")
                print('Nodes = ', SUM_NODE_NUM[len(SUM_NODE_NUM) - 1])
                plt.figure(3)
                plt.plot(range(len(CLOSED_ESTIMATION)), CLOSED_ESTIMATION, 'b-', label=u'挑选出来求后继的节点的f(n)')
                plt.plot(range(len(CLOSED_ESTIMATION)),
                         [length for _ in range(len(CLOSED_ESTIMATION))], 'r-', label=u'f*(S0)')
                plt.legend()
                plt.show()
                plt.figure(4)
                plt.scatter(range(len(DIFFERENCE)), DIFFERENCE, c='r')
                plt.xlabel(u'拓展的节点号')
                plt.xlabel(u'扩展的子节点j 的h(j) + 1 与该点n 的h(n)的差值')
                plt.show()



