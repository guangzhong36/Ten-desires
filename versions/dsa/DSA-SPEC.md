# 《数据结构与算法分析》教辅 · 文档编写规范(DSA-SPEC)

> 本规范供 DSA 教辅两种语言版(C++ / Python)的编写者统一遵守。
> 基准教材:Mark Allen Weiss《数据结构与算法分析》(C++ 第4版 / Python 语言描述)。
> 目标读者:**数据结构与算法零基础、已掌握 C 语言**的学生。全书目标:逐章配套教材,系统、详细,学完后能独立实现各章数据结构并用标准库解决工程问题。

## 一、项目目标与产出

- 每种语言产出一册 PDF:《DSA_<Lang>_教辅.pdf》,共 12 章(章节名两版完全一致)+ 封面。
- 每章末尾必须有一个**章末实验题**,参考答案为完整可编译/可运行程序。
- 每章必须有 **"从 C 到 X" 对照**(读者会 C,对照是最高效的学习路径),对照表用 `table class="cmp"`(两列:C 的写法 | 本语言的写法)。
- 每章必须有**复杂度速查表**(4 列:操作 | 数据结构/算法 | 平均 | 最坏)。
- 产出一份章节级 PDF 便于单章打印,最终合并为一册(≥110 页)。

## 二、目录结构

```
dsa/<lang>/
├── src/      # 源 HTML:cover.html + ch01_绪论.html … ch12_高级数据结构.html(编辑这里)
├── build/    # 编译后的自包含 HTML(中间产物)
└── pdf/      # 章节 PDF + 00_封面.pdf + 最终合并 DSA_<Lang>_教辅.pdf
```

章文件名(两版一致,无空格无半角括号):

| 章 | 文件名 |
|---|---|
| 1 绪论 | `ch01_绪论` |
| 2 算法分析 | `ch02_算法分析` |
| 3 表栈和队列 | `ch03_表栈和队列` |
| 4 树 | `ch04_树` |
| 5 散列 | `ch05_散列` |
| 6 优先队列(堆) | `ch06_优先队列-堆` |
| 7 排序 | `ch07_排序` |
| 8 不相交集 | `ch08_不相交集` |
| 9 图论算法 | `ch09_图论算法` |
| 10 算法设计技巧 | `ch10_算法设计技巧` |
| 11 摊还分析 | `ch11_摊还分析` |
| 12 高级数据结构 | `ch12_高级数据结构` |

kicker 与页脚:

| 语言 | kicker | 页脚文字 |
|---|---|---|
| C++ | `FROM C TO C++ · DATA STRUCTURES` | `数据结构与算法分析(Weiss)教辅 · 第N章 章名 · C++版` |
| Python | `FROM C TO PYTHON · DATA STRUCTURES` | `数据结构与算法分析(Weiss)教辅 · 第N章 章名 · Python版` |

## 三、构建命令(在仓库根目录 C:\Users\zz666\signals_and_systems_tutor 下执行)

```bash
node tools/build.mjs dsa/<lang>/src/chNN_章名.html dsa/<lang>/build/chNN.html
node tools/render.mjs dsa/<lang>/build/chNN.html dsa/<lang>/pdf/chNN_章名.pdf "<页脚文字>"
node tools/build.mjs dsa/<lang>/src/cover.html dsa/<lang>/build/cover.html
node tools/render.mjs dsa/<lang>/build/cover.html dsa/<lang>/pdf/00_封面.pdf cover
python tools/verify.py dsa/<lang>/pdf/chNN_章名.pdf
python dsa/qa_metrics.py dsa/<lang>/src/chNN_章名.html
python tools/merge.py dsa/<lang>/pdf/DSA_<Lang>_教辅.pdf dsa/<lang>/pdf/00_封面.pdf dsa/<lang>/pdf/ch01_*.pdf …(按 ch01…ch12 顺序全部列出)
```

## 四、HTML 编写规范

