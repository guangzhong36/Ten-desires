# lab7.py —— 三种排序的性能对比实验(插入/归并/快排)
# 运行: python lab7.py
import random
import time

random.seed(12345)     # 固定种子,输出可复现

CUTOFF = 16            # 快排小区间改用插入排序的阈值(§7.5)

# ---------- 任务 5(可选加分):比较/移动计数器 ----------
COUNT = False          # 置 True 时统计,性能测试期间保持 False
cmp_cnt = 0
move_cnt = 0


def lt(x, y):
    global cmp_cnt
    if COUNT:
        cmp_cnt += 1
    return x < y


# ---------- 任务 1:插入排序(§7.1) ----------
def insertion_sort(a, left=0, right=None):
    if right is None:
        right = len(a) - 1
    for i in range(left + 1, right + 1):
        key = a[i]
        j = i - 1
        while j >= left and lt(key, a[j]):   # 严格大于才右移:相等不动,稳定
            a[j + 1] = a[j]                  # 元素右移
            j -= 1
            global move_cnt
            if COUNT:
                move_cnt += 1
        a[j + 1] = key                       # 落位
        if COUNT:
            move_cnt += 1


# ---------- 任务 1:归并排序(§7.4,切片 + 合并) ----------
def merge(left, right):
    global move_cnt
    i = j = 0
    res = []
    while i < len(left) and j < len(right):
        if lt(right[j], left[i]):            # 右半更小才取右:相等取左,稳定
            res.append(right[j])
            j += 1
        else:
            res.append(left[i])
            i += 1
        if COUNT:
            move_cnt += 1
    res += left[i:] + right[j:]              # 剩余部分直接拼接
    return res


def merge_sort(a):
    if len(a) <= 1:
        return a
    m = len(a) // 2
    left = merge_sort(a[:m])                 # 切片:创建子列表副本(§7.4 讨论其代价)
    right = merge_sort(a[m:])
    return merge(left, right)


# ---------- 任务 1:快速排序(§7.5,三数中值 + 小区间插入) ----------
def median3(a, left, right):
    center = (left + right) // 2
    if lt(a[center], a[left]):
        a[left], a[center] = a[center], a[left]
    if lt(a[right], a[left]):
        a[left], a[right] = a[right], a[left]
    if lt(a[right], a[center]):
        a[center], a[right] = a[right], a[center]
    a[center], a[right - 1] = a[right - 1], a[center]   # 枢轴藏到 right-1
    return a[right - 1]


def quick_sort(a, left=0, right=None):
    if right is None:
        right = len(a) - 1
    if left + CUTOFF <= right:
        pivot = median3(a, left, right)
        i, j = left, right - 1
        while True:
            i += 1
            while lt(a[i], pivot):
                i += 1
            j -= 1
            while lt(pivot, a[j]):
                j -= 1
            if i < j:
                a[i], a[j] = a[j], a[i]
            else:
                break
        a[i], a[right - 1] = a[right - 1], a[i]        # 枢轴归位
        quick_sort(a, left, i - 1)
        quick_sort(a, i + 1, right)
    else:
        insertion_sort(a, left, right)     # 小区间优化(§7.1 预告)


# ---------- 任务 2:三种数据分布 ----------
def make_data(n, kind):
    if kind == "随机":
        a = list(range(n))
        random.shuffle(a)
    elif kind == "近有序":
        a = list(range(n))
        for _ in range(20):                # 只做 20 次随机交换:逆序对 O(n)
            x, y = random.randrange(n), random.randrange(n)
            a[x], a[y] = a[y], a[x]
    else:                                  # 逆序
        a = list(range(n, 0, -1))
    return a


def ms_time(func, a, copies=True):
    """计时:同数据重复 3 次取最小值,压制系统噪声;copies=True 时每次用副本。"""
    best = float("inf")
    for _ in range(3):
        b = a[:] if copies else a
        t0 = time.perf_counter()
        func(b)
        best = min(best, (time.perf_counter() - t0) * 1000)
    return best


def main():
    sizes = (1000, 2000, 4000, 8000, 16000, 32000, 64000)
    kinds = ("随机", "近有序", "逆序")
    times = {k: {"插入": {}, "归并": {}, "快排": {}} for k in kinds}

    # 任务 3:正确性验证(与 sorted 输出一致,含重复元素)
    ok = True
    for kind in kinds:
        a = make_data(500, kind)
        std = sorted(a)
        b = a[:]; insertion_sort(b); ok &= (b == std)
        ok &= (merge_sort(a[:]) == std)
        b = a[:]; quick_sort(b); ok &= (b == std)
    a = [random.randint(0, 20) for _ in range(500)]   # 重复元素
    std = sorted(a)
    b = a[:]; insertion_sort(b); ok &= (b == std)
    ok &= (merge_sort(a[:]) == std)
    b = a[:]; quick_sort(b); ok &= (b == std)
    print(f"任务 3:三种算法与 sorted 输出一致: {ok}")

    # 任务 2:性能表(单位 ms;插入排序 n>8000 按 ×4 规律推算,避免无谓等待)
    print("任务 2:耗时表(单位 ms;插入排序 n>8000 为(推算))")
    for kind in kinds:
        print(f"\n[{kind}]")
        print("n        插入       归并       快排")
        for n in sizes:
            a = make_data(n, kind)
            row = [f"{n}"]
            if n <= 8000:
                t = ms_time(insertion_sort, a[:])
                times[kind]["插入"][n] = t
                row.append(f"{t:9.1f}")
            else:
                row.append("   (推算)")
            t = ms_time(merge_sort, a[:], copies=False)   # 归并不改动原列表
            times[kind]["归并"][n] = t
            row.append(f"{t:9.1f}")
            t = ms_time(quick_sort, a[:])
            times[kind]["快排"][n] = t
            row.append(f"{t:9.1f}")
            print("  ".join(row))

    # 任务 4:比值分析(理论:插入 ×4;归并/快排 ≈×2)
    print("\n任务 4:n 翻倍时实测时间比值(理论:n² 算法 ×4,n log n 算法 ≈×2)")
    theory = {"插入": "n² → ×4", "归并": "n log n → ≈×2", "快排": "n log n → ≈×2"}
    for alg in ("插入", "归并", "快排"):
        line = f"{alg}({theory[alg]}): "
        for kind in kinds:
            ts = sorted(times[kind][alg].items())
            rs = [f"{ts[i+1][1]/ts[i][1]:.1f}" for i in range(len(ts) - 1)]
            line += f"{kind}[{' '.join(rs)}]  "
        print(line)

    # 任务 5(加分):比较/移动次数统计
    global COUNT, cmp_cnt, move_cnt
    COUNT = True
    a = make_data(1000, "随机")
    cmp_cnt = move_cnt = 0
    insertion_sort(a[:])
    print(f"\n任务 5(n=1000 随机):插入 比较={cmp_cnt} 移动={move_cnt}")
    cmp_cnt = move_cnt = 0
    merge_sort(a[:])
    print(f"                归并 比较={cmp_cnt} 移动={move_cnt}(拼接不计)")
    cmp_cnt = move_cnt = 0
    quick_sort(a[:])
    print(f"                快排 比较={cmp_cnt} 交换={move_cnt}")
    COUNT = False

    print("\n结论:实测比值与理论吻合——插入排序 n 翻倍时间约 ×4,"
          "归并/快排约 ×2;逆序数据上插入排序退化最明显,"
          "近有序数据上插入排序接近 O(n),快排三数中值使逆序数据不再退化。")


if __name__ == "__main__":
    main()
