# lab5.py —— 散列表单词统计对比实验(第一段:工具与手写散列表)
import re
import sys
import time
from collections import defaultdict


def next_prime(n):
    """返回大于等于 n 的最小素数。"""
    def is_prime(x):
        if x < 2:
            return False
        d = 2
        while d * d <= x:
            if x % d == 0:
                return False
            d += 1
        return True
    while not is_prime(n):
        n += 1
    return n


def split_words(text):
    """任务 5:切分单词——字母/数字连续成词(正则),其余字符跳过。"""
    return re.findall(r"[a-z0-9]+", text.lower())


class HashTable:
    """任务 1:分离链接散列表(字符串键,37 倍散列,素数表大小)。"""

    def __init__(self, m):
        self.buckets = [[] for _ in range(next_prime(m))]  # list of list
        self.size = 0

    def _hash(self, s):
        h = 0
        for c in s:
            h = h * 37 + ord(c)   # 全程不取模,最后再 % 表大小(见 §5.1.3)
        return h % len(self.buckets)

    def add(self, s):
        b = self._hash(s)
        for i, (key, cnt) in enumerate(self.buckets[b]):
            if key == s:
                self.buckets[b][i] = (key, cnt + 1)
                return
        self.buckets[b].append((s, 1))
        self.size += 1

    def count(self, s):
        for key, cnt in self.buckets[self._hash(s)]:
            if key == s:
                return cnt
        return 0

    @property
    def load_factor(self):
        return self.size / len(self.buckets)


def main():
    if len(sys.argv) < 2:
        print("用法: python lab5.py <文本文件>")
        return
    try:
        with open(sys.argv[1], encoding="utf-8") as f:
            text = f.read()
    except OSError as e:
        print("错误: 无法打开文件:", e)      # 任务 5:文件打不开
        return
    words = split_words(text)
    if not words:                             # 任务 5:空文本
        print("警告: 文本为空或没有合法单词,无法统计")
        return
    print(f"单词总数(含重复): {len(words)}")

    ht = HashTable(256)                       # 任务 2:手写表统计
    for w in words:
        ht.add(w)
    print(f"手写表: 不同单词 {ht.size} 个,装填因子 λ={ht.load_factor:.2f}")

    mp = defaultdict(int)                     # 任务 3:标准库 dict 重做
    for w in words:
        mp[w] += 1
    print(f"dict: 不同单词 {len(mp)} 个")
    for w in ("the", "hash", "hello"):
        print(f"  词 {w!r}: 手写表={ht.count(w)} dict={mp[w]}")

    print("任务4: 表大小(素数)不同 → λ 不同 → 插入耗时(3 次取最快):")
    for m in (8, 16, 64, 256):
        best = float("inf")
        for _ in range(3):                    # 重复 3 次取最快,减少系统噪声
            t = HashTable(m)
            t0 = time.perf_counter()
            for _ in range(30):               # 重复 30 遍放大差异
                for w in words:
                    t.add(w)
            best = min(best, (time.perf_counter() - t0) * 1000)
        print(f"  M={len(t.buckets)} λ={t.load_factor:.2f} 耗时 {best:.2f} ms")
    print("观察: λ 越大平均链表越长,耗时越高——这就是再散列(§5.4)的动机。")


if __name__ == "__main__":
    main()
