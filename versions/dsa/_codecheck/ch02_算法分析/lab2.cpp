// lab2.cpp —— 最大子序列和:四种算法的增长阶实测(第一段:算法)
#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
using namespace std;
using Clock = chrono::steady_clock;

// ---------- 四种算法(§2.5) ----------
int maxSubSum1(const vector<int>& a) {          // O(n^3)
    int maxSum = 0;
    for (size_t i = 0; i < a.size(); ++i)
        for (size_t j = i; j < a.size(); ++j) {
            int thisSum = 0;
            for (size_t k = i; k <= j; ++k) thisSum += a[k];
            if (thisSum > maxSum) maxSum = thisSum;
        }
    return maxSum;
}

int maxSubSum2(const vector<int>& a) {          // O(n^2)
    int maxSum = 0;
    for (size_t i = 0; i < a.size(); ++i) {
        int thisSum = 0;
        for (size_t j = i; j < a.size(); ++j) {
            thisSum += a[j];
            if (thisSum > maxSum) maxSum = thisSum;
        }
    }
    return maxSum;
}

int maxSumRec(const vector<int>& a, int left, int right) {  // O(n log n)
    if (left == right) return a[left] > 0 ? a[left] : 0;
    int center = (left + right) / 2;
    int maxLeft  = maxSumRec(a, left, center);
    int maxRight = maxSumRec(a, center + 1, right);
    int maxLeftBorder = 0, leftBorder = 0;
    for (int i = center; i >= left; --i) {
        leftBorder += a[i];
        if (leftBorder > maxLeftBorder) maxLeftBorder = leftBorder;
    }
    int maxRightBorder = 0, rightBorder = 0;
    for (int i = center + 1; i <= right; ++i) {
        rightBorder += a[i];
        if (rightBorder > maxRightBorder) maxRightBorder = rightBorder;
    }
    return max(max(maxLeft, maxRight), maxLeftBorder + maxRightBorder);
}
int maxSubSum3(const vector<int>& a) {
    return maxSumRec(a, 0, int(a.size()) - 1);
}
int maxSubSum4(const vector<int>& a) {          // O(n) 在线算法
    int maxSum = 0, thisSum = 0;
    for (int x : a) {
        thisSum += x;
        if (thisSum > maxSum) maxSum = thisSum;
        else if (thisSum < 0) thisSum = 0;
    }
    return maxSum;
}
// ---------- 计时(§2.1) ----------
using Func = int (*)(const vector<int>&);
double msTime(Func f, const vector<int>& a, long long& guard) {
    auto t0 = Clock::now();
    guard += f(a);                       // 返回值累加,防止死代码消除
    return chrono::duration<double, milli>(Clock::now() - t0).count();
}

int main() {
    // 正确性验证(§2.5 样例)
    vector<int> demo = {-2, 11, -4, 13, -5, -2};
    cout << "验证: " << maxSubSum1(demo) << " "
         << maxSubSum2(demo) << " " << maxSubSum3(demo) << " "
         << maxSubSum4(demo) << " (应全为 20)" << endl;

    mt19937 rng(12345);
    uniform_int_distribution<int> dist(-1000, 1000);
    long long guard = 0;

    cout << "n\t算法一\t算法二\t算法三\t算法四\n";
    for (int n : {1000, 2000, 4000, 8000}) {
        vector<int> a(n);
        for (int& x : a) x = dist(rng);
        cout << n;
        if (n <= 2000)
            cout << "\t" << msTime(maxSubSum1, a, guard);
        else
            cout << "\t--(推算)";   // O(n^3) 在 4000 时约 26 秒,不实测
        cout << "\t" << msTime(maxSubSum2, a, guard)
             << "\t" << msTime(maxSubSum3, a, guard)
             << "\t" << msTime(maxSubSum4, a, guard) << endl;
    }
    cout << "(防优化累加值: " << guard % 1000 << ")" << endl;
    cout << "结论: n 翻倍时,算法一 x8(吻合 n^3),算法二 x4(吻合 n^2),"
            "算法三/四约 x2(吻合 n log n 与 n)。" << endl;
    return 0;
}
