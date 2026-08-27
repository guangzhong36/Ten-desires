// lab10.cpp —— 0-1 背包与八皇后综合实验
#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <random>
#include <vector>
using namespace std;
using Clock = chrono::steady_clock;

// 任务 1a:0-1 背包,二维 DP;返回 {最大价值, 选取的物品下标}
pair<int, vector<int>> knap2D(int n, int W, const vector<int>& w,
                              const vector<int>& v) {
    vector<vector<int>> dp(n + 1, vector<int>(W + 1, 0));
    for (int i = 1; i <= n; ++i)
        for (int c = 1; c <= W; ++c)
            dp[i][c] = (c >= w[i - 1])
                ? max(dp[i - 1][c], dp[i - 1][c - w[i - 1]] + v[i - 1])
                : dp[i - 1][c];
    vector<int> chosen;                       // 回溯选取方案
    for (int i = n, c = W; i >= 1; --i)
        if (dp[i][c] != dp[i - 1][c]) {
            chosen.push_back(i - 1);
            c -= w[i - 1];
        }
    reverse(chosen.begin(), chosen.end());
    return {dp[n][W], chosen};
}

// 任务 1b:一维滚动数组(容量必须逆序!);pick 记录选择
pair<int, vector<int>> knap1D(int n, int W, const vector<int>& w,
                              const vector<int>& v) {
    vector<int> dp(W + 1, 0);
    vector<vector<char>> pick(n, vector<char>(W + 1, 0));
    for (int i = 0; i < n; ++i)
        for (int c = W; c >= w[i]; --c) {
            int t = dp[c - w[i]] + v[i];
            if (t > dp[c]) {
                dp[c] = t;
                pick[i][c] = 1;
            }
        }
    vector<int> chosen;
    for (int i = n - 1, c = W; i >= 0; --i)
        if (pick[i][c]) {
            chosen.push_back(i);
            c -= w[i];
        }
    reverse(chosen.begin(), chosen.end());
    return {dp[W], chosen};
}

// 任务 2:八皇后——通用回溯 + std::function 回调
long long queensCount = 0;
vector<vector<int>> firstSol;                  // 前 3 个解
void nQueens(int row, int n, vector<int>& pos, vector<bool>& col,
             vector<bool>& d1, vector<bool>& d2,
             const function<void(const vector<int>&)>& visit) {
    if (row == n) {
        visit(pos);
        return;
    }
    for (int c = 0; c < n; ++c) {
        if (col[c] || d1[row + c] || d2[row - c + n - 1]) continue;
        pos[row] = c;
        col[c] = d1[row + c] = d2[row - c + n - 1] = true;   // 选择
        nQueens(row + 1, n, pos, col, d1, d2, visit);        // 递归
        col[c] = d1[row + c] = d2[row - c + n - 1] = false;  // 撤销
    }
}

int main() {
    mt19937 rng(2026);                        // 固定种子,结果可复现
    uniform_int_distribution<int> dw(1, 50), dv(1, 100);
    // 任务 3:随机数据验证两版一致
    int n = 20, W = 200;
    vector<int> w(n), v(n);
    for (int i = 0; i < n; ++i) {
        w[i] = dw(rng);
        v[i] = dv(rng);
    }
    auto r2 = knap2D(n, W, w, v), r1 = knap1D(n, W, w, v);
    cout << "随机数据 n=" << n << " W=" << W
         << " 价值: 二维=" << r2.first << " 一维=" << r1.first
         << " 方案一致: " << (r2 == r1 ? "是" : "否") << endl;
    // 任务 4:计时对比(n=1000, 容量 10000 级别)
    n = 1000;
    W = 10000;
    w.assign(n, 0);
    v.assign(n, 0);
    for (int i = 0; i < n; ++i) {
        w[i] = dw(rng);
        v[i] = dv(rng);
    }
    auto t0 = Clock::now();
    auto big2 = knap2D(n, W, w, v);           // 累加防死代码消除
    double ms2 = chrono::duration<double, milli>(Clock::now() - t0).count();
    t0 = Clock::now();
    auto big1 = knap1D(n, W, w, v);
    double ms1 = chrono::duration<double, milli>(Clock::now() - t0).count();
    cout << "计时 n=" << n << " W=" << W << " 二维=" << ms2
         << " ms 一维=" << ms1 << " ms 加速比=" << ms2 / ms1 << endl;
    cout << "大样例价值一致: " << (big2.first == big1.first ? "是" : "否")
         << endl;
    // 任务 2:八皇后(前 3 个解 + 总数)
    vector<int> pos(8);
    vector<bool> col(8), d1(15), d2(15);
    nQueens(0, 8, pos, col, d1, d2, [&](const vector<int>& sol) {
        ++queensCount;
        if (firstSol.size() < 3) firstSol.push_back(sol);
    });
    cout << "八皇后解的总数: " << queensCount << endl;
    for (size_t i = 0; i < firstSol.size(); ++i) {
        cout << "解 " << i + 1 << ": ";
        for (int x : firstSol[i]) cout << x + 1 << ' ';
        cout << endl;
    }
    // 任务 5:边界——容量为 0、无物品
    vector<int> wb = {2, 3, 4}, vb = {3, 4, 5};
    cout << "容量 0: " << knap1D(3, 0, wb, vb).first
         << " 无物品: " << knap1D(0, 10, wb, vb).first << endl;
    return 0;
}
