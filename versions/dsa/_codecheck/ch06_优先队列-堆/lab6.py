# lab6.py —— 第一段:MinHeap(0 基)
# 运行: python lab6.py k < 文本文件
import heapq
import re
import sys


class MinHeap:
    """任务 1:手写二叉堆(0 基:根在下标 0,左子 2i+1、右子 2i+2、父 (i-1)//2)。
    元素为 (频次, 词) 元组:频次小者优先级高,平局按词序比较。"""

    def __init__(self):
        self.a = []                      # 底层就是普通 list,无需占位

    def __len__(self):
        return len(self.a)

    def _sift_up(self, i):
        """上滤:从下标 i 向上冒泡,直到父 <= 自己。"""
        while i > 0:
            p = (i - 1) // 2             # 0 基父结点公式
            if self.a[p] <= self.a[i]:
                break
            self.a[p], self.a[i] = self.a[i], self.a[p]
            i = p

    def _sift_down(self, i):
        """下滤:从下标 i 向下沉,始终与较小的孩子交换。"""
        n = len(self.a)
        while True:
            smallest = i
            for c in (2 * i + 1, 2 * i + 2):   # 左子、右子(0 基)
                if c < n and self.a[c] < self.a[smallest]:
                    smallest = c
            if smallest == i:
                break
            self.a[i], self.a[smallest] = self.a[smallest], self.a[i]
            i = smallest

    def push(self, x):
        self.a.append(x)
        self._sift_up(len(self.a) - 1)

    def pop(self):
        """删除最小元:最后一个元素搬到根再下滤(§6.3 套路)。"""
        top = self.a[0]
        last = self.a.pop()
        if self.a:
            self.a[0] = last
            self._sift_down(0)
        return top

    def top(self):
        return self.a[0]
    @classmethod
    def build_from(cls, items):
        """任务 1 补充:下滤建堆 O(n)——从最后一个非叶结点往上逐个下滤。"""
        h = cls()
        h.a = list(items)
        for i in range(len(h.a) // 2 - 1, -1, -1):
            h._sift_down(i)
        return h
def count_words(text):
    """任务 2:流式切词并统计词频,维护"词频"字典。"""
    freq = {}
    for w in re.findall(r"[a-zA-Z']+", text.lower()):
        freq[w] = freq.get(w, 0) + 1
    return freq


def topk_heap(items, k):
    """任务 3:手写小顶堆求频次最高的 k 个(小顶堆淘汰技巧)。"""
    h = MinHeap()
    for t in items:                      # t 是 (频次, 词) 元组
        if len(h) < k:
            h.push(t)                    # 堆未满:直接入堆
        elif t > h.top():                # 高于"第 k 大门槛"才替换
            h.pop()
            h.push(t)
    res = []
    while len(h) > 0:
        res.append(h.pop())
    return list(reversed(res))           # 小顶堆弹出是升序,反转为降序
def topk_heapq(items, k):
    """任务 4:标准库 heapq 重做,逻辑与手写版一字不差。"""
    h = []
    for t in items:
        if len(h) < k:
            heapq.heappush(h, t)
        elif t > h[0]:                   # h[0] 就是小顶堆的堆顶
            heapq.heapreplace(h, t)      # 等价于 heappop + heappush
    return sorted(h, reverse=True)


def main():
    # 用法: python lab6.py k < 文本文件
    if len(sys.argv) < 2:
        print("用法: python lab6.py k < 文本文件")
        return
    k = int(sys.argv[1])
    text = sys.stdin.read()
    freq = count_words(text)             # 任务 2:词频字典
    items = [(c, w) for w, c in freq.items()]   # (频次, 词)

    # 任务 5:边界处理——k <= 0 或文本为空
    if k <= 0 or not items:
        print(f"k = {k}, 词数 = {len(items)}:边界情形,答案为空。")
        return

    r1 = topk_heap(items, k)
    r2 = topk_heapq(items, k)
    print(f"词数 = {len(items)}, k = {k}")
    print("手写堆 :", [(w, c) for c, w in r1])
    print("heapq  :", [(w, c) for c, w in r2])
    print("结果一致:", r1 == r2)


if __name__ == "__main__":
    main()
