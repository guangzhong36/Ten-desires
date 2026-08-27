// lab4.cpp —— 单词频次统计器(BST 实现)(第一段:结点与核心操作)
#include <iostream>
#include <string>
#include <memory>
#include <cctype>
#include <algorithm>
using namespace std;
struct BSTNode {                      // 单词 + 频次 + 左右子树
    string word;
    int count;
    unique_ptr<BSTNode> left, right;
    explicit BSTNode(const string& w) : word(w), count(1) {}
};

class WordBST {
public:
    void insert(const string& w) { root_ = insert(move(root_), w); }
    int find(const string& w) const {          // 查找频次,不存在返回 0
        BSTNode* n = root_.get();
        while (n) {
            if (w == n->word) return n->count;
            n = (w < n->word) ? n->left.get() : n->right.get();
        }
        return 0;
    }
    void remove(const string& w) { root_ = remove(move(root_), w); }
    BSTNode* root() const { return root_.get(); }   // 供第二段辅助函数
private:
    unique_ptr<BSTNode> root_;
    static unique_ptr<BSTNode> insert(unique_ptr<BSTNode> n, const string& w) {
        if (!n) return make_unique<BSTNode>(w);  // 空位:新建结点
        if (w < n->word)      n->left  = insert(move(n->left), w);
        else if (w > n->word) n->right = insert(move(n->right), w);
        else                  ++n->count;        // 已存在:频次 +1
        return n;
    }
    static BSTNode* findMin(BSTNode* n) {        // 右子树最小结点
        while (n->left) n = n->left.get();
        return n;
    }
    static unique_ptr<BSTNode> removeMin(unique_ptr<BSTNode> n) {
        if (!n->left) return move(n->right);
        n->left = removeMin(move(n->left));
        return n;
    }
    static unique_ptr<BSTNode> remove(unique_ptr<BSTNode> n, const string& w) {
        if (!n) return nullptr;                   // 单词不存在,忽略
        if (w < n->word)      n->left  = remove(move(n->left), w);
        else if (w > n->word) n->right = remove(move(n->right), w);
        else {                                    // 找到目标结点
            if (!n->left)  return move(n->right); // 叶或只有右子树
            if (!n->right) return move(n->left);  // 只有左子树
            BSTNode* m = findMin(n->right.get()); // 双子:取右子树最小
            n->word  = m->word;                   // 复制数据顶替
            n->count = m->count;
            n->right = removeMin(move(n->right));
        }
        return n;
    }
};
// (第二段:中序遍历、树高与平衡检查——辅助函数,配合 tree.root())
#include <utility>
void inorder(BSTNode* n) {            // 中序 = 字典序
    if (!n) return;
    inorder(n->left.get());
    cout << n->word << '\t' << n->count << '\n';
    inorder(n->right.get());
}
int height(BSTNode* n) {              // 空树高度为 0
    if (!n) return 0;
    return 1 + max(height(n->left.get()), height(n->right.get()));
}
pair<int, bool> check(BSTNode* n) {   // 返回(高度, 是否平衡)
    if (!n) return {0, true};
    auto [hl, bl] = check(n->left.get());
    auto [hr, br] = check(n->right.get());
    return {1 + max(hl, hr), bl && br && abs(hl - hr) <= 1};
}
int main() {
    cout << "单词频次统计器(BST 实现)——输入英文文本,空行结束:\n";
    string line, text;
    while (getline(cin, line)) {
        if (line.empty()) break;
        text += line + '\n';
    }

    WordBST tree;
    string word;                                  // 任务 1:分词转小写
    for (char c : text) {
        if (isalpha((unsigned char)c))
            word += (char)tolower((unsigned char)c);
        else if (!word.empty()) {
            tree.insert(word);                    // 任务 2:插入统计
            word.clear();
        }
    }
    if (!word.empty()) tree.insert(word);

    cout << "\n--- 任务 3:中序遍历输出词频表(字典序)---\n";
    inorder(tree.root());

    cout << "\n--- 任务 4:查找频次 ---\n";
    for (const string& w : {"the", "dog", "hello"})
        cout << "查找 '" << w << "': 频次 = " << tree.find(w) << '\n';

    cout << "\n删除 'dog' 后重新遍历:\n";
    tree.remove("dog");
    cout << "删除后 'dog' 频次 = " << tree.find("dog") << "(应为 0)\n";
    inorder(tree.root());

    cout << "\n--- 任务 5(加分):树高与平衡检查 ---\n";
    cout << "树高 = " << height(tree.root())
         << ", 是否平衡 = " << (check(tree.root()).second ? "是" : "否") << '\n';
    return 0;
}