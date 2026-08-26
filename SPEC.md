# 编程语言使用大全 · 文档编写规范(SPEC)

> 本规范供 6 种语言(C++ / Python / Java / JavaScript / Rust / Go)手册的编写者统一遵守。
> 目标读者:**有 C 语言基础的学习者**。全书目标:系统、详细,读完后在生产实践中不再有任何语法困惑。

## 一、项目目标与产出

- 每种语言产出一份 PDF:《<Lang>_使用大全.pdf》,共 14 章(章节名六种语言完全一致)+ 封面。
- 每章末尾必须有一个**章末实验题**,复习全章所有知识点。
- 每章必须有 **"从 C 到 X" 对照**(读者会 C,对照是最高效的学习路径)。
- 产出一份章节级 PDF 便于单章打印(如 ch01_数据类型与变量.pdf),最终合并为一册。

## 二、目录结构(以 cpp 为例,其余语言同)

```
langdocs/cpp/
├── src/      # 源 HTML:cover.html + ch01.html … ch14.html(编辑这里)
├── build/    # 编译后的自包含 HTML(中间产物)
└── pdf/      # 章节 PDF + 00_封面.pdf + 最终合并 Cpp_使用大全.pdf
```

语言目录名与最终文件名:

| 语言 | 目录 | 封面 kicker 英文 | 最终 PDF |
|---|---|---|---|
| C++ | `langdocs/cpp/` | FROM C TO C++ | `Cpp_使用大全.pdf` |
| Python | `langdocs/python/` | FROM C TO PYTHON | `Python_使用大全.pdf` |
| Java | `langdocs/java/` | FROM C TO JAVA | `Java_使用大全.pdf` |
| JavaScript | `langdocs/javascript/` | FROM C TO JAVASCRIPT | `JavaScript_使用大全.pdf` |
| Rust | `langdocs/rust/` | FROM C TO RUST | `Rust_使用大全.pdf` |
| Go | `langdocs/go/` | FROM C TO GO | `Go_使用大全.pdf` |

章节文件名(六种语言一致):`ch01_数据类型与变量` `ch02_运算符与表达式` `ch03_流程控制` `ch04_函数与方法` `ch05_内存管理` `ch06_面向对象` `ch07_文件IO` `ch08_网络通信` `ch09_并发与异常` `ch10_泛型与元编程` `ch11_反射与动态特性` `ch12_注解与装饰器` `ch13_模块系统` `ch14_类型系统进阶`

## 三、构建命令(在仓库根目录 C:\Users\zz666\signals_and_systems_tutor 下执行)

```bash
# 编译某章(KaTeX/SVG/样式注入;源含 <!-- no-katex --> 时跳过公式渲染)
node tools/build.mjs langdocs/<lang>/src/ch01_数据类型与变量.html langdocs/<lang>/build/ch01.html
# 渲染为 A4 PDF,第 3 参是页脚文字
node tools/render.mjs langdocs/<lang>/build/ch01.html langdocs/<lang>/pdf/ch01_数据类型与变量.pdf "<语言名> 使用大全 · 第1章 数据类型与变量"
# 封面渲染(第 3 参 cover 表示无边距)
node tools/build.mjs langdocs/<lang>/src/cover.html langdocs/<lang>/build/cover.html
node tools/render.mjs langdocs/<lang>/build/cover.html langdocs/<lang>/pdf/00_封面.pdf cover
# 单章校验(页数/横向溢出/空页)
python tools/verify.py langdocs/<lang>/pdf/ch01_数据类型与变量.pdf
# 合并(封面 + 14 章按序)
python tools/merge.py langdocs/<lang>/pdf/<Lang>_使用大全.pdf langdocs/<lang>/pdf/00_封面.pdf langdocs/<lang>/pdf/ch01_*.pdf …(按 ch01…ch14 顺序全部列出)
```

## 四、HTML 编写规范

