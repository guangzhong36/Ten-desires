#include <iostream>
#include <vector>
using namespace std;

long long heapify(vector<int>& a) {
    int n = int(a.size());
    long long steps = 0;
    auto down = [&](int i) {
        while (true) {
            int l = 2 * i + 1, r = 2 * i + 2, m = i;
            if (l < n && a[l] > a[m]) m = l;
            if (r < n && a[r] > a[m]) m = r;
            if (m == i) break;
            swap(a[i], a[m]);
            ++steps;
            i = m;
        }
    };
    for (int i = n / 2 - 1; i >= 0; --i) down(i);
    return steps;
}

int main() {
    int n = 65535;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) a[i] = i + 1;   // 升序:对最大堆最不利
    long long s = heapify(a);
    cout << "n = " << n << ", 总下滤步数 = " << s
         << " (理论 O(n),实测 ≈ " << s * 100 / n << "% n)" << endl;
    return 0;
}
