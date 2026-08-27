// lab12.cpp —— 跳表字典的实现与对比(第一段:任务1 跳表核心)
#include <iostream>
#include <vector>
#include <random>
#include <map>
#include <algorithm>
#include <chrono>
using namespace std;
using Clock = chrono::steady_clock;

struct Node {
    int key;
    vector<Node*> next;
    Node(int k, int lvl) : key(k), next(lvl + 1, nullptr) {}
};

class SkipList {                     // 任务 1:三操作 + 随机层数
    Node* head_;
    int level_ = 0;
    mt19937 rng_{12345};             // 固定种子:可复现
public:
    SkipList(int maxLvl = 16) : head_(new Node(0, maxLvl)) {}
    ~SkipList() {
        Node* p = head_->next[0];
        while (p) { Node* t = p; p = p->next[0]; delete t; }
        delete head_;
    }
    int randomLevel() {              // 几何分布:每层 1/2 概率晋升
        int lvl = 1;
        while ((rng_() & 1) && lvl < int(head_->next.size()) - 1) ++lvl;
        return lvl;
    }
    bool find(int key) const {
        Node* cur = head_;
        for (int i = level_; i >= 0; --i)
            while (cur->next[i] && cur->next[i]->key < key)
                cur = cur->next[i];
        Node* x = cur->next[0];
        return x != nullptr && x->key == key;
    }
    void insert(int key) {
        if (find(key)) return;       // 字典语义:键唯一
        vector<Node*> update(head_->next.size());
        Node* cur = head_;
        for (int i = level_; i >= 0; --i) {
            while (cur->next[i] && cur->next[i]->key < key)
                cur = cur->next[i];
            update[i] = cur;
        }
        int lvl = randomLevel();
        if (lvl > level_) {
            for (int i = level_ + 1; i <= lvl; ++i) update[i] = head_;
            level_ = lvl;
        }
        Node* x = new Node(key, lvl);
        for (int i = 0; i <= lvl; ++i) {
            x->next[i] = update[i]->next[i];
            update[i]->next[i] = x;
        }
    }
    bool erase(int key) {
        vector<Node*> update(head_->next.size());
        Node* cur = head_;
        for (int i = level_; i >= 0; --i) {
            while (cur->next[i] && cur->next[i]->key < key)
                cur = cur->next[i];
            update[i] = cur;
        }
        Node* x = cur->next[0];
        if (!x || x->key != key) return false;
        for (int i = 0; i <= level_; ++i) {
            if (update[i]->next[i] != x) break;
            update[i]->next[i] = x->next[i];
        }
        delete x;
        while (level_ > 0 && head_->next[level_] == nullptr) --level_;
        return true;
    }
    vector<int> keys() const {
        vector<int> v;
        for (Node* p = head_->next[0]; p; p = p->next[0]) v.push_back(p->key);
        return v;
    }
    size_t size() const { return keys().size(); }
};

class SkipDict {                 // 任务 2:有序字典接口
    SkipList sl_;
public:
    void insert(int k)  { sl_.insert(k); }
    bool find(int k)    { return sl_.find(k); }
    bool erase(int k)   { return sl_.erase(k); }
    vector<int> keys()  { return sl_.keys(); }
    size_t size() const { return sl_.size(); }
};
int main() {
    // 任务 3:与 std::map 对照,输出必须一致
    cout << "== 任务3:跳表字典 vs std::map 一致性 ==" << endl;
    SkipDict dict;
    map<int, int> m;
    mt19937 rng(12345);
    uniform_int_distribution<int> dist(0, 99999);
    for (int i = 0; i < 20000; ++i) {
        int k = dist(rng);
        int op = i % 4;                 // 0/1 插,2 删,3 查
        if (op <= 1) {
            if (!m.count(k)) dict.insert(k);
            m[k] = k;
        } else if (op == 2) {
            dict.erase(k);
            m.erase(k);
        } else {
            bool a = dict.find(k), b = m.count(k) > 0;
            if (a != b) { cout << "不一致! key=" << k << endl; return 1; }
        }
    }
    vector<int> dk = dict.keys(), mk;
    for (auto& p : m) mk.push_back(p.first);
    cout << "20000 次随机操作后:跳表 size=" << dict.size()
         << ",map size=" << m.size() << endl;
    cout << "跳表前 10 键: ";
    for (int i = 0; i < 10; ++i) cout << dk[i] << ' ';
    cout << "\n按序遍历顺序一致: " << (dk == mk ? "是" : "否") << endl;
    // 任务 4:10^4 随机键计时对比
    const int N = 10000;
    vector<int> ks(N);
    for (int i = 0; i < N; ++i) ks[i] = i;
    shuffle(ks.begin(), ks.end(), rng);
    SkipDict s2;
    auto t0 = Clock::now();
    for (int k : ks) s2.insert(k);
    double slIns = chrono::duration<double, milli>(Clock::now() - t0).count();
    map<int, int> m2;
    t0 = Clock::now();
    for (int k : ks) m2[k] = k;
    double mIns = chrono::duration<double, milli>(Clock::now() - t0).count();
    long long hits = 0;
    t0 = Clock::now();
    for (int k : ks) hits += s2.find(k);
    double slFind = chrono::duration<double, milli>(Clock::now() - t0).count();
    t0 = Clock::now();
    for (int k : ks) hits += m2.count(k);
    double mFind = chrono::duration<double, milli>(Clock::now() - t0).count();
    cout << "\n== 任务4:10^4 随机键计时 ==" << endl;
    cout << "插入 " << N << " 键:跳表 " << slIns << " ms,map " << mIns << " ms" << endl;
    cout << "查找 " << N << " 键:跳表 " << slFind << " ms,map " << mFind << " ms" << endl;
    cout << "(命中数 " << hits << ",防止死代码消除)" << endl;

    // 任务 5:删除全部元素
    for (int k : ks) s2.erase(k);
    cout << "\n== 任务5:删除全部后 ==" << endl;
    cout << "跳表 size=" << s2.size() << ",为空=" << (s2.size() == 0 ? "是" : "否")
         << ",find(0)=" << s2.find(0) << endl;
    return 0;
}
