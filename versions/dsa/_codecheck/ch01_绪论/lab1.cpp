// lab1.cpp —— 递归综合实验:汉诺塔与全排列(第一段:任务1与任务2)
#include <iostream>
#include <vector>
#include <chrono>
using namespace std;

long long moveCnt = 0;                    // 汉诺塔移动总步数

// 任务 1:汉诺塔。printMove 为 true 时打印每步,否则只计数
void hanoi(int n, char from, char via, char to, bool printMove) {
    if (n == 1) {                         // 基准情形:只剩一个盘子,直接搬
        if (printMove) cout << "  " << from << " -> " << to << endl;
        ++moveCnt;
        return;
    }
    hanoi(n - 1, from, to, via, printMove);  // 1. 上面 n-1 个搬到中转柱
    if (printMove) cout << "  " << from << " -> " << to << endl;
    ++moveCnt;                              // 2. 最大盘子搬到目标柱
    hanoi(n - 1, via, from, to, printMove); // 3. n-1 个从中转柱搬到目标
}

// 任务 2:递归生成全排列(回溯)。固定前 k 位,把 a[k..] 逐个换到 a[k]
void permute(vector<int>& a, int k, long long& cnt) {
    if (k == (int)a.size()) {             // 基准情形:最后一位也固定了
        for (size_t i = 0; i < a.size(); ++i)
            cout << a[i] << (i + 1 == a.size() ? "\n" : " ");
        ++cnt;
        return;
    }
    for (int i = k; i < (int)a.size(); ++i) {
        swap(a[k], a[i]);                 // 把 a[i] 换到第 k 位
        permute(a, k + 1, cnt);           // 递归排列剩余部分
        swap(a[k], a[i]);                 // 回溯:换回来,还原现场
    }
}
// 任务 3:朴素递归斐波那契(用于计时)
long long fibNaive(int n) {
    if (n <= 1) return n;
    return fibNaive(n - 1) + fibNaive(n - 2);
}

int main() {
    cout << "===== 任务 1:汉诺塔(n = 3)=====\n";
    hanoi(3, 'A', 'B', 'C', true);
    cout << "移动步数: " << moveCnt << "(理论 2^3 - 1 = 7)\n";
    moveCnt = 0;                          // 清零,再进入批量验证
    for (int n = 1; n <= 10; ++n) {       // 验证 2^n - 1
        hanoi(n, 'A', 'B', 'C', false);
        long long expect = (1LL << n) - 1;
        cout << "n = " << n << ": 实际 " << moveCnt
             << ",理论 " << expect
             << (moveCnt == expect ? "  一致" : "  不一致!") << endl;
        moveCnt = 0;
    }

    cout << "\n===== 任务 2:全排列(n = 3)=====\n";
    vector<int> a = {1, 2, 3};
    long long permCnt = 0;
    permute(a, 0, permCnt);
    cout << "排列总数: " << permCnt << "(理论 3! = 6)\n";
    permCnt = 0;
    vector<int> b = {1, 2, 3, 4};         // 再验证 4! = 24
    permute(b, 0, permCnt);
    cout << "n = 4 排列总数: " << permCnt << "(理论 4! = 24)\n";

    cout << "\n===== 任务 3:朴素斐波那契计时 =====\n";
    for (int n : {32, 35, 38}) {          // n 每次 +3,调用数约 ×4.2
        auto t0 = chrono::steady_clock::now();
        long long f = fibNaive(n);
        double ms = chrono::duration<double, milli>
                    (chrono::steady_clock::now() - t0).count();
        cout << "fib(" << n << ") = " << f << ",耗时 " << ms << " ms\n";
    }
    cout << "观察: n 每加 3,耗时约乘 4~5(调用数 2*F(n+1) 的比值约 phi^3 = 4.2)\n";
    return 0;
}