1. 文件第 1 行:`<title>第N章 章节名</title>`;第 2 行:`<!-- no-katex -->`(必须,跳过公式渲染,防止代码里的 `\(` `\[` 被误判)。
2. **禁止使用 KaTeX**;复杂度等记号用普通文本,如 `<i>O</i>(<i>n</i> log <i>n</i>)`。
3. **禁止使用 `<div data-svg=…>`**;知识导图用文字树(见下)。
4. 不写 `<html>/<head>/<body>` 骨架,build 会组装。
5. **代码块**:`<pre class="code">…</pre>`。代码内必须转义:`&`→`&amp;`、`<`→`&lt;`、`>`→`&gt;`。代码块前可加标题:`<div class="code-title">示例 2-3 位运算</div>`。
6. 行内代码用 `<code>x</code>`(转义规则同上)。
7. 提示框:`<div class="box box-xxx"><div class="box-title">标题</div>…</div>`。可用类名及用途:
   - `box-goal` 学习目标(章首,必用)| `box-prereq` 前置知识 | `box-intuition` 从 C 到 X 对照(章首,必用)
   - `box-keypoint` 重点结论 | `box-example` 例题/示例 | `box-pitfall` 易错点/误区 | `box-tip` 工程建议/技巧
   - `box-summary` 小结/速查 | `box-answer` 答案
8. 术语用 `<span class="term">…</span>`,强调用 `<span class="hl">…</span>`。
9. 标题用 `<h2>`(节)与 `<h3>`(小节)。**h2 一律不加 class="sec"**(每章独立成 PDF,无需内部分页)。
10. 表格:`<table class="cmp"><tr><th>…</th><th>…</th></tr>…</table>`;C 对照表用 `class="cmp"`。
11. 代码注释用中文;代码必须语法正确、符合该语言主流风格;宁可示例短而精,不要长而错。
12. 写作语言:全书中文;代码标识符用英文。语气正式、面向教学,多举生产场景。

## 五、每章统一结构(严格按此顺序)

```html
<title>第N章 章节名</title>
<!-- no-katex -->
<div class="kicker">FROM C TO <LANG> · PRODUCTION READY</div>
<h1 class="chapter-title">第N章 章节名</h1>
<div class="chapter-meta"><b>建议学时:</b>3–5 小时 | <b>难度:</b>★★★☆☆ | <b>读者背景:</b>已掌握 C 语言</div>

<div class="box box-goal"><div class="box-title">本章学习目标</div><ul>…5–7 条,至少 1 条涉及"C 中怎么做→本语言怎么做"…</ul></div>
<div class="box box-intuition"><div class="box-title">从 C 到 <LANG>:本章主题如何迁移</div><p>…3–6 段,讲清 C 的做法、痛点,本语言的解法与理念…</p></div>
<div class="box box-prereq"><div class="box-title">前置知识自查</div><ul>…需要的 C 知识清单…</ul></div>
<div class="box box-tip"><div class="box-title">本章学习路线</div><ol><li>第一阶段…<li>第二阶段…<li>第三阶段(实验)…<li>验收标准…</ol></div>

<h2>本章知识导图</h2>
<div class="toc-map"><ul>…用嵌套 ul 文字树列出全部小节…</ul></div>

<h2>§N.1 …</h2> …(分节详解:正文 + 代码 + 例题 + 误区 + 重点)
<h2>§N.2 …</h2> …
…(若干节,5–8 节,每节含 h3 细分)

<h2>本章速查表</h2>
<table>…≥12 行…</table>

<h2>最小必会清单</h2>
<ul>…8–12 条,口吻:"能独立说出/写出…"…</ul>

<h2>自测题</h2>
<div class="q">1. …</div><div class="box box-answer"><div class="box-title">参考答案</div>…</div>
…≥5 题…

<h2>章末实验题:…(综合实验名)</h2>
<div class="box box-example lab"><div class="box-title">实验 N:实验名</div>
<p><b>实验目标:</b>…一句话…</p>
<p><b>实验要求:</b></p><ul><li>任务 1(对应本章哪些知识点)…<li>任务 2…<li>…(逐条覆盖全章知识点,用括号标注覆盖的知识点)…</ul>
<p><b>实验环境:</b>…工具/命令…</p></div>
<ol class="steps"><li>…实现提示 1…<li>…实现提示 2…<li>…实现提示 3…</ol>
<div class="box box-answer"><div class="box-title">参考答案要点</div>
<p>…关键代码 ≥60 行(可用多个 pre.code),加设计说明…</p></div>

<hr class="thin">
<p class="small">下一章预告:…</p>
```

## 六、质量硬指标(每章必须达标)

