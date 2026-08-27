# -*- coding: utf-8 -*-
"""从 ch12 HTML 提取章末实验参考答案代码,还原实体,写为 lab.cpp / lab.py。"""
import re
import sys

def extract(html_path, out_path):
    html = open(html_path, encoding="utf-8").read()
    m = html.find("章末实验题")
    seg = html[m:]
    end = seg.find("示例输出")
    if end >= 0:
        seg = seg[:end]
    blocks = re.findall(r'<pre class="code">(.*?)</pre>', seg, flags=re.S)
    code = "\n".join(b.strip("\n") for b in blocks)
    code = (code.replace("&lt;", "<").replace("&gt;", ">")
                .replace("&amp;", "&").replace("&quot;", '"'))
    open(out_path, "w", encoding="utf-8", newline="\n").write(code + "\n")
    print(f"{out_path}: {len(code.splitlines())} 行")

extract(sys.argv[1], sys.argv[2])
