# lab.py —— 迷宫生成与连通分量统计(并查集综合应用)
# 运行:python lab.py
import random
import time

# ---------- 任务 1:并查集(按秩合并 + 路径压缩) ----------
class UnionFind:
    """按秩合并 + 路径压缩的并查集,parent[i] == i 表示 i 是根。"""

    def __init__(self, n):
        self.parent = list(range(n))   # parent[i] = i 表示 i 是根
        self.rank = [0] * n            # 秩:树高的上界,只在根上有意义
        self.count = n                 # 当前连通分量个数

    def find(self, x):
        """迭代版路径压缩:先上溯找根,再沿原路把沿途结点挂到根。"""
        root = x
        while self.parent[root] != root:
            root = self.parent[root]
        while self.parent[x] != x:     # 第二趟:逐个压缩
            nxt = self.parent[x]
            self.parent[x] = root
            x = nxt
        return root

    def union(self, a, b):
        """合并 a、b 所在集合;合并成功返回 True,已在同一集合返回 False。"""
        ra, rb = self.find(a), self.find(b)
        if ra == rb:
            return False
        if self.rank[ra] < self.rank[rb]:   # 浅树挂到深树下
            ra, rb = rb, ra
        self.parent[rb] = ra
        if self.rank[ra] == self.rank[rb]:
            self.rank[ra] += 1              # 两棵同秩树合并,新树秩 +1
        self.count -= 1                     # 成功合并一次,连通分量减 1
        return True

# ---------- 任务 4 对比用:朴素版(无任何优化) ----------
class NaiveUF:
    def __init__(self, n):
        self.parent = list(range(n))

    def find(self, x):
        while self.parent[x] != x:
            x = self.parent[x]
        return x

    def union(self, a, b):
        a, b = self.find(a), self.find(b)
        if a != b:
            self.parent[a] = b       # 固定让 a 挂到 b 下,容易形成长链

# ---------- 任务 2/3:随机迷宫生成 ----------
def gen_maze(w, h, seed=2024):
    uf = UnionFind(w * h)
    right = [[True] * w for _ in range(h)]   # 右墙
    down = [[True] * w for _ in range(h)]    # 下墙
    walls = []
    for r in range(h):
        for c in range(w):
            if c + 1 < w:
                walls.append((r, c, r, c + 1))   # 右墙,连接 (r,c) 与 (r,c+1)
            if r + 1 < h:
                walls.append((r, c, r + 1, c))   # 下墙,连接 (r,c) 与 (r+1,c)
    random.seed(seed)
    random.shuffle(walls)
    record = []                             # 任务 3:连通分量变化采样
    key = [len(walls) // 4, len(walls) // 2, len(walls) * 3 // 4]
    ki = 0
    for idx, (r1, c1, r2, c2) in enumerate(walls, 1):
        if uf.union(r1 * w + c1, r2 * w + c2):   # 两端不连通才拆墙
            if c1 == c2:
                down[r1][c1] = False
            else:
                right[r1][c1] = False
        if ki < 3 and idx >= key[ki]:
            record.append((idx, uf.count))
            ki += 1
    record.append((len(walls), uf.count))   # 结束时必为 1
    return right, down, record

def render(w, h, right, down):
    """把迷宫画成 ASCII:每格 2 字符宽,"+" 是柱子,"--" 是横墙,"|" 是竖墙。"""
    lines = ["+" + "--+" * w]               # 顶部横线(永不拆)
    for r in range(h):
        row = "|"                           # 最左竖墙(永不拆)
        for c in range(w):
            row += "  " + ("|" if right[r][c] else " ")
        lines.append(row)
        if r == h - 1:
            lines.append("+" + "--+" * w)   # 底部横线(永不拆)
        else:
            line = "+"
            for c in range(w):
                line += "--" if down[r][c] else "  "
                line += "+"
            lines.append(line)
    return "\n".join(lines)

# ---------- 任务 4:朴素版 vs 完整优化版性能对比 ----------
def bench(n):
    ops = [(i, i + 1) for i in range(n - 1)]   # 顺序合并:朴素版会连成长链
    t0 = time.perf_counter()
    naive = NaiveUF(n)
    for a, b in ops:
        naive.union(a, b)                      # 每次 union 都沿链走 O(n)
    t1 = time.perf_counter()
    s = 0
    for x in range(n):
        s += naive.find(x)                     # 链尾 find 一步 O(n)
    t2 = time.perf_counter()
    opt = UnionFind(n)
    for a, b in ops:
        opt.union(a, b)
    t3 = time.perf_counter()
    for x in range(n):
        s += opt.find(x)
    t4 = time.perf_counter()
    ms = lambda a, b: (b - a) * 1000
    print(f"任务4: {n} 次 union(连成链)+ {n} 次 find(耗时对比):")
    print(f"  朴素版: union 阶段 {ms(t0, t1):.0f} ms, find 阶段 {ms(t1, t2):.0f} ms")
    print(f"  优化版: union 阶段 {ms(t2, t3):.0f} ms, find 阶段 {ms(t3, t4):.0f} ms")
    print(f"  find 阶段加速比:约 {(t2 - t1) / (t4 - t3):.0f} 倍(检查值 s={s % 1000})")

def main():
    w, h = 6, 4
    right, down, record = gen_maze(w, h, seed=2024)
    print("=" * 44)
    print(f"随机迷宫 {w} 列 x {h} 行(种子 2024,共 {w * h} 个格子):")
    print(render(w, h, right, down))
    print()
    total = record[-1][0]
    print(f"任务3: 拆墙过程中连通分量数量的变化(共 {total} 面候选墙):")
    print(f"  初始: {w * h} 个连通分量")
    for tried, comp in record:
        print(f"  尝试 {tried:2d}/{total} 面墙后: 连通分量 {comp:2d}")
    print(f"  结束时连通分量为 1,迷宫全连通、无环(恰好拆 {w * h - 1} 面墙)。")
    print()
    bench(5000)

if __name__ == "__main__":
    main()
