# lab11.py —— 实验 11:动态数组扩容的摊还代价实测
# 运行:python lab11.py
import sys

# ---------- 任务 1:带真实拷贝计数器的倍增动态数组 ----------
class DoublingArray:
    """倍增扩容:容量满时 ×2,记录每次 push_back 的真实拷贝次数。"""
    def __init__(self):
        self._buf = [None] * 1        # 底层存储,初始容量 1
        self._size = 0                # 元素个数
        self._cap = 1                 # 容量
        self.total_copies = 0         # 累计拷贝次数(真实代价总和)
        self.last_cost = 0            # 最近一次 push 付出的拷贝次数
        self.max_cost = 0             # 单次 push 的最大拷贝次数

    def push_back(self, x):
        self.last_cost = 0
        if self._size == self._cap:   # 容量已满:倍增扩容
            new_buf = [None] * (self._cap * 2)
            for i in range(self._size):
                new_buf[i] = self._buf[i]     # 真实拷贝 1 次
            self.last_cost = self._size       # 本次操作的真实拷贝代价
            self.total_copies += self._size
            if self.last_cost > self.max_cost:
                self.max_cost = self.last_cost
            self._buf = new_buf
            self._cap *= 2
        self._buf[self._size] = x
        self._size += 1

# ---------- 任务 1:每次只多 1 格的"笨扩容"(对照策略) ----------
class LinearArray:
    """每次容量只 +1:摊还代价退化为 O(n),用于对照。"""
    def __init__(self):
        self._buf = [None] * 1
        self._size = 0
        self._cap = 1
        self.total_copies = 0
        self.last_cost = 0
        self.max_cost = 0

    def push_back(self, x):
        self.last_cost = 0
        if self._size == self._cap:   # 容量已满:只多开 1 格
            new_buf = [None] * (self._cap + 1)
            for i in range(self._size):
                new_buf[i] = self._buf[i]
            self.last_cost = self._size
            self.total_copies += self._size
            if self.last_cost > self.max_cost:
                self.max_cost = self.last_cost
            self._buf = new_buf
            self._cap += 1
        self._buf[self._size] = x
        self._size += 1

# ---------- 任务 4:会计方法视角的"存 3 元"动态数组 ----------
# 每次 push 收 3 元:1 元支付"写入自己",2 元存入账户;
# 扩容时每拷贝 1 个旧元素,从账户支付 1 元。
# 证明(设 size = s,容量 = c = 2^k):
#   总收费 = 3s;总花费 = s + (1+2+...+c/2) = s + c - 1;
#   余额 = 3s - s - (c-1) = 2s - c + 1 ≥ 1 > 0。
# 所以"存 3 元"永远够用——摊还 O(1) 的会计证明。
class BankArray:
    def __init__(self):
        self._buf = [None] * 1
        self._size = 0
        self._cap = 1
        self.balance = 0              # 账户余额
        self.min_balance = float("inf")   # 全程最低余额(应恒 ≥ 0)

    def push_back(self, x):
        self.balance += 3             # 收 3 元
        self.balance -= 1             # 支付本次写入
        if self._size == self._cap:   # 扩容:每拷 1 个元素付 1 元
            new_buf = [None] * (self._cap * 2)
            for i in range(self._size):
                new_buf[i] = self._buf[i]
                self.balance -= 1
            self._buf = new_buf
            self._cap *= 2
        self._buf[self._size] = x
        self._size += 1
        if self.balance < self.min_balance:
            self.min_balance = self.balance

def main():
    N = 10000   # 连续插入的元素个数

    # ---- 任务 2、3:两种扩容策略的拷贝统计与代价对比 ----
    d = DoublingArray()
    for i in range(N):
        d.push_back(i)
    print(f"[任务2/3] 倍增策略:连续插入 {N} 个元素")
    print(f"  总拷贝次数 = {d.total_copies} (理论 ≈ 2n = {2 * N})")
    print(f"  单次最大代价 = {d.max_cost} 次拷贝(仅发生在扩容的那一次 push)")
    print(f"  平均代价 = {d.total_copies / N:.4f} 次拷贝/push\n")

    l = LinearArray()
    for i in range(N):
        l.push_back(i)
    print(f"[任务2/3] 笨扩容(每次 +1):连续插入 {N} 个元素")
    print(f"  总拷贝次数 = {l.total_copies} (理论 ≈ n^2/2 = {N * N // 2})")
    print(f"  单次最大代价 = {l.max_cost} 次拷贝(最后一次扩容)")
    print(f"  平均代价 = {l.total_copies / N:.4f} 次拷贝/push\n")

    # ---- 任务 4:会计方法验证"存 3 元"够不够 ----
    b = BankArray()
    for i in range(N):
        b.push_back(i)
    print(f"[任务4] 会计方法:存 3 元策略运行 {N} 次 push 后")
    print(f"  账户余额 = {b.balance}, 全程最低余额 = {b.min_balance} (非负即证明有效)\n")

    # ---- 任务 5:标准库 list 的扩容拐点(用 sys.getsizeof 观察) ----
    a = []
    last = sys.getsizeof(a)
    print("[任务5] Python list 扩容拐点(元素数 → 占用字节,前 200 个元素):")
    row = []
    for i in range(1, 201):
        a.append(i)
        cur = sys.getsizeof(a)
        if cur != last:               # 内存变化 = 一次扩容拐点
            row.append(f"{len(a)}→{cur}B")
            if len(row) == 12:
                print("  " + "  ".join(row))
                row = []
            last = cur
    if row:
        print("  " + "  ".join(row))
    print("\n结论: CPython 的 list 按比例增长(约 1/8 增量),拐点约 1.125 倍,"
          "虽不是严格的 2 倍,但几何增长 ⇒ 摊还 O(1) 不变,与本章理论一致。")

if __name__ == "__main__":
    main()
