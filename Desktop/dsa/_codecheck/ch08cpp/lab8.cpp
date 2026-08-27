// lab.cpp —— 迷宫生成与连通分量统计(第一段:并查集与朴素版)
// 编译:g++ -std=c++17 -O2 -o lab lab.cpp   运行:./lab
#include <iostream>
#include <vector>
#include <numeric>
#include <random>
#include <chrono>
#include <string>
#include <utility>
#include <algorithm>
using namespace std;
using Clock = chrono::steady_clock;

// ---------- 任务 1:并查集(按秩合并 + 路径压缩) ----------
class UnionFind {
    vector<int> parent_, rank_;
public:
    int count;                              // 当前连通分量个数
    UnionFind(int n) : parent_(n), rank_(n, 0), count(n) {
        iota(parent_.begin(), parent_.end(), 0);
    }
    int find(int x) {                       // 迭代版路径压缩:两趟
        int root = x;
        while (parent_[root] != root) root = parent_[root];
        while (parent_[x] != x) {           // 第二趟:沿途结点挂到根
            int nxt = parent_[x];
            parent_[x] = root;
            x = nxt;
        }
        return root;
    }
    bool unite(int a, int b) {
        int ra = find(a), rb = find(b);
        if (ra == rb) return false;
        if (rank_[ra] < rank_[rb]) swap(ra, rb);
        parent_[rb] = ra;                   // 浅树挂到深树
        if (rank_[ra] == rank_[rb]) ++rank_[ra];
        --count;                            // 成功合并,分量减 1
        return true;
    }
};

// ---------- 任务 4 对比用:朴素版(无任何优化) ----------
struct NaiveUF {
    vector<int> parent_;
    NaiveUF(int n) : parent_(n) {
        iota(parent_.begin(), parent_.end(), 0);
    }
    int find(int x) {
        while (parent_[x] != x) x = parent_[x];
        return x;
    }
    void unite(int a, int b) {
        a = find(a); b = find(b);
        if (a != b) parent_[a] = b;         // 固定挂到 b 下,易成长链
    }
};
// ---------- 任务 2/3:随机迷宫生成 ----------
struct Maze {
    int w, h;
    vector<vector<bool>> right, down;       // 右墙、下墙
    vector<pair<int, int>> record;          // 任务 3:连通分量变化采样
};

Maze genMaze(int w, int h, int seed = 2024) {
    UnionFind uf(w * h);
    Maze m{w, h,
           vector<vector<bool>>(h, vector<bool>(w, true)),
           vector<vector<bool>>(h, vector<bool>(w, true)),
           {}};
    vector<pair<int, int>> walls;           // 墙用两端格子编号表示
    auto id = [w](int r, int c) { return r * w + c; };
    for (int r = 0; r < h; ++r)
        for (int c = 0; c < w; ++c) {
            if (c + 1 < w) walls.push_back({id(r, c), id(r, c + 1)});   // 右墙
            if (r + 1 < h) walls.push_back({id(r, c), id(r + 1, c)});   // 下墙
        }
    mt19937 rng(seed);
    shuffle(walls.begin(), walls.end(), rng);
    int key[3] = {int(walls.size()) / 4, int(walls.size()) / 2,
                  int(walls.size()) * 3 / 4};
    int ki = 0;
    for (size_t i = 0; i < walls.size(); ++i) {
        int a = walls[i].first, b = walls[i].second;
        if (uf.unite(a, b)) {               // 两端不连通才拆墙
            int r1 = a / w, c1 = a % w, r2 = b / w, c2 = b % w;
            if (c1 == c2) m.down[min(r1, r2)][c1] = false;
            else          m.right[r1][min(c1, c2)] = false;
        }
        if (ki < 3 && int(i + 1) >= key[ki]) {
            m.record.push_back({int(i + 1), uf.count});
            ++ki;
        }
    }
    m.record.push_back({int(walls.size()), uf.count});
    return m;
}
// ---------- 任务 2:ASCII 渲染 ----------
string render(const Maze& m) {
    string top = "+";
    for (int c = 0; c < m.w; ++c) top += "--+";     // 顶部横线(永不拆)
    string s = top + "\n";
    for (int r = 0; r < m.h; ++r) {
        string row = "|";                           // 最左竖墙(永不拆)
        for (int c = 0; c < m.w; ++c)
            row += "  " + string(1, m.right[r][c] ? '|' : ' ');
        s += row + "\n";
        if (r == m.h - 1) {
            s += top + "\n";                        // 底部横线(永不拆)
        } else {
            string line = "+";
            for (int c = 0; c < m.w; ++c)
                line += string(m.down[r][c] ? "--" : "  ") + "+";
            s += line + "\n";
        }
    }
    return s;
}
// ---------- 任务 4:朴素版 vs 完整优化版性能对比 ----------
void bench(int n) {
    vector<pair<int, int>> ops;
    for (int i = 0; i < n - 1; ++i) ops.push_back({i, i + 1});  // 顺序合并成链
    auto ms = [](auto t0, auto t1) {
        return chrono::duration<double, milli>(t1 - t0).count();
    };
    long long s = 0;
    auto t0 = Clock::now();
    NaiveUF naive(n);
    for (auto [a, b] : ops) naive.unite(a, b);      // 每次 union 沿链走 O(n)
    auto t1 = Clock::now();
    for (int x = 0; x < n; ++x) s += naive.find(x); // 链尾 find 走 O(n)
    auto t2 = Clock::now();
    UnionFind opt(n);
    for (auto [a, b] : ops) opt.unite(a, b);
    auto t3 = Clock::now();
    for (int x = 0; x < n; ++x) s += opt.find(x);
    auto t4 = Clock::now();
    cout << "任务4: " << n << " 次 union(连成链)+ " << n
         << " 次 find(耗时对比):\n";
    cout << "  朴素版: union 阶段 " << ms(t0, t1) << " ms, find 阶段 "
         << ms(t1, t2) << " ms\n";
    cout << "  优化版: union 阶段 " << ms(t2, t3) << " ms, find 阶段 "
         << ms(t3, t4) << " ms\n";
    cout << "  find 阶段加速比:约 " << (t2 - t1) / (t4 - t3)
         << " 倍(检查值 s=" << s % 1000 << ")\n";
}

int main() {
    int w = 6, h = 4;
    Maze m = genMaze(w, h, 2024);
    cout << "随机迷宫 " << w << " 列 x " << h << " 行(种子 2024,共 "
         << w * h << " 个格子):\n";
    cout << render(m);
    int total = m.record.back().first;
    cout << "\n任务3: 拆墙过程中连通分量数量的变化(共 " << total
         << " 面候选墙):\n";
    cout << "  初始: " << w * h << " 个连通分量\n";
    for (auto [tried, comp] : m.record)
        cout << "  尝试 " << tried << "/" << total << " 面墙后: 连通分量 "
             << comp << "\n";
    cout << "  结束时连通分量为 1,迷宫全连通、无环(恰好拆 " << w * h - 1
         << " 面墙)。\n\n";
    bench(40000);
    return 0;
}
