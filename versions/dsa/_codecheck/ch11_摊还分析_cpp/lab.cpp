// lab11.cpp —— 实验 11:动态数组扩容的摊还代价实测
// 编译:g++ -std=c++17 -O2 -o lab11 lab11.cpp ;运行 ./lab11
#include <iostream>
#include <vector>
using namespace std;

// ---------- 任务 1:带真实拷贝计数器的倍增动态数组 ----------
class DoublingArray {
    int* data_ = nullptr;
    size_t size_ = 0, cap_ = 0;
public:
    long long totalCopies = 0;  // 累计拷贝次数(真实代价总和)
    long long lastCost = 0;     // 最近一次 push 操作付出的拷贝次数
    long long maxCost = 0;      // 单次 push 的最大拷贝次数
    void push_back(int x) {
        lastCost = 0;
        if (size_ == cap_) {                  // 容量已满:倍增扩容
            size_t newCap = cap_ == 0 ? 1 : cap_ * 2;
            int* p = new int[newCap];
            for (size_t i = 0; i < size_; ++i) p[i] = data_[i];  // 真实拷贝
            lastCost = (long long)size_;      // 本次操作的真实拷贝代价
            totalCopies += size_;
            if (lastCost > maxCost) maxCost = lastCost;
            delete[] data_;
            data_ = p;
            cap_ = newCap;
        }
        data_[size_++] = x;
    }
    ~DoublingArray() { delete[] data_; }
};

// ---------- 任务 1:每次只多 1 格的"笨扩容"(对照策略) ----------
class LinearArray {
    int* data_ = nullptr;
    size_t size_ = 0, cap_ = 0;
public:
    long long totalCopies = 0;
    long long lastCost = 0;
    long long maxCost = 0;
    void push_back(int x) {
        lastCost = 0;
        if (size_ == cap_) {                  // 容量已满:只多开 1 格
            int* p = new int[cap_ + 1];
            for (size_t i = 0; i < size_; ++i) p[i] = data_[i];
            lastCost = (long long)size_;
            totalCopies += size_;
            if (lastCost > maxCost) maxCost = lastCost;
            delete[] data_;
            data_ = p;
            ++cap_;
        }
        data_[size_++] = x;
    }
    ~LinearArray() { delete[] data_; }
};

// ---------- 任务 4:会计方法视角的"存 3 元"动态数组 ----------
// 每次 push 收 3 元:1 元支付"写入自己",2 元存入账户。
// 扩容时每拷贝 1 个旧元素,从账户支付 1 元。
// 证明(设 size = s,容量 = c = 2^k,已扩容序列 1,2,...,c/2):
//   总收费 = 3s;总花费 = s(写入)+ (1+2+...+c/2)(拷贝)= s + c - 1;
//   余额 = 3s - s - (c-1) = 2s - c + 1 ≥ 2*(c/2) - c + 1 = 1 > 0。
// 所以"存 3 元"永远够用——这正是摊还 O(1) 的会计证明。
class BankArray {
    int* data_ = nullptr;
    size_t size_ = 0, cap_ = 0;
public:
    long long balance = 0;      // 账户余额
    long long minBalance = 1LL << 60;   // 全程最低余额(应恒 ≥ 0)
    void push_back(int x) {
        balance += 3;                       // 收 3 元
        balance -= 1;                       // 支付本次写入
        if (size_ == cap_) {
            size_t newCap = cap_ == 0 ? 1 : cap_ * 2;
            int* p = new int[newCap];
            for (size_t i = 0; i < size_; ++i) { p[i] = data_[i]; balance -= 1; }
            delete[] data_;
            data_ = p;
            cap_ = newCap;
        }
        data_[size_++] = x;
        if (balance < minBalance) minBalance = balance;
    }
    ~BankArray() { delete[] data_; }
};

int main() {
    const int N = 10000;   // 连续插入的元素个数

    // ---- 任务 2、3:两种扩容策略的拷贝统计与代价对比 ----
    DoublingArray d;
    for (int i = 0; i < N; ++i) d.push_back(i);
    cout << "[任务2/3] 倍增策略:连续插入 " << N << " 个元素\n"
         << "  总拷贝次数 = " << d.totalCopies
         << " (理论 ≈ 2n = " << 2 * N << ")\n"
         << "  单次最大代价 = " << d.maxCost << " 次拷贝(仅发生在扩容的那一次 push)\n"
         << "  平均代价 = " << (double)d.totalCopies / N << " 次拷贝/push\n\n";

    LinearArray l;
    for (int i = 0; i < N; ++i) l.push_back(i);
    cout << "[任务2/3] 笨扩容(每次 +1):连续插入 " << N << " 个元素\n"
         << "  总拷贝次数 = " << l.totalCopies
         << " (理论 ≈ n^2/2 = " << (long long)N * N / 2 << ")\n"
         << "  单次最大代价 = " << l.maxCost << " 次拷贝(最后一次扩容)\n"
         << "  平均代价 = " << (double)l.totalCopies / N << " 次拷贝/push\n\n";

    // ---- 任务 4:会计方法验证"存 3 元"够不够 ----
    BankArray b;
    for (int i = 0; i < N; ++i) b.push_back(i);
    cout << "[任务4] 会计方法:存 3 元策略运行 " << N << " 次 push 后\n"
         << "  账户余额 = " << b.balance
         << ", 全程最低余额 = " << b.minBalance << " (非负即证明有效)\n\n";

    // ---- 任务 5:标准库 vector 的 capacity 拐点 ----
    {
        vector<int> v;
        size_t prev = 0;
        cout << "[任务5] std::vector 扩容拐点(size → capacity):\n  ";
        for (int i = 1; i <= 2048; ++i) {
            v.push_back(i);
            if (v.capacity() != prev) {      // 容量变化 = 一次扩容拐点
                cout << v.size() << "→" << v.capacity() << "  ";
                prev = v.capacity();
            }
        }
        cout << "\n\n结论: vector 采用倍增策略,拐点序列 1,2,4,8,...,"
                "总拷贝 ≈ 2n,与本章理论完全一致。" << endl;
    }
    return 0;
}
