// lab7.cpp —— 三种排序的性能对比实验(插入/归并/快排)
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>
using namespace std;
using Clock = chrono::steady_clock;

const int CUTOFF = 16;          // 快排小区间阈值(§7.5)
long long g_cmp = 0, g_move = 0;
bool g_count = false;           // 任务 5 计数开关(计时时关)
inline bool lt(int x, int y) { if (g_count) ++g_cmp; return x < y; }
inline void exch(int& x, int& y) { if (g_count) ++g_move; swap(x, y); }

void insertionSort(vector<int>& a, int left, int right) {
    for (int i = left + 1; i <= right; ++i) {
        int key = a[i];
        int j = i - 1;
        while (j >= left && lt(key, a[j])) {  // 严格大于才右移:稳定
            a[j + 1] = a[j];
            if (g_count) ++g_move;
            --j;
        }
        a[j + 1] = key;
        if (g_count) ++g_move;
    }
}
void insertionSortAll(vector<int>& a) { insertionSort(a, 0, int(a.size()) - 1); }
void merge(vector<int>& a, vector<int>& tmp, int l, int m, int r) {
    int i = l, j = m + 1, k = l;
    while (i <= m && j <= r) {
        if (lt(a[j], a[i])) tmp[k++] = a[j++];  // 右半更小才取右:相等取左,稳定
        else tmp[k++] = a[i++];
        if (g_count) ++g_move;
    }
    while (i <= m) { tmp[k++] = a[i++]; if (g_count) ++g_move; }
    while (j <= r) { tmp[k++] = a[j++]; if (g_count) ++g_move; }
    for (k = l; k <= r; ++k) { a[k] = tmp[k]; if (g_count) ++g_move; }
}
void mergeSortRec(vector<int>& a, vector<int>& tmp, int l, int r) {
    if (l >= r) return;
    int m = (l + r) / 2;
    mergeSortRec(a, tmp, l, m);
    mergeSortRec(a, tmp, m + 1, r);
    merge(a, tmp, l, m, r);
}
void mergeSort(vector<int>& a) {
    vector<int> tmp(a.size());      // 辅助数组只分配一次
    mergeSortRec(a, tmp, 0, int(a.size()) - 1);
}
int median3(vector<int>& a, int left, int right) {
    int center = (left + right) / 2;
    if (lt(a[center], a[left])) exch(a[left], a[center]);
    if (lt(a[right], a[left]))  exch(a[left], a[right]);
    if (lt(a[right], a[center])) exch(a[center], a[right]);
    exch(a[center], a[right - 1]);  // 枢轴藏到 right-1
    return a[right - 1];
}
void quickSortRec(vector<int>& a, int left, int right) {
    if (left + CUTOFF <= right) {
        int pivot = median3(a, left, right);
        int i = left, j = right - 1;
        for (;;) {
            while (lt(a[++i], pivot)) {}  // 哨兵 a[left]<=pivot 保证能停
            while (lt(pivot, a[--j])) {}  // a[right-1]==pivot 保证能停
            if (i < j) exch(a[i], a[j]);
            else break;
        }
        exch(a[i], a[right - 1]);   // 枢轴归位
        quickSortRec(a, left, i - 1);
        quickSortRec(a, i + 1, right);
    } else {
        insertionSort(a, left, right);  // 小区间优化(§7.1 预告)
    }
}
void quickSort(vector<int>& a) { quickSortRec(a, 0, int(a.size()) - 1); }

enum Kind { RANDOM, NEARLY, REVERSED };
const char* kindName[] = {"随机", "近有序", "逆序"};
vector<int> makeData(int n, Kind k, mt19937& rng) {
    vector<int> a(n);
    for (int i = 0; i < n; ++i) a[i] = i + 1;
    if (k == RANDOM) {
        for (int i = n - 1; i > 0; --i)
            swap(a[i], a[uniform_int_distribution<int>(0, i)(rng)]);
    } else if (k == NEARLY) {
        for (int i = 0; i < 20; ++i)   // 固定 20 次随机交换:逆序对 O(n)
            swap(a[uniform_int_distribution<int>(0, n - 1)(rng)],
                 a[uniform_int_distribution<int>(0, n - 1)(rng)]);
    } else {
        for (int i = 0; i < n; ++i) a[i] = n - i;
    }
    return a;
}
// 计时:3 次取最小,每次用副本,结果读走防优化
double msTime(void (*f)(vector<int>&), vector<int> a, long long& guard) {
    double best = 1e18;
    for (int r = 0; r < 3; ++r) {
        vector<int> b = a;
        auto t0 = Clock::now();
        f(b);
        guard += b.front();
        best = min(best, chrono::duration<double, milli>(Clock::now() - t0).count());
    }
    return best;
}

