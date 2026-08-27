# lab3.py —— 中缀表达式计算器(第一段:词法与转换)
def tokenize(s):
    """任务 1:拆分 token(支持多位数与空格)。"""
    toks = []
    i = 0
    while i < len(s):
        if s[i].isspace():
            i += 1
            continue
        if s[i].isdigit():
            j = i
            while j < len(s) and s[j].isdigit():
                j += 1
            toks.append(s[i:j])
            i = j
        else:
            toks.append(s[i])
            i += 1
    return toks

def prec(op):
    return 1 if op in "+-" else 2

def infix_to_postfix(toks):
    """任务 2:中缀转后缀(调度场算法)。"""
    out = []
    st = []
    for t in toks:
        if t.lstrip("-").isdigit():
            out.append(t)                       # 数字直接输出
        elif t == "(":
            st.append(t)
        elif t == ")":
            while st and st[-1] != "(":
                out.append(st.pop())
            if not st:
                raise ValueError("括号不匹配")
            st.pop()                            # 丢弃左括号
        else:                                   # 运算符
            while st and st[-1] != "(" and prec(st[-1]) >= prec(t):
                out.append(st.pop())
            st.append(t)
    while st:
        if st[-1] == "(":
            raise ValueError("括号不匹配")
        out.append(st.pop())
    return out
def eval_postfix(toks):
    """任务 3:后缀求值(注意弹出顺序)。"""
    st = []
    for t in toks:
        if t.lstrip("-").isdigit():
            st.append(int(t))
        else:
            if len(st) < 2:
                raise ValueError("操作数不足")
            b = st.pop()                        # 先弹的是右操作数
            a = st.pop()
            if t == "+":
                st.append(a + b)
            elif t == "-":
                st.append(a - b)
            elif t == "*":
                st.append(a * b)
            else:
                if b == 0:
                    raise ValueError("除数为 0")
                st.append(a // b)
    if len(st) != 1:
        raise ValueError("表达式非法")
    return st[0]

def main():
    print("中缀表达式计算器(支持 + - * / 与括号,输入 quit 退出)")
    while True:
        line = input("> ").strip()
        if line == "quit":
            break
        try:
            toks = tokenize(line)
            post = infix_to_postfix(toks)
            result = eval_postfix(post)
            print("后缀式:", " ".join(post))
            print("结果:", result)
        except ValueError as e:
            print("错误:", e)

if __name__ == "__main__":
    main()
