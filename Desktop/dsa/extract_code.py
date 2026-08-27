# -*- coding: utf-8 -*-
"""抽取章节 HTML 中全部代码块到临时目录(还原 HTML 实体),供编译/运行验证。
用法: python dsa/extract_code.py dsa/cpp/src/ch03_表栈和队列.html [输出目录]
默认输出到 dsa/_codecheck/<章节名>/blockNN.txt,并打印索引(块号/标题/行数)。"""
import re
import sys
import pathlib


def unescape(s: str) -> str:
    return (s.replace("&lt;", "<").replace("&gt;", ">")
             .replace("&amp;", "&").replace("&quot;", '"')
             .replace("&#39;", "'").replace("&nbsp;", " "))


def main() -> None:
    src = pathlib.Path(sys.argv[1])
    outdir = pathlib.Path(sys.argv[2]) if len(sys.argv) > 2 else (
        pathlib.Path(__file__).parent / "_codecheck" / src.stem)
    html = src.read_text(encoding="utf-8")

    # 找出所有代码块及其前面的 code-title
    blocks = list(re.finditer(r'<pre class="code">(.*?)</pre>', html, flags=re.S))
    titles = []
    for m in blocks:
        before = html[:m.start()]
        tm = re.findall(r'<div class="code-title">(.*?)</div>', before)
        titles.append(tm[-1] if tm else "(无标题)")

    outdir.mkdir(parents=True, exist_ok=True)
    print(f"共 {len(blocks)} 个代码块 -> {outdir}")
    for i, m in enumerate(blocks, 1):
        code = unescape(m.group(1)).strip("\n") + "\n"
        nlines = code.count("\n")
        (outdir / f"block{i:02d}.txt").write_text(code, encoding="utf-8")
        print(f"  block{i:02d} | {nlines:3d} 行 | {titles[i-1]}")


if __name__ == "__main__":
    main()
