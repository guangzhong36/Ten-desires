// lab9.cpp —— 迷宫最短路径(BFS + Dijkstra)
// 编译: g++ -std=c++17 -O2 -o lab9 lab9.cpp
// 运行: ./lab9          (内置迷宫)
//       ./lab9 sealed   (内置无解样例:S 被墙封死)
//       ./lab9 maze.txt (从文件读迷宫)
#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <queue>
#include <string>
#include <utility>
#include <vector>
using namespace std;

// 任务 1 辅助:字符 -> 通过代价(墙返回 -1)
int costOf(char c) {
    switch (c) {
        case 'S': case 'E': case '.': return 1;   // 平地
        case '~': return 5;                       // 沼泽:代价高
        default:  return -1;                      // '#' 墙
    }
}

vector<string> readMaze(const string& arg) {
    if (arg == "sealed") {                        // 内置样例 2:S 被墙封死
        return {"#####", "##S##", "#.###", "#.E.#", "#####"};
    }
    if (!arg.empty()) {                           // 从文件读
        ifstream in(arg);
        vector<string> m;
        string line;
        while (getline(in, line))
            if (!line.empty()) m.push_back(line);
        return m;
    }
    return {                                      // 内置样例 1:
        "##########",                             // 上走廊短(12 步)但有两格
        "#S...~~..#",                             // 沼泽(代价 21);下走廊长
        "#.#####.##",                             // (18 步)但全平地(代价 18)
        "#.#####.##",                             // ——BFS 选短,Dijkstra 选廉
        "#.##....##",
        "#.##.##.##",
        "#.##.##.##",
        "#....##E##",
        "##########",
        "##########",
    };
}

// 任务 1:网格 -> 图,顶点编号 id = 行 * 宽 + 列,四方向邻居即边(隐式图)
int findPos(const vector<string>& maze, char ch) {
    int w = int(maze[0].size());
    for (int r = 0; r < int(maze.size()); ++r)
        for (int c = 0; c < w; ++c)
            if (maze[r][c] == ch) return r * w + c;
    return -1;
}

// 任务 2:无权最短路(BFS)。返回步数;无解返回 -1
int bfsShortest(const vector<string>& maze, int sid, int tid, vector<int>& path) {
    int h = int(maze.size()), w = int(maze[0].size());
    vector<int> dist(h * w, -1), prev(h * w, -1);
    queue<int> q;                                 // 队列(第3章)
    dist[sid] = 0;
    q.push(sid);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        if (u == tid) break;
        int r = u / w, c = u % w;
        int dr[4] = {-1, 1, 0, 0}, dc[4] = {0, 0, -1, 1};
        for (int k = 0; k < 4; ++k) {
            int nr = r + dr[k], nc = c + dc[k];
            if (nr < 0 || nr >= h || nc < 0 || nc >= w) continue;
            if (maze[nr][nc] == '#') continue;    // 墙不可走
            int v = nr * w + nc;
            if (dist[v] != -1) continue;          // 第一次到达即最短
            dist[v] = dist[u] + 1;
            prev[v] = u;
            q.push(v);
        }
    }
    if (dist[tid] == -1) return -1;               // 任务 5:终点不可达
    for (int x = tid; x != -1; x = prev[x])       // 前驱数组回溯
        path.push_back(x);
    reverse(path.begin(), path.end());
    return int(path.size()) - 1;
}

// 任务 3:最低代价路径(Dijkstra 堆优化)。返回总代价;无解返回 -1
int dijkstraCost(const vector<string>& maze, int sid, int tid, vector<int>& path) {
    const int INF = 1e9;                          // 大常量,INF + w 不溢出
    int h = int(maze.size()), w = int(maze[0].size());
    vector<int> dist(h * w, INF), prev(h * w, -1);
    priority_queue<pair<int, int>, vector<pair<int, int>>,
                   greater<pair<int, int>>> pq;   // 最小堆(第6章)
    dist[sid] = 0;
    pq.push({0, sid});
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue;                // 懒删除:过期记录直接跳过
        if (u == tid) break;
        int r = u / w, c = u % w;
        int dr[4] = {-1, 1, 0, 0}, dc[4] = {0, 0, -1, 1};
        for (int k = 0; k < 4; ++k) {
            int nr = r + dr[k], nc = c + dc[k];
            if (nr < 0 || nr >= h || nc < 0 || nc >= w) continue;
            int wgt = costOf(maze[nr][nc]);
            if (wgt < 0) continue;                // 墙
            int v = nr * w + nc;
            if (dist[v] > d + wgt) {
                dist[v] = d + wgt;
                prev[v] = u;
                pq.push({dist[v], v});
            }
        }
    }
    if (dist[tid] == INF) return -1;
    for (int x = tid; x != -1; x = prev[x])
        path.push_back(x);
    reverse(path.begin(), path.end());
    return dist[tid];
}

void drawPath(const vector<string>& maze, const vector<int>& path, int w) {
    vector<string> grid = maze;
    for (int id : path)
        if (grid[id / w][id % w] != 'S' && grid[id / w][id % w] != 'E')
            grid[id / w][id % w] = '*';           // 用 * 标出路径
    for (const string& row : grid) {
        for (char ch : row) cout << ' ' << ch;
        cout << '\n';
    }
}

void printPathCoords(const vector<int>& path, int w) {
    for (size_t i = 0; i < path.size(); ++i)
        cout << (i ? " -> " : "")
             << "(" << path[i] / w << "," << path[i] % w << ")";
    cout << '\n';
}

int main(int argc, char* argv[]) {
    string arg = argc > 1 ? argv[1] : "";
    vector<string> maze = readMaze(arg);
    int w = int(maze[0].size());
    int s = findPos(maze, 'S'), t = findPos(maze, 'E');
    if (s < 0 || t < 0) {
        cout << "迷宫缺少起点 S 或终点 E\n";
        return 1;
    }
    cout << "迷宫 " << maze.size() << " 行 x " << w << " 列,起点 ("
         << s / w << "," << s % w << "),终点 (" << t / w << "," << t % w << ")\n";
    cout << "图例: # 墙  . 平地(代价1)  ~ 沼泽(代价5)  S/E 起点/终点\n";

    // 任务 2:BFS 无权最短路
    vector<int> pathB;
    int steps = bfsShortest(maze, s, t, pathB);
    if (steps < 0) {
        cout << "BFS  : 无解——起点到终点不可达(被墙封死)\n";
    } else {
        cout << "BFS  : 最短步数 = " << steps << "\n  路径: ";
        printPathCoords(pathB, w);
        drawPath(maze, pathB, w);
    }

    // 任务 3:Dijkstra 最低代价路径
    vector<int> pathD;
    int cost = dijkstraCost(maze, s, t, pathD);
    if (cost < 0) {
        cout << "Dijkstra: 无解\n";
    } else {
        cout << "Dijkstra: 最低总代价 = " << cost
             << ",步数 = " << int(pathD.size()) - 1 << "\n  路径: ";
        printPathCoords(pathD, w);
        drawPath(maze, pathD, w);
    }

    // 任务 4:两种路径对比
    if (steps >= 0 && cost >= 0) {
        int bfsCost = 0;                          // BFS 路径的真实代价
        for (int id : pathB) bfsCost += costOf(maze[id / w][id % w]);
        cout << "对比: BFS 最短步数 " << steps << "(路径代价 " << bfsCost
             << ") vs Dijkstra 最低代价路径步数 " << int(pathD.size()) - 1
             << "(代价 " << cost << ")\n";
    }
    return 0;
}
