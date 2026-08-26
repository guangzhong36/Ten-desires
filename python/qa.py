# -*- coding: utf-8 -*-
"""QA 脚本:统计 langdocs/python/src 下各章源 HTML 的硬指标。
用法: python langdocs/python/qa.py [ch01]  (可只查某章,默认全部)"""
import re
import sys
import pathlib

SRC = pathlib.Path(__file__).parent / "src"
CJK = re.compile(r"[一-鿿　-〿＀-￯]")


def body_text(html: str) -> str:
    html = re.sub(r"<!--.*?-->", "", html, flags=re.S)          # 注释
    html = re.sub(r'<pre class="code">.*?</pre>', "", html, flags=re.S)  # 代码块
    html = re.sub(r"<[^>]+>", "", html)                          # 标签
    return html


def report(f: pathlib.Path) -> None:
    html = f.read_text(encoding="utf-8")
    size = f.stat().st_size
    cjk = len(CJK.findall(body_text(html)))
    blocks = re.findall(r'<pre class="code">(.*?)</pre>', html, flags=re.S)
    lines = [b.strip("\n").count("\n") + 1 for b in blocks]
    pitfalls = html.count("box-pitfall")
    cmp_tables = len(re.findall(r'<table class="cmp">', html))
    m = html.find("本章速查表")
    qrows = len(re.findall(r"<tr>", html[m:])) - 1 if m >= 0 else 0
    questions = html.count('<div class="q">')
    labm = html.find("章末实验题")
    labseg = html[labm:] if labm >= 0 else ""
    lablines = sum(b.strip("\n").count("\n") + 1 for b in
                   re.findall(r'<pre class="code">(.*?)</pre>', labseg, flags=re.S))
    tips = html.count("box-tip")
    bad = [n for n in lines if not 8 <= n <= 60]
    flag = ""
    if cjk < 3500:
        flag += "  !!汉字不足"
    if len(blocks) < 8:
        flag += "  !!代码块不足"
    if bad:
        flag += f"  !!代码块行数越界{bad}"
    if pitfalls < 3:
        flag += "  !!pitfall不足"
    if cmp_tables < 3:
        flag += "  !!C对照表不足"
    if qrows < 12:
        flag += "  !!速查表行数不足"
    if questions < 5:
        flag += "  !!自测题不足"
    if lablines < 60:
        flag += "  !!实验参考答案不足60行"
    if not 18 * 1024 <= size <= 32 * 1024:
        flag += f"  !!体积{size/1024:.1f}KB不在18-32KB"
    print(f"{f.name}: {size/1024:6.1f}KB 汉字={cjk} 代码块={len(blocks)}"
          f" 行数={lines} pitfall={pitfalls} cmp={cmp_tables} 速查行={qrows}"
          f" 自测={questions} 实验码行={lablines} tip={tips}{flag}")


def main() -> None:
    names = sys.argv[1:] or sorted(p.name for p in SRC.glob("ch*.html"))
    for n in names:
        report(SRC / n)


if __name__ == "__main__":
    main()
