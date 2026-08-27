// lab6.cpp —— 第一段:MinHeap(1 基)
#include <algorithm>
#include <cctype>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

// 任务 1:手写二叉堆(1 基,下标公式见 §6.2.3)
template <typename T>
class MinHeap {
    vector<T> a;                              // 元素从下标 1 开始

    void siftDown(size_t i) {                 // 下滤:与较小孩子交换
        size_t n = a.size();
        while (true) {
            size_t smallest = i;
            for (size_t c : {2 * i, 2 * i + 1})   // 左子、右子(1 基)
                if (c < n && a[c] < a[smallest])
                    smallest = c;
            if (smallest == i) break;
            swap(a[i], a[smallest]);
            i = smallest;
        }
    }

public:
    MinHeap() { a.push_back(T{}); }           // 占住 a[0]
    size_t size() const { return a.size() - 1; }
    bool empty() const { return size() == 0; }
    const T& top() const { return a[1]; }     // 根即最小元 O(1)

    void push(const T& x) {                   // 上滤:放末尾后向上冒泡
        a.push_back(x);
        size_t i = a.size() - 1;
        while (i > 1) {
            size_t p = i / 2;                 // 父结点(1 基)
            if (a[p] <= a[i]) break;
            swap(a[p], a[i]);
            i = p;
        }
    }

    void pop() {                              // 删除最小:末元素搬根再下滤
        a[1] = a.back();
        a.pop_back();
        if (a.size() > 1) siftDown(1);
    }

    static MinHeap buildFrom(const vector<T>& items) {  // 下滤建堆 O(n)
        MinHeap h;
        for (const T& x : items) h.a.push_back(x);
        for (size_t i = h.a.size() / 2; i >= 1; --i)    // 最后非叶 = n/2
            h.siftDown(i);
        return h;
    }
};
// 任务 2:流式切词并统计词频
void countWords(unordered_map<string, int>& freq) {
    string line;
    while (getline(cin, line)) {
        for (size_t i = 0; i < line.size();) {
            if (!isalpha((unsigned char)line[i])) { ++i; continue; }
            size_t j = i;
            while (j < line.size() && isalpha((unsigned char)line[j])) ++j;
            string w = line.substr(i, j - i);
            for (char& c : w) c = char(tolower((unsigned char)c));
            ++freq[w];
            i = j;
        }
    }
}

// 任务 3:手写小顶堆求 top-k
vector<pair<int, string>> topkHeap(const vector<pair<int, string>>& items, int k) {
    MinHeap<pair<int, string>> h;
    for (const auto& t : items) {            // t = (频次, 词)
        if (h.size() < size_t(k))
            h.push(t);                       // 堆未满:直接入堆
        else if (t > h.top()) {              // 高于"第 k 大门槛"才替换
            h.pop();
            h.push(t);
        }
    }
    vector<pair<int, string>> res;
    while (!h.empty()) { res.push_back(h.top()); h.pop(); }
    reverse(res.begin(), res.end());         // 弹出是升序,反转为降序
    return res;
}
// 任务 4:标准库重做(小顶堆 = 大顶堆 + greater)
vector<pair<int, string>> topkStd(const vector<pair<int, string>>& items, int k) {
    using P = pair<int, string>;
    priority_queue<P, vector<P>, greater<P>> pq;
    for (const P& t : items) {
        if (int(pq.size()) < k)
            pq.push(t);
        else if (t > pq.top()) {
            pq.pop();
            pq.push(t);
        }
    }
    vector<P> res;
    while (!pq.empty()) { res.push_back(pq.top()); pq.pop(); }
    reverse(res.begin(), res.end());
    return res;
}

int main(int argc, char* argv[]) {
    if (argc < 2) { cerr << "用法: ./lab6 k < 文本文件\n"; return 1; }
    int k = stoi(argv[1]);
    unordered_map<string, int> freq;
    countWords(freq);                        // 任务 2:词频字典
    vector<pair<int, string>> items;
    for (const auto& [w, c] : freq) items.push_back({c, w});

    // 任务 5:边界处理——k <= 0 或文本为空
    if (k <= 0 || items.empty()) {
        cout << "k = " << k << ", 词数 = " << items.size()
             << ":边界情形,答案为空。\n";
        return 0;
    }

    auto r1 = topkHeap(items, k);
    auto r2 = topkStd(items, k);
    cout << "词数 = " << items.size() << ", k = " << k << "\n";
    cout << "手写堆 : ";
    for (const auto& [c, w] : r1) cout << w << "(" << c << ") ";
    cout << "\npriority_queue: ";
    for (const auto& [c, w] : r2) cout << w << "(" << c << ") ";
    cout << "\n结果一致: " << (r1 == r2 ? "是" : "否") << "\n";
    return 0;
}
