// lab5.cpp —— 散列表单词统计对比实验(第一部分:工具函数)
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <unordered_map>
#include <chrono>
#include <cctype>
using namespace std;
using Clock = chrono::steady_clock;

// ---------- 工具:素数判断与下一个素数 ----------
bool isPrime(int n) {
    if (n < 2) return false;
    for (int d = 2; d * d <= n; ++d)
        if (n % d == 0) return false;
    return true;
}
int nextPrime(int n) {
    while (!isPrime(n)) ++n;
    return n;
}

// 任务 5:切分单词——字母/数字连续成词并转小写,其余字符跳过
vector<string> splitWords(const string& text) {
    vector<string> words;
    string cur;
    for (char c : text) {
        if (isalnum((unsigned char)c)) {
            cur += (char)tolower((unsigned char)c);
        } else if (!cur.empty()) {
            words.push_back(cur);
            cur.clear();
        }
    }
    if (!cur.empty()) words.push_back(cur);
    return words;
}
// ---------- 任务 1:分离链接散列表(字符串键,37 倍散列,素数表) ----------
class HashTable {
    struct Entry { string key; int cnt; };
    vector<list<Entry>> buckets_;   // 桶数组:vector<list<...>>
    int size_ = 0;                  // 不同单词个数
public:
    explicit HashTable(int m) : buckets_(nextPrime(m)) {}

    int hash(const string& s) const {
        unsigned long long h = 0;   // 全程不取模,最后再 % 表大小(见 §5.1.3)
        for (char c : s)
            h = h * 37 + (unsigned char)c;
        return int(h % buckets_.size());
    }

    void add(const string& s) {            // 计数 +1,首次出现则插入
        int b = hash(s);
        for (auto& e : buckets_[b])
            if (e.key == s) { ++e.cnt; return; }
        buckets_[b].push_back({s, 1});
        ++size_;
    }

    int count(const string& s) const {     // 查询词频,不存在返回 0
        int b = hash(s);
        for (const auto& e : buckets_[b])
            if (e.key == s) return e.cnt;
        return 0;
    }

    int distinct() const { return size_; }
    int bucketCount() const { return int(buckets_.size()); }
    double loadFactor() const {            // 装填因子 λ = 元素数/表大小
        return double(size_) / buckets_.size();
    }
};
// ---------- 计时工具:插入 repeat 遍,放大差异 ----------
double msInsert(HashTable& ht, const vector<string>& words, int repeat) {
    auto t0 = Clock::now();
    for (int r = 0; r < repeat; ++r)
        for (const string& w : words) ht.add(w);
    return chrono::duration<double, milli>(Clock::now() - t0).count();
}

int main(int argc, char* argv[]) {
    cerr << "DBG1 argc=" << argc << "
";
    if (argc < 2) { cout << "用法: lab5 <文本文件>\n"; return 1; }
    ifstream fin(argv[1]);
    if (!fin) { cout << "错误: 无法打开文件 " << argv[1] << "\n"; return 1; }
    stringstream ss;
    ss << fin.rdbuf();
    cerr << "DBG2 before splitWords
";
    vector<string> words = splitWords(ss.str());
    cerr << "DBG3 words=" << words.size() << "
";
    if (words.empty()) {                  // 任务 5:空文本
        cout << "警告: 文本为空或没有合法单词\n";
        return 0;
    }
    cout << "单词总数(含重复): " << words.size() << "\n";

    // 任务 2:手写表统计
    cerr << "DBG4 before HashTable
";
    HashTable ht(256);
    cerr << "DBG5 after HashTable
";
    cerr << "DBG6 before msInsert
";
    msInsert(ht, words, 1);
    cerr << "DBG7 after msInsert
";
    cout << "手写表: 不同单词 " << ht.distinct()
         << " 个,装填因子 λ=" << ht.loadFactor() << "\n";

    // 任务 3:标准库重做,抽查输出一致性
    unordered_map<string, int> mp;
    for (const string& w : words) ++mp[w];
    cout << "unordered_map: 不同单词 " << mp.size() << " 个\n";
    for (const char* w : {"the", "hash", "hello"})
        cout << "  词 \"" << w << "\": 手写表=" << ht.count(w)
             << " map=" << mp[w] << "\n";

    // 任务 4:不同 λ 下的插入耗时(表大小不同 → λ 不同,3 次取最快)
    cout << "任务4: 表大小(素数)不同 → λ 不同 → 插入耗时(3 次取最快):\n";
    for (int m : {8, 16, 64, 256}) {
        double best = 1e300;
        for (int trial = 0; trial < 3; ++trial) {   // 重复 3 次取最快
            HashTable t(m);
            double ms = msInsert(t, words, 30);
            if (ms < best) best = ms;
        }
        HashTable t(m);
        msInsert(t, words, 1);
        cout << "  M=" << t.bucketCount() << " λ=" << t.loadFactor()
             << " 耗时 " << best << " ms\n";
    }
    cout << "观察: λ 越大平均链表越长,耗时越高——这就是再散列(§5.4)的动机。\n";
    return 0;
}
