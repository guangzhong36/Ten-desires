// lab3.cpp —— 中缀表达式计算器(第一段:词法与转换)
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <cctype>
#include <stdexcept>
using namespace std;

// 任务 1:拆分 token(支持多位数与空格)
vector<string> tokenize(const string& s) {
    vector<string> toks;
    for (size_t i = 0; i < s.size();) {
        if (isspace((unsigned char)s[i])) { ++i; continue; }
        if (isdigit((unsigned char)s[i])) {
            string num;
            while (i < s.size() && isdigit((unsigned char)s[i]))
                num += s[i++];
            toks.push_back(num);
        } else {
            toks.push_back(string(1, s[i++]));
        }
    }
    return toks;
}

int prec(const string& op) { return (op == "+" || op == "-") ? 1 : 2; }

// 任务 2:中缀转后缀(调度场算法)
vector<string> infixToPostfix(const vector<string>& toks) {
    vector<string> out;
    stack<string> st;
    for (const string& t : toks) {
        if (isdigit((unsigned char)t[0]))
            out.push_back(t);                       // 数字直接输出
        else if (t == "(") st.push(t);
        else if (t == ")") {
            while (!st.empty() && st.top() != "(") {
                out.push_back(st.top()); st.pop();
            }
            if (st.empty()) throw runtime_error("括号不匹配");
            st.pop();                               // 丢弃左括号
        } else {                                    // 运算符
            while (!st.empty() && st.top() != "(" &&
                   prec(st.top()) >= prec(t)) {
                out.push_back(st.top()); st.pop();
            }
            st.push(t);
        }
    }
    while (!st.empty()) {
        if (st.top() == "(") throw runtime_error("括号不匹配");
        out.push_back(st.top()); st.pop();
    }
    return out;
}

// 任务 3:后缀求值(注意弹出顺序)
int evalPostfix(const vector<string>& toks) {
    stack<int> st;
    for (const string& t : toks) {
        if (isdigit((unsigned char)t[0]))
            st.push(stoi(t));
        else {
            if (st.size() < 2) throw runtime_error("操作数不足");
            int b = st.top(); st.pop();   // 先弹的是右操作数
            int a = st.top(); st.pop();
            if (t == "+") st.push(a + b);
            else if (t == "-") st.push(a - b);
            else if (t == "*") st.push(a * b);
            else {
                if (b == 0) throw runtime_error("除数为 0");
                st.push(a / b);
            }
        }
    }
    if (st.size() != 1) throw runtime_error("表达式非法");
    return st.top();
}

int main() {
    cout << "中缀表达式计算器(支持 + - * / 与括号,输入 quit 退出)\n";
    string line;
    while (true) {
        cout << "> ";
        getline(cin, line);
        if (line == "quit") break;
        try {
            auto toks = tokenize(line);
            auto post = infixToPostfix(toks);
            int result = evalPostfix(post);
            cout << "后缀式: ";
            for (const string& t : post) cout << t << ' ';
            cout << "\n结果: " << result << endl;
        } catch (const exception& e) {
            cout << "错误: " << e.what() << endl;
        }
    }
    return 0;
}
