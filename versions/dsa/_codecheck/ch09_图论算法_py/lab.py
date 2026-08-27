# lab9.py —— 迷宫最短路径(BFS + Dijkstra)
# 运行: python lab9.py            # 内置迷宫
#       python lab9.py sealed     # 内置"无解"迷宫(S 被墙封死)
#       python lab9.py maze.txt   # 从文件读迷宫
import heapq
import sys
from collections import deque

# 内置迷宫:'#' 墙,'.' 平地(代价 1),'~' 沼泽(代价 5),'S' 起点,'E' 终点
# 设计: 上走廊短(12 步)但有两格沼泽(代价 21);
#       下走廊长(18 步)但全平地(代价 18)——BFS 选短,Dijkstra 选廉。
MAZE = """\
##########
#S...~~..#
#.#####.##
#.#####.##
#.##....##
#.##.##.##
#.##.##.##
#....##E##
##########
##########\
"""

SEALED = """\
#####
##S##
#.###
#.E.#
#####\
"""

COST = {'S': 1, 'E': 1, '.': 1, '~': 5}


def read_maze(arg):
    if arg == "sealed":
        return SEALED.splitlines()
    if arg:
        with open(arg, encoding="utf-8") as f:
            return [line.rstrip("\n") for line in f if line.strip()]
    return MAZE.splitlines()


def find_pos(maze, ch):
    for r, row in enumerate(maze):
        for c, cell in enumerate(row):
            if cell == ch:
                return r, c
    return -1, -1


def neighbors(maze, r, c):
    """任务 1:网格 -> 图,四方向邻居即边(隐式图)。"""
    h, w = len(maze), len(maze[0])
    for dr, dc in ((-1, 0), (1, 0), (0, -1), (0, 1)):
        nr, nc = r + dr, c + dc
        if 0 <= nr < h and 0 <= nc < w and maze[nr][nc] != '#':
            yield nr, nc


def bfs_shortest(maze, s, t):
    """任务 2:无权最短路(BFS)。返回 (步数, 路径坐标列表);无解返回 None。"""
    q = deque([s])
    prev = {s: None}
    while q:
        u = q.popleft()
        if u == t:
            break
        for v in neighbors(maze, *u):
            if v not in prev:          # 第一次到达即最短
                prev[v] = u
                q.append(v)
    if t not in prev:
        return None                    # 任务 5:终点不可达
    path = []
    x = t
    while x is not None:               # 前驱数组回溯
        path.append(x)
        x = prev[x]
    path.reverse()
    return len(path) - 1, path


def dijkstra_cost(maze, s, t):
    """任务 3:最低代价路径(Dijkstra 堆优化)。返回 (总代价, 路径);无解返回 None。"""
    h, w = len(maze), len(maze[0])
    INF = float('inf')                 # Python 用无穷大,无溢出问题
    dist = [[INF] * w for _ in range(h)]
    prev = {}
    dist[s[0]][s[1]] = 0
    pq = [(0, s)]                      # (当前代价, 坐标),小顶堆
    while pq:
        d, u = heapq.heappop(pq)
        if d > dist[u[0]][u[1]]:
            continue                   # 懒删除:过期记录直接跳过
        if u == t:
            break
        for v in neighbors(maze, *u):
            nd = d + COST[maze[v[0]][v[1]]]
            if nd < dist[v[0]][v[1]]:
                dist[v[0]][v[1]] = nd
                prev[v] = u
                heapq.heappush(pq, (nd, v))
    if dist[t[0]][t[1]] == INF:
        return None
    path = []
    x = t
    while x is not None:
        path.append(x)
        x = prev.get(x)
    path.reverse()
    return dist[t[0]][t[1]], path


def draw_path(maze, path):
    """把路径画回迷宫:* 标出经过的格子。"""
    grid = [list(row) for row in maze]
    for r, c in path:
        if grid[r][c] not in 'SE':
            grid[r][c] = '*'
    return [' '.join(row) for row in grid]


def main():
    arg = sys.argv[1] if len(sys.argv) > 1 else ""
    maze = read_maze(arg)
    s, t = find_pos(maze, 'S'), find_pos(maze, 'E')
    if s == (-1, -1) or t == (-1, -1):
        print("迷宫缺少起点 S 或终点 E")
        return
    print(f"迷宫 {len(maze)} 行 x {len(maze[0])} 列,起点 {s},终点 {t}")
    print("图例: # 墙  . 平地(代价1)  ~ 沼泽(代价5)  S/E 起点/终点")

    res_bfs = bfs_shortest(maze, s, t)
    if res_bfs is None:
        print("BFS  : 无解——起点到终点不可达(被墙封死)")
    else:
        steps, path = res_bfs
        print(f"BFS  : 最短步数 = {steps}")
        print("  路径:", " -> ".join(f"({r},{c})" for r, c in path))
        for row in draw_path(maze, path):
            print("   ", row)

    res_dij = dijkstra_cost(maze, s, t)
    if res_dij is None:
        print("Dijkstra: 无解")
    else:
        cost, path = res_dij
        print(f"Dijkstra: 最低总代价 = {cost},步数 = {len(path)-1}")
        print("  路径:", " -> ".join(f"({r},{c})" for r, c in path))
        for row in draw_path(maze, path):
            print("   ", row)

    # 任务 4:两种路径对比
    if res_bfs and res_dij:
        steps, path = res_bfs
        cost, path2 = res_dij
        bfs_cost = sum(COST[maze[r][c]] for r, c in path)   # BFS 路径的真实代价
        print(f"对比: BFS 最短步数 {steps}(路径代价 {bfs_cost}) vs "
              f"Dijkstra 最低代价路径步数 {len(path2)-1}(代价 {cost})")


if __name__ == "__main__":
    main()