1. 文件第 1 行:`<title>第N章 章节名</title>`;第 2 行:`<!-- no-katex -->`(必须,跳过公式渲染,防止代码里的 `\(` `\[` 被误判)。
2. **禁止使用 KaTeX**;数学记号一律按本规范第五节写法。
3. **禁止使用 `<div data-svg=…>`**;知识导图用文字树(`toc-map`)。
4. 不写 `<html>/<head>/<body>` 骨架,build 会组装。
5. **代码块**:`<pre class="code">…</pre>`。代码内必须转义:`&`→`&amp;`、`<`→`&lt;`、`>`→`&gt;`。代码块前必须加标题:`<div class="code-title">示例 N-M 标题</div>`。行内代码用 `<code>x</code>`(转义规则同上)。
6. 提示框:`<div class="box box-xxx"><div class="box-title">标题</div>…</div>`。可用类名及用途:
   - `box-goal` 学习目标(章首,必用)| `box-intuition` 从 C 到 X 迁移(章首,必用)| `box-prereq` 前置知识(章首,必用)
   - `box-keypoint` 重点结论 | `box-example` 例题/示例 | `box-pitfall` 易错点/误区 | `box-tip` 实现建议/调试技巧/记号说明
   - `box-summary` 小结/速查 | `box-answer` 答案
7. 术语用 `<span class="term">…</span>`,强调用 `<span class="hl">…</span>`。
8. 标题用 `<h2>`(节)与 `<h3>`(小节)。**h2 一律不加 class="sec"**(每章独立成 PDF,无需内部分页)。
9. 表格:两列 C 对照表用 `<table class="cmp">`;普通表格 `<table>` 或 `<table class="tight">`(紧凑小字)。禁止其他自定义 class 与一切内联 style。
10. 代码注释用中文;代码必须语法正确、符合该语言主流风格(读者已会 C,尽量与 C 对照);宁可示例短而精,不要长而错。
11. 写作语言:全书中文;代码标识符用英文。语气正式、面向教学,多举生产场景与调试技巧。
12. 文件一律 UTF-8 编码。

## 五、数学记号规范(no-katex,全系列统一)

- 大 O 等记号:`<i>O</i>(<i>n</i>)`、`<i>O</i>(<i>n</i> log <i>n</i>)`、`<i>O</i>(<i>n</i><sup>2</sup>)`、`<i>Ω</i>(<i>n</i>)`、`<i>Θ</i>(<i>n</i> log <i>n</i>)`、`<i>o</i>(<i>n</i>)`。
- 幂与下标:`n<sup>2</sup>`、`2<sup>n</sup>`、`log<sub>2</sub> n`、`T<sub>avg</sub>(<i>n</i>)`。对数首现时给定义与约定(log<sub>2</sub>、ln、lg)。
- 递推式(行内):`<code>T(n) = 2T(n/2) + n</code>`;分式一律用斜线 `n/2`,不造分数样式。
- 求和:`Σ<sub>i=1</sub><sup>n</sup> i = n(n+1)/2`(Σ 直接输入希腊字符)。
- 模运算:正文 `<i>a</i> mod <i>b</i>`;代码语境一律 `a % b`。
- 指数:`e<sup>n</sup>`;近似:`≈`。
- **数学记号与代码记号分离**:正文用 `<i>/<sub>/<sup>` 记号,代码里只用语言语法(`%`、`//`、`pow`、`**` 等)。
- 每章首次出现较复杂公式处放一个 box-tip"记号说明"。

## 六、每章统一结构(严格按此顺序)