| 指标 | 要求 |
|---|---|
| 章源 HTML 大小 | 18–32 KB(纯内容,不含样式) |
| 正文汉字 | ≥ 3500 字(不含代码) |
| 代码块 | ≥ 8 个 `<pre class="code">`(每个 8–60 行) |
| 易错点 | ≥ 3 个 `box-pitfall` |
| 与 C 的显式对照 | ≥ 3 处(可用 `class="cmp"` 两列表格:"C 的写法 | 本语言的写法") |
| 速查表 | ≥ 12 行 |
| 自测题 | ≥ 5 道(带答案) |
| 章末实验题 | 覆盖全章所有知识点(逐条标注),参考答案代码 ≥ 60 行 |
| 工程实践建议 | ≥ 2 处(box-tip 或正文,涉及规范/性能/安全/工具链) |
| 章节 PDF | 6–12 页;verify.py 无横向溢出、无空页 |
| 全书合并 | ≥ 110 页 |

## 七、14 章内容大纲(每章必须覆盖的点)

> 各章大纲按"通用要求 + 本语言适配"给出。语言没有的机制(如 C++ 无反射、Go 无三目、Rust 无继承),要**如实讲清"本语言没有 X"**,并给出生产中的替代惯用法——这同样满足"不再有困惑"。

### 第1章 数据类型与变量
- **类型总览**:全部内建类型表(类型名/字节数/取值范围),与 C 类型一一对照(char/int/long/float/double 的对应与差异);布尔类型(C 没有真正的 bool);字符类型与编码。
- **变量**:声明语法、初始化与默认值(零值/默认初始化)、未初始化行为的差异(C 局部未初始化是 UB,各语言怎么规定);推导声明(auto/var/:=/let/const)。
- **字面量**:整型(各进制/后缀)、浮点(科学计数法)、字符与字符串字面量、转义序列;语言特色字面量(分隔符、raw 字符串、模板字符串、f-string、字节字面量、多行字符串)。
- **常量**:const 语义与 C 的差异(编译期常量 constexpr/final、可变性 let/const、Python 的"常量只是约定")。
- **类型转换**:隐式转换规则(含整型提升);显式转换语法(static_cast/类型转换函数/as);转换的坑(截断、溢出、JS 隐式转换、Rust as 截断语义)。
- **字符串**:与 C 的 char[] 对照;不可变 vs 可变;长度 vs 字节数;常用 API 速查(拼接/查找/截取/替换/格式化)。
- **复合类型入门**:数组/切片/向量/列表/映射(map/dict/hash)的基础操作(深入在 14 章)。
- **枚举**:从 C 的 enum 到强类型枚举(enum class)、带值枚举(Rust)、JS 的枚举模拟。
- **类型别名**:typedef/using/type;别名 vs 新类型。
- **实验题方向**:设计一个用到全章所有类型与转换的小程序(如:学生信息登记 + 成绩统计,要求覆盖整型/浮点/布尔/字符/字符串/数组/映射/枚举/常量/转换/格式化输出)。

### 第2章 运算符与表达式
- **算术**:溢出行为与 C 对照(C 有符号溢出是 UB;本语言:回绕/panic/大整数/双精度);整数除法与取模的语义差异(负数取模:C 截断 vs Python 向下取整);幂/整除等特色运算符。
- **自增自减**:前置/后置语义;语言差异(Go 中是语句不是表达式;Rust 没有 ++)。
- **位运算**:全套位运算符;语言差异(Go 的 &^、JS 按位 32 位限制、无符号右移);位运算生产场景(标志位、位域、快速运算)。
- **逻辑与短路**:&& || 的短路求值;语言差异(JS/Python 的逻辑运算符返回操作数值而非布尔、Rust 无隐式真值转换)。
- **比较**:== != 语义(值比较 vs 引用比较、=== 与 ==、NaN 的坑、结构体整体比较、浮点比较的正确姿势)。
- **赋值**:复合赋值;解构赋值/模式绑定(JS/Python/Rust);Go 的多重赋值与 :=。
- **三目/条件表达式**:C 的 ?: 在本语言中(Go 没有!Rust/Python 用 if 表达式)。
- **优先级与结合性**:完整优先级表;求值顺序(C 的未定义行为 vs 本语言的规定)。
- **特色运算符**:C++20 <=>、Python //、**、:=、Rust ?、范围 ..、Go 通道 <-。
- **实验题方向**:表达式求值计算器(读入表达式,支持全章运算符,正确处理优先级与短路)。

