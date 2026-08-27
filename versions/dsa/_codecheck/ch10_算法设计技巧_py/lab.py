# lab10.py —— 0-1 背包与八皇后综合实验
import random
import time

# 任务 1a:二维 DP;返回 {价值, 方案下标}
def knap2d(n, W, w, v):
    dp = [[0] * (W + 1) for _ in range(n + 1)]
    for i in range(1, n + 1):
        for c in range(1, W + 1):
            if c >= w[i - 1]:
                dp[i][c] = max(dp[i - 1][c], dp[i - 1][c - w[i - 1]] + v[i - 1])
            else:
                dp[i][c] = dp[i - 1][c]
    chosen = []                       # 回溯选取方案
    i, c = n, W
    while i >= 1:
        if dp[i][c] != dp[i - 1][c]:
            chosen.append(i - 1)
            c -= w[i - 1]
        i -= 1
    chosen.reverse()
    return dp[n][W], chosen

# 任务 1b:一维滚动数组(容量必须逆序!)
def knap1d(n, W, w, v):
    dp = [0] * (W + 1)
    pick = [[False] * (W + 1) for _ in range(n)]
    for i in range(n):
        for c in range(W, w[i] - 1, -1):
            t = dp[c - w[i]] + v[i]
            if t > dp[c]:
                dp[c] = t
                pick[i][c] = True
    chosen = []
    i, c = n - 1, W
    while i >= 0:
        if pick[i][c]:
            chosen.append(i)
            c -= w[i]
        i -= 1
    chosen.reverse()
    return dp[W], chosen

# 任务 2:八皇后——通用回溯 + 闭包回调
queens_count = 0
first_sol = []

def n_queens(row, n, pos, col, d1, d2, visit):
    if row == n:
        visit(pos)
        return
    for c in range(n):
        if col[c] or d1[row + c] or d2[row - c + n - 1]:
            continue
        pos[row] = c
        col[c] = d1[row + c] = d2[row - c + n - 1] = True   # 选择
        n_queens(row + 1, n, pos, col, d1, d2, visit)       # 递归
        col[c] = d1[row + c] = d2[row - c + n - 1] = False  # 撤销

if __name__ == "__main__":
    random.seed(2026)                 # 固定种子,可复现
    # 任务 3:随机数据验证两版一致
    n, W = 20, 200
    w = [random.randint(1, 50) for _ in range(n)]
    v = [random.randint(1, 100) for _ in range(n)]
    r2 = knap2d(n, W, w, v)
    r1 = knap1d(n, W, w, v)
    print("随机数据 n=%d W=%d 价值: 二维=%d 一维=%d 方案一致: %s"
          % (n, W, r2[0], r1[0], "是" if r2 == r1 else "否"))
    # 任务 4:计时对比(n=1000, W=10000)
    n, W = 1000, 10000
    w = [random.randint(1, 50) for _ in range(n)]
    v = [random.randint(1, 100) for _ in range(n)]
    t0 = time.perf_counter()
    big2 = knap2d(n, W, w, v)         # 结果参与打印,防死代码消除
    ms2 = (time.perf_counter() - t0) * 1000
    t0 = time.perf_counter()
    big1 = knap1d(n, W, w, v)
    ms1 = (time.perf_counter() - t0) * 1000
    print("计时 n=%d W=%d 二维=%.1f ms 一维=%.1f ms 加速比=%.2f"
          % (n, W, ms2, ms1, ms2 / ms1))
    print("大样例价值一致: %s" % ("是" if big2[0] == big1[0] else "否"))
    # 八皇后(前 3 个解 + 总数)
    pos = [0] * 8
    col = [False] * 8
    d1 = [False] * 15
    d2 = [False] * 15

    def visit(sol):
        global queens_count
        queens_count += 1
        if len(first_sol) < 3:
            first_sol.append(sol[:])  # 拷贝,防撤销污染

    n_queens(0, 8, pos, col, d1, d2, visit)
    print("八皇后解的总数: %d" % queens_count)
    for i, sol in enumerate(first_sol, 1):
        print("解 %d: %s" % (i, " ".join(str(x + 1) for x in sol)))
    # 任务 5:边界——容量为 0、无物品
    wb = [2, 3, 4]
    vb = [3, 4, 5]
    print("容量 0: %d 无物品: %d" % (knap1d(3, 0, wb, vb)[0],
                                  knap1d(0, 10, wb, vb)[0]))