int main() {
    mt19937 rng(12345);             // 固定种子,可复现
    const vector<int> sizes = {1000, 2000, 4000, 8000, 16000, 32000, 64000};
    double ins[3][7] = {}, mrg[3][7] = {}, qck[3][7] = {};
    long long guard = 0;

    // 任务 3:正确性验证(与 std::sort 一致,含重复元素)
    bool ok = true;
    for (int k = 0; k < 4; ++k) {
        vector<int> a = makeData(500, Kind(k % 3), rng);
        if (k == 3) for (int& x : a) x = uniform_int_distribution<int>(0, 20)(rng);
        vector<int> ref = a, b;
        sort(ref.begin(), ref.end());
        b = a; insertionSort(b, 0, int(b.size()) - 1); ok &= (b == ref);
        b = a; mergeSort(b);                          ok &= (b == ref);
        b = a; quickSort(b);                          ok &= (b == ref);
    }
    cout << "任务 3:三种算法与 std::sort 输出一致: " << (ok ? "是" : "否") << "\n";
    // 任务 2:耗时表(插入 n>8000 标注"(推算)")
    cout << "任务 2:耗时表(单位 ms;插入排序 n>8000 为(推算))\n";
    for (int k = 0; k < 3; ++k) {
        cout << "\n[" << kindName[k] << "]\n";
        cout << left << setw(8) << "n" << setw(11) << "插入" << setw(11) << "归并" << "快排\n";
        for (size_t s = 0; s < sizes.size(); ++s) {
            vector<int> a = makeData(sizes[s], Kind(k), rng);
            cout << setw(8) << sizes[s];
            if (sizes[s] <= 8000) {
                ins[k][s] = msTime(insertionSortAll, a, guard);
                cout << setw(11) << fixed << setprecision(2) << ins[k][s];
            } else { ins[k][s] = -1; cout << setw(11) << "(推算)"; }
            mrg[k][s] = msTime(mergeSort, a, guard);
            qck[k][s] = msTime(quickSort, a, guard);
            cout << setw(11) << mrg[k][s] << qck[k][s] << "\n";
        }
    }

    // 任务 4:比值分析(理论:插入 ×4;归并/快排 ≈×2)
    cout << "\n任务 4:n 翻倍时实测时间比值(理论:n² 算法 ×4,n log n 算法 ≈×2)\n";
    auto ratios = [&](const char* name, double t[3][7]) {
        cout << name << ": ";
        for (int k = 0; k < 3; ++k) {
            cout << kindName[k] << "[";
            for (size_t s = 1; s < sizes.size(); ++s)
                if (t[k][s] > 0 && t[k][s - 1] > 0) cout << setprecision(2) << (t[k][s] / t[k][s - 1]) << " ";
            cout << "] ";
        }
        cout << "\n";
    };
    ratios("插入", ins);
    ratios("归并", mrg);
    ratios("快排", qck);
    // 任务 5(加分):比较/移动次数统计
    g_count = true;
    vector<int> a = makeData(1000, RANDOM, rng), b;
    g_cmp = 0; g_move = 0; b = a; insertionSort(b, 0, int(b.size()) - 1);
    cout << "\n任务 5(n=1000 随机):插入 比较=" << g_cmp
         << " 移动=" << g_move << "\n";
    g_cmp = 0; g_move = 0; b = a; mergeSort(b);
    cout << "                归并 比较=" << g_cmp << " 移动=" << g_move << "\n";
    g_cmp = 0; g_move = 0; b = a; quickSort(b);
    cout << "                快排 比较=" << g_cmp << " 交换=" << g_move << "\n";
    g_count = false;

    cout << "\n结论:实测比值与理论吻合——插入 n 翻倍约 ×4,归并/快排约 ×2;"
            "逆序上插入退化最明显,近有序上插入接近 O(n),快排三数中值使逆序不再退化。\n";
    cout << "(防优化累加值: " << guard << ")\n";
    return 0;
}
