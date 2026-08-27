# lab1.py —— 递归综合实验(第一段:任务1与任务2)
import time

move_cnt = 0                      # 汉诺塔移动总步数

# 任务 1:汉诺塔。print_move=True 时打印每步
def hanoi(n, src, via, dst, print_move):
    global move_cnt
    if n == 1:                    # 基准情形:只剩一个盘子,直接搬
        if print_move:
            print(f"  {src} -> {dst}")
        move_cnt += 1
        return
    hanoi(n - 1, src, dst, via, print_move)   # 1. 上面 n-1 个搬到中转柱
    if print_move:
        print(f"  {src} -> {dst}")
    move_cnt += 1                 # 2. 最大盘子搬到目标柱
    hanoi(n - 1, via, src, dst, print_move)   # 3. n-1 个从中转柱搬到目标

# 任务 2:全排列(回溯)。固定前 k 位,把 a[k..] 逐个换到 a[k]
def permute(a, k, result):
    if k == len(a):               # 基准情形:最后一位也固定了
        result.append(a[:])       # 拷贝当前排列(避免后续回溯改掉它)
        return
    for i in range(k, len(a)):
        a[k], a[i] = a[i], a[k]   # 把 a[i] 换到第 k 位
        permute(a, k + 1, result) # 递归排列剩余部分
        a[k], a[i] = a[i], a[k]   # 回溯:换回来,还原现场

# 任务 3:朴素递归斐波那契(用于计时)
def fib_naive(n):
    if n <= 1:
        return n
    return fib_naive(n - 1) + fib_naive(n - 2)

def main():
    global move_cnt
    print("===== 任务 1:汉诺塔(n = 3)=====")
    hanoi(3, "A", "B", "C", True)
    print(f"移动步数: {move_cnt}(理论 2^3 - 1 = 7)")
    move_cnt = 0                  # 清零,再进入批量验证
    for n in range(1, 11):        # 验证 2^n - 1
        hanoi(n, "A", "B", "C", False)
        expect = 2 ** n - 1
        print(f"n = {n}: 实际 {move_cnt},理论 {expect}",
              "一致" if move_cnt == expect else "不一致!")
        move_cnt = 0

    print("\n===== 任务 2:全排列(n = 3)=====")
    result = []
    permute([1, 2, 3], 0, result)
    for p in result:
        print(" ".join(map(str, p)))
    print(f"排列总数: {len(result)}(理论 3! = 6)")
    result = []
    permute([1, 2, 3, 4], 0, result)   # 再验证 4! = 24
    print(f"n = 4 排列总数: {len(result)}(理论 4! = 24)")

    print("\n===== 任务 3:朴素斐波那契计时 =====")
    for n in (28, 31, 34):        # n 每次 +3,调用数约 ×4.2
        t0 = time.perf_counter()
        f = fib_naive(n)
        ms = (time.perf_counter() - t0) * 1000
        print(f"fib({n}) = {f},耗时 {ms:.1f} ms")
    print("观察: n 每加 3,耗时约乘 4~5(调用数 2*F(n+1) 的比值约 phi^3 = 4.2)")

if __name__ == "__main__":
    main()