```html
<title>第N章 章节名</title>
<!-- no-katex -->
<div class="kicker">FROM C TO <LANG> · DATA STRUCTURES</div>
<h1 class="chapter-title">第N章 章节名</h1>
<div class="chapter-meta"><b>建议学时:</b>3–5 小时 | <b>难度:</b>★★★☆☆ | <b>读者背景:</b>已掌握 C 语言,DS&amp;A 零基础</div>

<div class="box box-goal"><div class="box-title">本章学习目标</div><ul>…5–7 条,至少 1 条"C 中怎么做→本语言怎么做",至少 1 条"学完能独立实现/调用什么"…</ul></div>
<div class="box box-intuition"><div class="box-title">从 C 到 <LANG>:本章主题如何迁移</div><p>…3–6 段:先讲算法直觉(为什么需要这个数据结构/结论),再讲 C 的做法与痛点、本语言的解法…</p></div>
<div class="box box-prereq"><div class="box-title">前置知识自查</div><p><b>数学前置:</b>…本章用到的数学…</p><p><b>C 语言前置:</b>…需要的 C 知识清单…</p><p><b>依赖的前面章节:</b>…(第4章起必写,注明依赖哪个知识点)…</p></div>
<div class="box box-tip"><div class="box-title">本章学习路线</div><ol><li>第一阶段…<li>第二阶段…<li>第三阶段(实验)…<li>验收标准…</ol></div>

<h2>本章知识导图</h2>
<div class="toc-map"><ul>…用嵌套 ul 文字树列出全部小节…</ul></div>

<h2>§N.1 …</h2> …(分节详解:知识点 → 代码 → 例题/误区,每节至少一轮)
<h2>§N.2 …</h2> …
…(6–8 节,每节含 2–4 个 h3;每章至少 1 处"手写实现 + 标准库写法"并排展示)

<h2>本章复杂度速查表</h2>
<table>…4 列"操作 | 数据结构/算法 | 平均 | 最坏",≥15 行…</table>

<h2>最小必会清单</h2>
<ul>…8–12 条,口吻:"能独立说出/写出/实现…"…</ul>

<h2>自测题</h2>
<div class="q">1. …</div><div class="box box-answer"><div class="box-title">参考答案</div>…</div>
…≥5 题…

<h2>章末实验题:…(综合实验名)</h2>
<div class="box box-example lab"><div class="box-title">实验 N:实验名</div>
<p><b>实验目标:</b>…一句话…</p>
<p><b>实验要求:</b></p><ul><li>任务 1(对应本章哪些知识点)…<li>任务 2…<li>…(逐条覆盖全章知识点,用括号标注覆盖的知识点)…</ul>
<p><b>实验环境:</b>…编译/运行命令…</p></div>
<ol class="steps"><li>…实现提示 1…<li>…实现提示 2…<li>…实现提示 3…</ol>
<div class="box box-answer"><div class="box-title">参考答案(完整可运行程序)</div>
<p>…关键代码 ≥60 行(可用多个 pre.code),必须是完整可编译/可运行的程序…</p></div>

<hr class="thin">
<p class="small">下一章预告:…(必须写明与后续章节的依赖关系)…</p>
```

### 章末实验题硬性要求(超出 langdocs SPEC 的部分)

- 参考答案必须是**完整程序**(C++ 含 main,Python 含 `if __name__ == "__main__"` 入口),≥60 行;
- 附**编译/运行命令**(C++:`g++ -std=c++17 xxx.cpp -o xxx && ./xxx`;Python:`python xxx.py`);
- 附 **1–2 组输入输出样例**;
- 结尾必须有 `<p><b>设计说明:</b>…</p>` 综述设计取舍与知识点覆盖。

## 七、质量硬指标(每章必须达标)

| 指标 | 要求 |
|---|---|
| 章源 HTML 大小 | 18–42 KB(纯内容,不含样式;DS&A 章含 60–110 行实验参考答案,上限高于 langdocs 的 32 KB) |
| 正文汉字 | ≥ 3500 字(不含代码) |
| 代码块 | ≥ 8 个 `<pre class="code">`(每个 8–60 行) |
| 易错点 | ≥ 3 个 `box-pitfall` |
| 与 C 的显式对照 | ≥ 3 处(`class="cmp"` 两列表格:"C 的写法 | 本语言的写法") |
| 复杂度速查表 | ≥ 15 行(4 列) |
| 自测题 | ≥ 5 道(带答案) |
| 章末实验题 | 覆盖全章所有知识点(逐条标注),参考答案完整程序 ≥ 60 行 |
| 实现建议/调试技巧 | ≥ 2 处(box-tip 或正文) |
| 章节 PDF | 8–18 页(金标准章实测 16–18 页,宁多勿少);verify.py 无横向溢出、无空页 |
| 全书合并 | ≥ 110 页 |

## 八、术语表(两版必须同词,全系列统一)

- 抽象数据类型 ADT、表(list)、栈(stack)、队列(queue)
- 树:根、父结点、子结点、兄弟、叶、深度、高度、路径;先序/中序/后序/层序遍历
- 二叉查找树 BST、平衡树、AVL 树、伸展树(splay tree)、红黑树、B 树
- 散列:散列函数、装填因子 λ、分离链接法、开放定址法、线性探测、平方探测、双散列、再散列、可扩散列
- 堆:堆序性质、结构性质、完全二叉树、上滤、下滤、d-堆、左式堆、斜堆、二项队列
- 排序:稳定性、枢轴、三数中值分割法、增量序列、外部排序、比较下界
- 不相交集:等价关系、按大小合并、按秩合并、路径压缩
- 图:顶点、边、邻接矩阵、邻接表、拓扑排序、最短路径、最小生成树、割点、双连通
- 算法设计:贪心、分治、动态规划、记忆化、回溯、随机化
- 摊还分析:聚集分析、会计方法、势能方法、摊还代价
- 复杂度记号:大 O、Ω、Θ、小 o;常数、对数、线性、线性对数、平方、立方、指数