### 第3章 流程控制
- **if/else**:条件类型要求(Go/Rust/Java 必须布尔 vs JS/Python 真值);if 作为表达式(Rust/Python);悬挂 else。
- **switch/match**:C 的 switch 缺陷(fallthrough、只能整型)→ 本语言的改进;Go 隐式 break、JS 严格 ===、C++ 只能整型、Python 结构匹配 match、Rust 穷尽性 match + if let;模式匹配概念(值/解构/守卫)。
- **循环**:for 的三种形态;while;do-while(哪些语言没有:Go 用 for、Rust 无、Python 无);范围遍历/迭代器遍历(与 C 的 for(i=0;i<n;i++) 对照,给出"现代写法"对照表);循环变量捕获的坑(JS 的 var、Go 1.22 前的经典 bug)。
- **break/continue**:带标签的 break(Go/Rust/JS);break 带值(Rust);Python 的 for-else。
- **goto**:C 的 goto → 各语言态度(Go 有 goto 的合法用法、Java 保留字不可用、Rust/Python/JS 无;现代替代:提前返回、break 标签)。
- **迭代器失效**(C++ 容器遍历中修改)、无限循环写法对照表。
- **实验题方向**:菜单驱动的控制台程序 + 数据过滤统计(强制用到 if/switch/match/三种循环/break/continue,最好带标签跳转),如"文本菜单 + 素数筛选 + 数组遍历统计"。

