# -*- coding: utf-8 -*-
"""QA 脚本:DSA 教辅章节源 HTML 硬指标统计。
用法: python dsa/qa_metrics.py dsa/cpp/src/ch03_表栈和队列.html
退出码: 0=全部达标, 1=存在不达标项"""
import re
import sys
import pathlib

CJK = re.compile(r"[一-鿿　-〿＀-￯]")

# guide.css 中允许使用的 class 白名单(见 assets/guide.css)
WHITELIST = {
    "kicker", "chapter-title", "chapter-meta",
    "box", "box-goal", "box-intuition", "box-prereq", "box-tip",
    "box-keypoint", "box-example", "box-pitfall", "box-summary",
    "box-answer", "box-title",
    "toc-map", "code-title", "code", "cmp", "tight",
    "steps", "q", "lab", "term", "hl", "thin", "small", "center",
}


def body_text(html: str) -> str:
    html = re.sub(r"<!--.*?-->", "", html, flags=re.S)
    html = re.sub(r'<pre class="code">.*?</pre>', "", html, flags=re.S)
    html = re.sub(r"<[^>]+>", "", html)
    return html


def main() -> None:
    path = pathlib.Path(sys.argv[1])
    html = path.read_text(encoding="utf-8")
    size = path.stat().st_size
    fails = []

    # ---- 文件头 ----
    lines0 = html.splitlines()
    if not lines0 or "<title>" not in lines0[0]:
        fails.append("首行缺 <title>")
    if len(lines0) < 2 or "no-katex" not in lines0[1]:
        fails.append("第2行缺 <!-- no-katex -->")

    # ---- 基础指标 ----
    cjk = len(CJK.findall(body_text(html)))
    blocks = re.findall(r'<pre class="code">(.*?)</pre>', html, flags=re.S)
    blines = [b.strip("\n").count("\n") + 1 for b in blocks]
    pitfalls = html.count("box-pitfall")
    cmp_tables = len(re.findall(r'<table class="cmp">', html))
    tips = html.count("box-tip")
    questions = html.count('<div class="q">')

    # 复杂度速查表行数(锚定 h2 标题,避免正文提及的干扰;含表头)
    m = html.find("<h2>本章复杂度速查表</h2>")
    seg = html[m:] if m >= 0 else ""
    qrows = len(re.findall(r"<tr>", seg)) - 1

    # 章末实验题参考答案代码行数
    labm = html.find("章末实验题")
    labseg = html[labm:] if labm >= 0 else ""
    lablines = sum(b.strip("\n").count("\n") + 1 for b in
                   re.findall(r'<pre class="code">(.*?)</pre>', labseg, flags=re.S))

    # 节数(h2 含 §)
    sections = len(re.findall(r"<h2>§", html))

    # 未知 class 与内联 style
    used = set()
    for cls in re.findall(r'class="([^"]*)"', html):
        used.update(cls.split())
    unknown = used - WHITELIST
    inline_styles = len(re.findall(r"<[^>]+\sstyle=", html))

    if cjk < 3500:
        fails.append(f"汉字不足({cjk})")
    if len(blocks) < 8:
        fails.append(f"代码块不足({len(blocks)})")
    bad = [n for n in blines if not 8 <= n <= 60]
    if bad:
        fails.append(f"代码块行数越界{bad}")
    if pitfalls < 3:
        fails.append(f"pitfall不足({pitfalls})")
    if cmp_tables < 3:
        fails.append(f"C对照表不足({cmp_tables})")
    if m < 0 or qrows < 15:
        fails.append(f"复杂度速查表行数不足({qrows})")
    if questions < 5:
        fails.append(f"自测题不足({questions})")
    if lablines < 60:
        fails.append(f"实验参考答案不足60行({lablines})")
    if not 18 * 1024 <= size <= 42 * 1024:
        fails.append(f"体积{size/1024:.1f}KB不在18-42KB")
    if not 6 <= sections <= 8:
        fails.append(f"正文节数({sections})不在6-8")
    if unknown:
        fails.append(f"未知class:{sorted(unknown)}")
    if inline_styles:
        fails.append(f"内联style {inline_styles} 处")
    if tips < 2:
        fails.append(f"box-tip不足({tips})")

    print(f"{path.name}: {size/1024:6.1f}KB 汉字={cjk} 代码块={len(blocks)}"
          f" 行数={blines} 节数={sections} pitfall={pitfalls} cmp={cmp_tables}"
          f" 速查行={qrows} 自测={questions} 实验码行={lablines} tip={tips}")
    if fails:
        print("  !! 不达标:" + "; ".join(fails))
        sys.exit(1)
    print("  ✓ 全部达标")


if __name__ == "__main__":
    main()