## 九、复杂度口径(速查表必须与本表一致)

- 动态数组(C++ vector / Python list):按索引访问 O(1);尾部插入平均 O(1),最坏 O(n)(扩容);头部插入/中间插入 O(n);按值查找 O(n);删除尾部 O(1)
- 链表:查找 O(n);已定位处插入/删除 O(1);按索引访问 O(n)
- 栈/队列(数组或链表实现):push/pop/入队/出队/取顶 O(1)
- BST:查找/插入/删除平均 O(log n),最坏 O(n)
- AVL 树:查找/插入/删除 O(log n)
- 散列表(分离链接):查找/插入/删除平均 O(1),最坏 O(n);开放定址同
- 二叉堆:insert O(log n);deleteMin/deleteMax O(log n);buildHeap O(n);decreaseKey O(log n);merge O(n)
- 排序:插入 O(n²) 稳定;希尔近似 O(n<sup>3/2</sup>) 不稳定;堆排 O(n log n) 不稳定;归并 O(n log n) 稳定、O(n) 额外空间;快排平均 O(n log n) 最坏 O(n²) 不稳定;桶排 O(n)
- 不相交集:union/find 均摊近 O(1)(按秩合并+路径压缩);无优化最坏 O(n)
- 图:DFS/BFS O(V+E);拓扑排序 O(V+E);Dijkstra 朴素 O(V²)、堆优化 O(E log V);Bellman-Ford O(VE);Kruskal O(E log E);Prim 朴素 O(V²)、堆优化 O(E log V);最大流(Edmonds-Karp)O(VE²)

## 十、语言差异点(两版刻意写不同内容的位置)

| 章 | C++ 版要点 | Python 版要点 |
|---|---|---|
| 1 | 负数取模截断;int 溢出;栈深崩溃 | 取模向下取整;任意精度大整数;RecursionError 与 setrecursionlimit |
| 3 | 迭代器失效;std::list 与 vector 选择;RAII | 几乎不手写链表;list 即动态数组;切片 |
| 4 | 指针/unique_ptr 建树与内存管理 | 对象引用建树;递归深度限制 |
| 5 | unordered_map/unordered_set 与自定义哈希 | dict 可哈希要求、3.7+ 插入有序、set 推导 |
| 6 | priority_queue 默认大顶堆,最小堆用 greater;1 基数组习惯 | heapq 默认小顶堆,大顶堆用负号;0 基数组换算 |
| 7 | C qsort(函数指针+void*)→ std::sort(模板+lambda 比较器) | sorted(key=) 与 functools.cmp_to_key |
| 8 | 递归与迭代 find 都讲 | 深链递归 find 的 RecursionError 风险,建议迭代版 |
| 9 | INF 用 INT_MAX,注意加法溢出坑 | INF 用 float('inf'),无溢出 |
| 10 | lambda/std::function 递归;<random> 代替 rand() | 闭包/生成器(yield 版全排列);random 模块 |
| 12 | STL map/set 即红黑树;unordered 系列选择 | 无内置平衡树;bisect/排序/sortedcontainers 替代惯例 |

## 十一、QA 与报告要求

1. **边写边验**:每写完 1 章即执行 build + render + verify + qa_metrics;build 报错或 verify 报横向溢出/空页/页数出界,必须修复后才提交。
2. 每章 verify 输出应显示页数在 8–18 页之间;不足补内容(优先补例题与误区),超过可接受(宁多勿少)。
3. **代码真实验证**:Python 版实验参考答案与关键完整程序必须实际运行通过;C++ 版必须用本机 MSVC 编译运行通过(片段示例允许静态审查,但语法必须正确)。
4. 两版一致性:节标题、复杂度表数值、术语必须一致(见第八、九节)。
5. 不修改 dsa/ 之外的任何文件;不提交 git。
6. **最终报告**(最后一条消息)必须包含:
   - 各章页数清单(ch01–ch12)、封面页数、合并总页数(两册)
   - 最终 PDF 完整路径与文件大小
   - 横向溢出/空页检查结果
   - 代码编译/运行验证结果汇总
   - 与规范的任何偏离(如实说明)