### 第4章 函数与方法
- **定义与调用**:函数语法、参数与返回;与 C 的函数指针时代对照。
- **参数传递**:值/引用/指针语义全对照表(重点!C 的传值+指针模拟引用 → C++ 引用、Rust 所有权移动与借用、Java/Python/JS/Go 的"值传递,值可能是引用"——列表/字典/切片/对象在函数内修改是否影响外部,必须讲透)。
- **返回值**:多返回值(Go/Rust/Python);命名返回值;返回值优化;JS 解构接收。
- **参数特性**:默认参数(哪些语言有:Python/JS/C++/Go 无/Rust 无);可变参数(...、*args/**kwargs、展开运算符)。
- **一等函数**:函数类型、回调、高阶函数(map/filter/reduce)、函数作为参数/返回值(与 C 的函数指针对照,讲闭包超越函数指针之处)。
- **lambda/闭包**:全部语言的 lambda 语法;捕获语义(C++ 捕获列表、Rust move、JS/Python 词法绑定、Go 闭包与循环变量坑、Python 晚期绑定坑)。
- **重载**:C++/Java 的静态重载;无重载语言的替代(默认参数/泛型/不同函数名)。
- **递归**:深度限制(Python 递归上限)、栈溢出风险、尾递归;Go 栈自动增长。
- **错误返回约定**:Go 的 (T, error)、Rust 的 Result、C 的 errno 对照;异常体系放第9章。
- **入口与结构**:main 函数形态、各语言一个程序的组织方式(与 C 的 main 对照)。
- **实验题方向**:实现一个"函数式风格"的小工具库(回调、闭包、高阶函数、多返回值全覆盖),如:可配置的数组统计/过滤管线。

### 第5章 内存管理
- **内存模型回顾**:栈/堆/静态区;与 C 的 malloc/free/calloc/realloc 对照,指出 C 手动管理的痛点(泄漏、悬垂、重复释放)。
- **手动管理分支(C++/Rust)**:
  - C++:new/delete 与 new[]/delete[];RAII(资源获取即初始化);三大件/五法则;智能指针 unique_ptr/shared_ptr/weak_ptr(各自场景、循环引用、enable_shared_from_this);与 C 的 malloc/free 的差异表(构造/析构调用、类型安全);内存检查工具(ASan/valgrind 一句话)。
  - Rust:所有权三法则(每值一所有者、move 语义、drop);Copy 与 Clone;借用规则(可变/不可变借用互斥);生命周期标注(函数签名里的 'a);智能指针 Box/Rc/Arc/RefCell/Cell 组合拳;与 C 的对照(没有悬垂指针/use-after-free 由编译器保证);unsafe 的边界。
- **GC 分支(Java/Python/JS/Go)**:垃圾回收的基本思想(标记-清除/分代/引用计数);各语言 GC 特点(Java 分代与 Stop-The-World、Python 引用计数+循环回收(gc 模块、__del__ 的坑)、JS V8 分代+标记清除、Go 三色标记+并发);Go 的逃逸分析(变量可能分配在堆上!);内存泄漏在 GC 语言里怎么发生(全局容器、闭包、缓存、定时器、循环引用)与排查工具(内存 profile、pprof)。
- **资源释放惯用法**:C++ RAII、Rust Drop、Go defer、Java try-with-resources、Python with 上下文管理器。
- **生产实践**:避免泄漏清单;分配性能(频繁 new 的代价、对象池、字符串拼接效率)。
- **实验题方向**:内存行为观察实验(如:引用计数变化演示/循环引用与解决/所有权重构练习/内存泄漏复现与修复)。

### 第6章 面向对象
- **从 C 到面向对象**:C 用 struct+函数指针模拟多态的笨拙 → 类的自然表达;讲"面向对象是组织代码的方式"。
- **类的基础**:定义、字段、构造函数(重载/委托构造/初始化列表)、析构函数、this/self;各语言类语法速览。
- **访问控制**:public/private/protected;Python 的 _ 约定(无强制);Rust 的模块私有 pub;Go 的大小写导出(无类!);JS 的 # 私有字段与私有方法。
- **继承**:C++ 单/多继承、虚继承与菱形问题;Java 单继承+接口;Python 多继承与 MRO(C3 线性化);JS 原型链与 class 语法糖(讲清 class 背后的 prototype);Rust 无继承——trait+组合;Go 无继承——struct 嵌入(组合优于继承)。
- **多态**:虚函数与 vtable;抽象类/纯虚/接口;鸭子类型(Python);隐式接口(Go);trait 对象(dyn Trait,Rust);动态分派 vs 静态分派的性能与语义。
- **运算符重载**:C++/Python/Rust(通过 trait)三大家(讲清实现机制);Java/Go/JS 不支持(说明理由与替代)。
- **静态成员/方法**:类属性与类方法(Python @classmethod/@staticmethod、C++/Java static、JS static)。
- **内部类/匿名类**(Java 特色)、嵌套类型(C++/Rust)。
- **设计原则**:组合优于继承;SOLID 简述;何时用继承何时用组合(生产判断标准)。
- **实验题方向**:一个小型类层次设计(如:几何图形/员工薪酬系统),要求继承/接口/多态/运算符重载(语言支持时)/访问控制全覆盖,并给出 UML 式文字说明。

### 第7章 文件 I/O
- **基础读写**:打开/读文本/写文本/追加;编码参数(UTF-8、BOM 坑);与 C 的 fopen/fread/fwrite 对照表(API 对应关系)。
- **二进制 I/O**:字节数组/缓冲区;结构体读写(C 直接 fwrite 结构体的危险 → 本语言的安全做法)。
- **路径与目录**:路径拼接、存在性、目录遍历、创建/删除;各语言标准库(pathlib、std::filesystem、java.nio.file、node:path/fs、std::path、os/filepath);跨平台分隔符与绝对路径。
- **大文件处理**:按行流式读取(内存占用对比)、缓冲、内存映射(可选)。
- **序列化**:JSON(各语言原生支持度与主要库)、二进制序列化(pickle、serde、gob、Java 序列化);序列化的生产坑(版本兼容、安全反序列化)。
- **标准流**:stdin/stdout/stderr 重定向(与 C 对照);命令行参数读取。
- **生产场景**:配置文件读写、CSV 处理(简)、日志文件追加。
- **实验题方向**:一个文件处理工具(如:日志解析+统计+按格式输出),覆盖文本/二进制、目录遍历、JSON 配置、大文件流式处理。

### 第8章 网络通信
- **概念回顾**:TCP/UDP、端口、字节序;与 C 的 socket/htons/htonl 对照(各语言如何封装)。
- **TCP 编程**:socket/connect/bind/listen/accept/send/recv 全套 API 对照表(C → 本语言);完整客户端+服务端示例。
- **UDP 编程**:sendto/recvfrom;无连接语义与适用场景。
- **超时与非阻塞**:settimeout/超时设置、非阻塞模式;为什么必须设置超时(生产事故:永久挂起)。
- **HTTP 客户端**:标准库或事实标准库(Go net/http、Python urllib+requests、Java HttpClient、JS fetch、C++ curl 库/cpp-httplib、Rust reqwest);GET/POST/JSON 完整示例。
- **HTTP 服务端**:最小服务器示例(≤40 行),说明生产框架方向(Flask/Spring/Express/Actix/Gin 一句话)。
- **生产坑**:粘包/拆包(定长/分隔符/长度前缀三种协议设计并给出代码)、连接关闭与 TIME_WAIT、优雅停机。
- **实验题方向**:回声服务器+客户端(覆盖 TCP/UDP、超时、协议设计),或"文件传输+简单 HTTP API"。

### 第9章 并发与异常
- **异常体系**:try/catch/finally 语法;异常层次;自定义异常;各语言差异(Java checked vs unchecked、C++ 异常安全与 noexcept、Python 异常族与 else、Rust 无异常→Result/panic!/?、Go 无异常→error 接口+panic/recover+errors.Is/As/Join、JS throw 与 Promise 错误传播)。
- **线程**:创建/join/分离;各语言线程 API 对照表;线程安全的边界。
- **同步原语**:互斥锁(全语言写法对照)、条件变量、读写锁;原子操作与内存序(简介,生产要点:什么时候必须用原子);死锁的成因与避免(固定加锁顺序、超时)。
- **消息传递**:channel(Go 核心!select 多路复用、缓冲/无缓冲语义)、Rust mpsc、Python Queue、Java BlockingQueue;为什么"用通信共享内存"。
- **并发模型差异**(重点讲本语言):Python GIL(多线程限制→多进程/异步)、JS 事件循环(单线程+异步,宏任务/微任务)、Go CSP(goroutine+channel,百万级并发)、Rust Send/Sync+所有权天然防竞态(+tokio 简介)、C++ std::thread+async(+协程一句话)。
- **async/await**:JS Promise/async/await 深入(必须,该语言核心)、Python asyncio、Rust async/tokio 入门、C++20 协程简介、Go 的"无 async 关键字"(同步风格代码即并发代码)。
- **并发 bug 排查**:竞态、死锁、活锁;检测工具(race detector、TSan)。
- **实验题方向**:生产者-消费者系统(线程+锁+队列/channel+异常处理全覆盖),如"多线程下载模拟器"或"任务队列处理器"。

### 第10章 泛型与元编程
- **为什么需要泛型**:从 C 的 void*/宏/手写多份 的痛点出发。
- **C++**:函数/类模板、模板实参推导、特化与偏特化、SFINAE 与 enable_if、C++20 concepts(约束)、constexpr 元编程(编译期计算);"C++ 模板是代码生成器"的本质。
- **Java**:泛型类/方法、类型擦除(与 C++ 模板的本质差异:擦除 vs 生成)、通配符 ? extends/? super(PECS 原则)、泛型与数组的坑。
- **Python**:动态类型天然泛型;typing 的 TypeVar/Generic/Protocol(标注用);鸭子类型哲学;元编程:元类、__init_subclass__、描述符协议。
- **JavaScript**:无静态类型参数;TypeScript 泛型简介(为读者指出方向);动态元编程:Proxy/Reflect、原型链操纵。
- **Rust**:泛型+特征约束(trait bound)、where 子句、关联类型、const 泛型、单态化(与 C++ 一致,与 Java 擦除不同);宏体系:声明宏 macro_rules!、过程宏(derive/属性/函数式,写一个简单示例)。
- **Go**:类型参数(1.18+)、约束(interface 约束、comparable、~近似元素)、泛型函数/类型示例;与空接口 interface{} 的历史写法对照;Go 泛型的限制(方法不能带类型参数等)。
- **对照表**:各语言"编译期算力"排序与取舍(生成代码 vs 擦除 vs 运行时动态)。
- **实验题方向**:实现一个泛型数据结构(栈/队列/二叉搜索树)+ 元编程小技巧(如:编译期判断、泛型约束的序列化函数),要求写出泛型定义与多种类型实例化。

### 第11章 反射与动态特性
- **概念**:运行时类型信息;为什么 C 没有(只能函数指针表模拟)而高级语言有;反射的用途(序列化、DI 容器、ORM、框架)与代价(性能、类型安全)。
- **C++**:typeid/dynamic_cast 的边界;std::any/type_traits;标准反射的缺失 → 生产替代(宏、代码生成、PFR 库一句话);如实讲清"C++ 反射现状"。
- **Java**:Class 对象、Field/Method/Constructor、getDeclaredXxx vs getXxx、动态代理 Proxy;反射+注解=框架基础;性能与缓存。
- **Python**:全面内省(dir/vars/getattr/setattr/hasattr)、inspect 模块、type() 动态造类、元类、__getattr__/__setattr__/__getattribute__、猴子补丁(生产双刃剑,何时该/不该)。
- **JavaScript**:typeof/instanceof 的坑(数组、null);Object.keys/getOwnPropertyDescriptors;Reflect 与 Proxy 全套(拦截表);动态属性与原型链。
- **Rust**:没有反射!讲清设计取舍(零成本、单态化);替代:Any 与 downcast_ref、trait 对象、serde 的 derive、宏在编译期生成代码。
- **Go**:reflect.Type/Value;三大法则;struct tag 配合(为第12章铺垫);手写迷你 JSON 编解码器(用反射,~40 行,这是最经典的反射教学示例);性能注意(反射比直接调用慢 1–2 个数量级)。
- **生产判断标准**:何时用反射(框架/序列化),何时不用(业务代码);替代手段清单。
- **实验题方向**:自写序列化/对象拷贝工具(如:通用"结构体→JSON"或"对象属性遍历打印器"),覆盖本语言全部反射 API。

### 第12章 注解/装饰器
- **概念**:元数据 vs 行为注入;与 C 的 #pragma/__attribute__ 对照。
- **C++**:标准属性 [[nodiscard]] [[deprecated]] [[fallthrough]] [[maybe_unused]] [[likely]] 等逐个示例;编译器扩展(__attribute__、__declspec)一句话。
- **Java**:内置注解(@Override/@Deprecated/@SuppressWarnings/@FunctionalInterface);元注解(@Target/@Retention);自定义注解完整示例;运行时读取(反射)与编译时(注解处理器,如 Lombok 原理一句话)。
- **Python**:装饰器本质(函数即对象,闭包返回函数);@ 语法糖的等价形式;带参数装饰器;functools.wraps(必须讲!元数据保留);类装饰器;常用装饰器(staticmethod/classmethod/property/contextmanager/functools.lru_cache);自写:计时/重试/缓存/登录校验装饰器。
- **JavaScript**:装饰器提案(TC39)与 TypeScript 装饰器(类/方法/属性/参数装饰器,为读框架代码做准备);高阶函数式"装饰"(组合函数)。
- **Rust**:属性分类(内建/derive/工具);#[derive(Debug, Clone, PartialEq, Serialize)] 的原理(过程宏展开思想);#[cfg]/#[cfg(feature)] 条件编译;#[allow]/#[warn];自定义属性宏入门。
- **Go**:没有注解;struct tag(`json:"name" omitempty` 等)与反射配合;go:generate + stringer 生成 String 方法的完整示例;代码生成哲学("生成优于魔法")。
- **实验题方向**:自写装饰器/注解框架(如:函数执行计时+重试装饰器,或 校验注解+反射校验器,或 stringer 式代码生成器),并在实验中实际使用自己写的东西。

### 第13章 模块系统
- **从 C 出发**:#include 头文件模式的弊端(重复包含、编译慢、符号冲突、无包管理)→ 各语言的解法总览。
- **C++**:头文件/源文件组织、include guard 与 #pragma once、ODR 与 inline、命名空间(与 C 全局符号冲突对照)、extern "C" 与 C 互操作、C++20 modules 简介与现状、构建工具(CMake 一句话)、静态库/动态库。
- **Java**:package 与目录对应、import/静态导入、包可见性、classpath、JAR、JPMS 模块系统(module-info.java,9+)、构建工具 Maven/Gradle 概念(依赖坐标、仓库)。
- **Python**:模块=文件、包=目录(__init__.py)、import 机制与 sys.path、绝对/相对导入、__name__=="__main__"、虚拟环境 venv、pip 与 requirements.txt/pyproject.toml、发布到 PyPI 一句话。
- **JavaScript**:历史(CJS require/module.exports → ESM import/export)、package.json、node_modules 与 npm、ESM/CJS 互操作坑、打包器(esbuild/webpack)一句话、作用域(模块级封装,与 C 的全局污染对照)。
- **Rust**:crate 概念、mod 树、use/pub/路径规则(相对/绝对、super)、Cargo 全流程(new/build/run/test/发布)、Cargo.toml 依赖与 features、workspace、crates.io;单文件内模块示例(嵌套 mod)。
- **Go**:package 与目录、导入路径(域名风格)、go.mod 与 module、go get/go mod tidy、internal 目录规则、空白导入 _、gofmt 强制风格;可执行包 main。
- **依赖管理对照表**:锁文件、语义化版本、离线/私有源。
- **实验题方向**:把一个 3–5 个文件的小项目按本语言规范组织起来并成功构建/运行(如:数学工具库 + 命令行入口 + 第三方依赖各一个),输出目录树与构建命令。

### 第14章 类型系统进阶
- **类型推导**:auto/var/:=/let 与 C 无推导对照;推导边界(函数签名是否显式标注——工程约定);过度推导的可读性代价。
- **指针/引用/值语义**(深化):C 指针 → C++ 引用/智能指针、Java/Python/JS 的"引用即一切"(深浅拷贝!)、Rust 所有权回顾(作为类型系统组成部分)、Go 指针(无指针运算,与 C 对照);值语义 vs 引用语义表。
- **空安全**:null 的"十亿美元错误";Optional/Option/nullable/None;各语言空处理惯用法(空对象模式、提前返回、map/or_else);JS 的 null/undefined 双重空与 ?? 运算符。
- **联合与和类型**:C 的 union(不安全)→ C++ std::variant(安全 union,访问方式)、Rust enum(和类型!Option/Result 都是它)、Python Union/typing 与 match、Java sealed classes + record 模式匹配(现代 Java!)、TS 判别联合(概念对照)。
- **结构体与数据类**:struct 的演进;C++ 聚合初始化;Java record、Python @dataclass、JS 对象/类、Rust struct + derive、Go struct 与方法。
- **类型别名与新类型**:typedef/using/type、newtype 模式(Rust/Go 的包装类型防混淆)。
- **静态 vs 动态、强 vs 弱**:类型系统四象限;JS 的隐式转换之坑(动态弱)、Python 的强动态(运行时 TypeError);渐进类型(Python typing/JS→TS/JSDoc)趋势。
- **进阶话题**:协变/逆变(数组协变的坑、泛型的型变),只讲实用结论;类型与性能(单态化 vs 擦除 vs 动态分派)。
- **生产建议**:何时标注类型、公共 API 的类型契约、类型即文档。
- **实验题方向**:大综合——设计一个"类型安全"的小型领域模型(如:订单+商品+用户系统),综合运用全章(推导、可选类型、联合类型、数据类、别名/新类型、深浅拷贝、类型标注),并写一个"遍历+汇总"消费该模型的程序。

## 八、封面规范

参考 `chapters/cover.html` 的结构(210mm×296mm 渐变底 + 细边框 + 装饰圆),替换为:

- kicker 行:`FROM C TO <LANG> · PRODUCTION-READY REFERENCE`
- 主标题:`<语言名> 使用大全`(42pt 加粗)
- 副标题:`面向 C 语言学习者的 14 讲系统进阶`(16pt 字距 10px)
- 说明文字:`覆盖 14 大主题:数据类型 → 运算符 → 流程控制 → 函数 → 内存管理 → 面向对象 → 文件I/O → 网络 → 并发异常 → 泛型与元编程 → 反射 → 注解装饰器 → 模块系统 → 类型系统进阶`
- 14 章列表(两行小字):`第1章 数据类型与变量 · 第2章 运算符与表达式 · 第3章 流程控制 · 第4章 函数与方法 · 第5章 内存管理 · 第6章 面向对象 · 第7章 文件I/O`<br>`第8章 网络通信 · 第9章 并发与异常 · 第10章 泛型与元编程 · 第11章 反射与动态特性 · 第12章 注解与装饰器 · 第13章 模块系统 · 第14章 类型系统进阶`
- 底部一行:`知识导图 × C 语言对照 × 例题精讲 × 章末实验`
- 各语言渐变主色(160deg 三色):

| 语言 | 渐变三色 | 强调色 |
|---|---|---|
| C++ | #0e2a43 → #164863 → #1a5276 | #ffd76e |
| Python | #0d3b4f → #1a5a7a → #2a7f8f | #ffd76e |
| Java | #3d1408 → #7a2d0e → #b34700 | #ffc87a |
| JavaScript | #3d2b00 → #7a5a00 → #b38a00 | #ffe08a |
| Rust | #3d1a08 → #7a330e → #b3501a | #ffd0a8 |
| Go | #05333d → #0b5c6e → #1199b8 | #9ee8f7 |

## 九、QA 与报告要求

1. **边写边验**:每写完 3–4 章即执行 build + render + verify;build 报错(KaTeX/SVG)或 verify 报横向溢出/空页,必须修复后才继续。
2. 每章 verify 输出应显示页数在 6–12 页之间;不足补内容(优先补例题与误区),超过可接受(宁多勿少)。
3. 全部完成后执行合并,并对合并 PDF 执行 `python tools/verify.py`(总页数 ≥110 页)。
4. 不修改 langdocs/<自己的语言>/ 之外的任何文件;不提交 git。
5. **最终报告**(你的最后一条消息)必须包含:
   - 各章页数清单(ch01–ch14)、封面页数、合并总页数
   - 最终 PDF 完整路径与文件大小
   - 横向溢出/空页检查结果
   - 与规范的任何偏离(如实说明)
