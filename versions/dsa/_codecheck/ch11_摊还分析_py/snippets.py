# -*- coding: utf-8 -*-
# 验证 ch11 Python 版 HTML 中的三个示例片段

print("===== 示例 11-1:栈操作序列总代价 =====")
def multiPop(st, k):
    cost = 0
    while st and cost < k:
        st.pop()
        cost += 1
    return cost

n = 10000
st, total = [], 0
for i in range(n):
    st.append(i)                    # push:代价 1
    total += 1
    if i % 1000 == 999:             # 每 1000 次 push 来一个大 multiPop
        total += multiPop(st, n)    # 弹空:每个元素只出栈一次
print(f"操作数 n = {n}, 总代价 = {total} (≤ 2n = {2*n}), 摊还 = {total/n}")

print("===== 示例 11-3:buildHeap 总下滤步数 =====")
def heapify(a):
    n, steps = len(a), 0
    def down(i):                    # 下滤一个结点
        nonlocal steps
        while True:
            l, r, m = 2 * i + 1, 2 * i + 2, i
            if l < n and a[l] > a[m]: m = l
            if r < n and a[r] > a[m]: m = r
            if m == i: break
            a[i], a[m] = a[m], a[i]
            steps += 1              # 交换一层记 1 步
            i = m
    for i in range(n // 2 - 1, -1, -1):
        down(i)
    return steps

n = 65535
a = list(range(n, 0, -1))
s = heapify(a)
print(f"n = {n}, 总下滤步数 = {s} (理论 O(n),实测 ≈ {s * 100 // n}% n)")

print("===== 示例 11-4:手写倍增 vs 内置 list =====")
import sys

class MyList:                       # 手写倍增动态数组
    def __init__(self):
        self._buf = [None] * 1
        self._size = 0
        self._cap = 1
    def push_back(self, x):
        if self._size == self._cap:
            new_buf = [None] * (self._cap * 2)   # 倍增
            for i in range(self._size):
                new_buf[i] = self._buf[i]
            self._buf = new_buf
            self._cap *= 2
            print(f"{self._size}→{self._cap}", end="  ")  # 打印拐点
        self._buf[self._size] = x
        self._size += 1

print("手写倍增:", end=" ")
v = MyList()
for i in range(200):
    v.push_back(i)
print("\n内置 list:", end=" ")
a, last = [], sys.getsizeof([])
for i in range(1, 201):
    a.append(i)
    cur = sys.getsizeof(a)
    if cur != last:                 # 内存变化 = 扩容拐点
        print(f"{len(a)}→{cur}B", end="  ")
        last = cur
print()
