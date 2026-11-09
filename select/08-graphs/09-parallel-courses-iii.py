# LC 2050 Parallel Courses III (Hard). Pattern: topological order + longest path (DAG).
# Courses 1..n, relations [prev, next] = prev must finish before next starts, course i
# takes time[i-1] months; any number run at once. Return the min months to finish all.

from collections import deque


# Kahn's order; a course starts when its last prereq finishes. T: O(V+E), S: O(V+E).
def minimum_time(n, relations, time):
    # graph[u] = courses that wait for u (0-based)
    graph = [[] for _ in range(n)]
    # indeg[v] = prereqs of v not finished yet
    indeg = [0] * n
    for rel in relations:
        before = rel[0] - 1
        after = rel[1] - 1
        graph[before].append(after)
        indeg[after] += 1
    # start[v] = latest finish among v's prereqs
    start = [0] * n
    q = deque()
    for v in range(n):
        if indeg[v] == 0:
            q.append(v)
    total = 0
    while q:
        cur = q.popleft()
        finish = start[cur] + time[cur]
        total = max(total, finish)
        for nxt in graph[cur]:
            start[nxt] = max(start[nxt], finish)
            indeg[nxt] -= 1
            if indeg[nxt] == 0:
                q.append(nxt)
    return total


def test(label, n, relations, time, want):
    print(label, "PASS" if minimum_time(n, relations, time) == want else "FAIL")


def main():
    test("diamond", 4, [[1, 2], [1, 3], [2, 4], [3, 4]], [1, 5, 2, 1], 7)
    test("chain", 3, [[1, 2], [2, 3]], [1, 2, 3], 6)
    test("independent", 2, [], [5, 1], 5)


main()

# follow-up: every course takes 1 unit (LC 1136) -> Kahn level by level, count levels;
#   return -1 if a cycle leaves courses untaken
# follow-up: critical courses -> parent[v] = prereq that set start[v]; walk back
