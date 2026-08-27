# lab12.py —— 跳表字典的实现与对比(第一段:任务1 跳表核心)
import random
import time

class Node:
    __slots__ = ("key", "next")
    def __init__(self, key, lvl):
        self.key = key
        self.next = [None] * (lvl + 1)      # 层号 0..lvl 均有效

class SkipList:                              # 任务 1:三操作 + 随机层数
    def __init__(self, max_lvl=16):
        self.head = Node(0, max_lvl)
        self.max_lvl = max_lvl
        self.level = 0
        self.rng = random.Random(12345)      # 固定种子:可复现

    def random_level(self):                  # 几何分布:每层 1/2 概率晋升
        lvl = 1
        while self.rng.getrandbits(1) and lvl < self.max_lvl:
            lvl += 1
        return lvl

    def find(self, key):
        cur = self.head
        for i in range(self.level, -1, -1):
            while cur.next[i] and cur.next[i].key < key:
                cur = cur.next[i]
        x = cur.next[0]
        return x is not None and x.key == key

    def insert(self, key):
        if self.find(key):
            return                           # 字典语义:键唯一
        update = [None] * len(self.head.next)
        cur = self.head
        for i in range(self.level, -1, -1):
            while cur.next[i] and cur.next[i].key < key:
                cur = cur.next[i]
            update[i] = cur
        lvl = self.random_level()
        if lvl > self.level:
            for i in range(self.level + 1, lvl + 1):
                update[i] = self.head
            self.level = lvl
        x = Node(key, lvl)
        for i in range(lvl + 1):
            x.next[i] = update[i].next[i]
            update[i].next[i] = x
    def erase(self, key):
        update = [None] * len(self.head.next)
        cur = self.head
        for i in range(self.level, -1, -1):
            while cur.next[i] and cur.next[i].key < key:
                cur = cur.next[i]
            update[i] = cur
        x = cur.next[0]
        if x is None or x.key != key:
            return False
        for i in range(self.level + 1):
            if update[i].next[i] != x:
                break
            update[i].next[i] = x.next[i]
        while self.level > 0 and self.head.next[self.level] is None:
            self.level -= 1                  # 回收空高层
        return True

    def keys(self):
        v = []
        p = self.head.next[0]
        while p:
            v.append(p.key)
            p = p.next[0]
        return v

    def __len__(self):
        return len(self.keys())


class SkipDict:                              # 任务 2:有序字典接口
    def __init__(self):
        self._sl = SkipList()
    def insert(self, k): self._sl.insert(k)
    def find(self, k):   return self._sl.find(k)
    def erase(self, k):  return self._sl.erase(k)
    def keys(self):      return self._sl.keys()
    def __len__(self):   return len(self._sl)
def main():
    # 任务 3:与 dict 对照,输出必须一致
    print("== 任务3:跳表字典 vs dict 一致性 ==")
    d = SkipDict()
    ref = {}
    rng = random.Random(12345)
    for i in range(20000):
        k = rng.randrange(100000)
        op = i % 4                           # 0/1 插,2 删,3 查
        if op <= 1:
            if k not in ref:
                d.insert(k)
            ref[k] = k
        elif op == 2:
            d.erase(k)
            ref.pop(k, None)
        else:
            if d.find(k) != (k in ref):
                print("不一致! key=", k)
                return 1
    dk, mk = d.keys(), sorted(ref)
    print("20000 次随机操作后:跳表 len=%d,dict len=%d" % (len(d), len(ref)))
    print("跳表前 10 键:", dk[:10])
    print("按键序与 sorted(dict) 一致:", dk == mk)

    # 任务 4:10^4 随机键计时
    N = 10000
    ks = list(range(N))
    rng.shuffle(ks)                          # 洗牌为随机序列
    s2 = SkipDict()
    t0 = time.perf_counter()
    for k in ks:
        s2.insert(k)
    sl_ins = (time.perf_counter() - t0) * 1000
    ref2 = {}
    t0 = time.perf_counter()
    for k in ks:
        ref2[k] = k
    d_ins = (time.perf_counter() - t0) * 1000
    hits = 0
    t0 = time.perf_counter()
    for k in ks:
        hits += 1 if s2.find(k) else 0
    sl_find = (time.perf_counter() - t0) * 1000
    t0 = time.perf_counter()
    for k in ks:
        hits += 1 if k in ref2 else 0
    d_find = (time.perf_counter() - t0) * 1000
    print("\n== 任务4:10^4 随机键计时 ==")
    print("插入 %d 键:跳表 %.4f ms,dict %.4f ms" % (N, sl_ins, d_ins))
    print("查找 %d 键:跳表 %.4f ms,dict %.4f ms" % (N, sl_find, d_find))
    print("(命中数 %d,防止死代码消除)" % hits)
    # 任务 5:删除全部元素,验证跳表为空
    for k in ks:
        s2.erase(k)
    print("\n== 任务5:删除全部后 ==")
    print("跳表 len=%d,为空=%s,find(0)=%s"
          % (len(s2), len(s2) == 0, s2.find(0)))


if __name__ == "__main__":
    main()
