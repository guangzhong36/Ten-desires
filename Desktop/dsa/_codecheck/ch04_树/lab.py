# lab4.py —— 单词频次统计器(BST 实现)(第一段:结点与核心操作)
import sys

class Node:                       # 单词 + 频次 + 左右子树
    def __init__(self, word, count=1):
        self.word, self.count = word, count
        self.left = self.right = None

class WordBST:
    def __init__(self):
        self.root = None

    def insert(self, w):
        self.root = self._insert(self.root, w)

    def _insert(self, n, w):
        if n is None:
            return Node(w)          # 空位:新建结点
        if w < n.word:
            n.left = self._insert(n.left, w)
        elif w > n.word:
            n.right = self._insert(n.right, w)
        else:
            n.count += 1            # 已存在:频次 +1
        return n

    def find(self, w):              # 查找频次,不存在返回 0
        n = self.root
        while n:
            if w == n.word:
                return n.count
            n = n.left if w < n.word else n.right
        return 0

    def remove(self, w):
        self.root = self._remove(self.root, w)

    def _find_min_node(self, n):    # 右子树最小结点
        while n.left:
            n = n.left
        return n

    def _remove(self, n, w):
        if n is None:
            return None             # 单词不存在,忽略
        if w < n.word:
            n.left = self._remove(n.left, w)
        elif w > n.word:
            n.right = self._remove(n.right, w)
        else:                       # 找到目标结点
            if n.left is None:
                return n.right      # 叶或只有右子树
            if n.right is None:
                return n.left       # 只有左子树
            m = self._find_min_node(n.right)   # 双子:取右子树最小
            n.word = m.word         # 复制数据顶替
            n.count = m.count
            n.right = self._remove(n.right, m.word)
        return n

# (第二段:中序、树高与平衡检查——类方法,复用示例 4-5 思路)
    def inorder(self):              # 中序 = 字典序
        self._inorder(self.root)

    def _inorder(self, n):
        if n is None:
            return
        self._inorder(n.left)
        print(n.word + '\t' + str(n.count))
        self._inorder(n.right)

    def height(self):               # 空树高度为 0
        return self._height(self.root)

    def _height(self, n):
        if n is None:
            return 0
        return 1 + max(self._height(n.left), self._height(n.right))

    def is_balanced(self):          # 自底向上检查
        _, ok = self._check(self.root)
        return ok

    def _check(self, n):            # 返回(高度, 是否平衡)
        if n is None:
            return 0, True
        hl, bl = self._check(n.left)
        hr, br = self._check(n.right)
        return 1 + max(hl, hr), bl and br and abs(hl - hr) <= 1

# (第三段:主程序——分词、统计、输出)
def main():
    print("单词频次统计器(BST 实现)——输入英文文本,空行结束:")
    lines = []
    for line in sys.stdin:
        if line.strip() == '':
            break
        lines.append(line)
    text = '\n'.join(lines)

    tree = WordBST()
    word = ''
    for ch in text:                 # 任务 1:分词转小写
        if ch.isalpha():
            word += ch.lower()
        elif word:
            tree.insert(word)       # 任务 2:插入统计
            word = ''
    if word:
        tree.insert(word)

    print("\n--- 任务 3:中序遍历输出词频表(字典序)---")
    tree.inorder()

    print("\n--- 任务 4:查找频次 ---")
    for w in ('the', 'dog', 'hello'):
        print("查找 '%s': 频次 = %d" % (w, tree.find(w)))

    print("\n删除 'dog' 后重新遍历:")
    tree.remove('dog')
    print("删除后 'dog' 频次 = %d(应为 0)" % tree.find('dog'))
    tree.inorder()

    print("\n--- 任务 5(加分):树高与平衡检查 ---")
    print("树高 = %d, 是否平衡 = %s" % (tree.height(),
          ('是' if tree.is_balanced() else '否')))

if __name__ == '__main__':
    main()
