// 手写倍增(左)与 std::vector(右)对照:打印每次扩容的拐点(size → capacity)
#include <iostream>
#include <vector>
using namespace std;

class MyVector {                       // 手写倍增动态数组
    int* data_ = nullptr;
    size_t size_ = 0, cap_ = 0;
public:
    void push_back(int x) {
        if (size_ == cap_) {
            size_t newCap = cap_ == 0 ? 1 : cap_ * 2;   // 倍增
            int* p = new int[newCap];
            for (size_t i = 0; i < size_; ++i) p[i] = data_[i];
            delete[] data_;
            data_ = p;
            cap_ = newCap;
            cout << size_ << "→" << cap_ << "  ";   // 打印拐点
        }
        data_[size_++] = x;
    }
    ~MyVector() { delete[] data_; }
};

int main() {
    cout << "手写倍增: ";
    MyVector v;
    for (int i = 0; i < 1024; ++i) v.push_back(i);
    cout << "\nstd::vector: ";
    vector<int> sv;                    // 标准库:同样的拐点观察
    size_t prev = 0;
    for (int i = 0; i < 1024; ++i) {
        sv.push_back(i);
        if (sv.capacity() != prev) {   // 容量变化 = 扩容拐点
            cout << sv.size() << "→" << sv.capacity() << "  ";
            prev = sv.capacity();
        }
    }
    cout << endl;
    return 0;
}
