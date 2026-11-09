# LC 207 Course Schedule (Medium). Pattern: topological sort (Kahn's BFS).
# numCourses courses 0..n-1, prerequisites [a, b] = take b before a. Can all courses be
# finished? LC 210: return one valid order, or [] if impossible.

from collections import deque


# Kahn's BFS: keep taking courses with indegree 0; a cycle leaves some never taken.
# T: O(V+E), S: O(V+E).
def can_finish(n, prerequisites):
    # graph[prereq] = courses it unlocks (edge prereq -> course)
    graph = [[] for _ in range(n)]
    # indeg[c] = prereqs of c not taken yet
    indeg = [0] * n
    for req in prerequisites:
        course = req[0]
        prereq = req[1]
        graph[prereq].append(course)
        indeg[course] += 1
    # courses whose prereqs are all taken
    q = deque()
    for c in range(n):
        if indeg[c] == 0:
            q.append(c)
    taken = 0
    while q:
        cur = q.popleft()
        taken += 1
        for nxt in graph[cur]:
            indeg[nxt] -= 1
            if indeg[nxt] == 0:
                q.append(nxt)
    # untaken courses sit on or behind a cycle
    return taken == n


# LC 210: same Kahn's BFS, recording the pop order. T: O(V+E), S: O(V+E).
def find_order(n, prerequisites):
    # graph[prereq] = courses it unlocks (edge prereq -> course)
    graph = [[] for _ in range(n)]
    # indeg[c] = prereqs of c not taken yet
    indeg = [0] * n
    for req in prerequisites:
        course = req[0]
        prereq = req[1]
        graph[prereq].append(course)
        indeg[course] += 1
    q = deque()
    for c in range(n):
        if indeg[c] == 0:
            q.append(c)
    # courses in the order taken
    order = []
    while q:
        cur = q.popleft()
        order.append(cur)
        for nxt in graph[cur]:
            indeg[nxt] -= 1
            if indeg[nxt] == 0:
                q.append(nxt)
    if len(order) < n:
        return []
    return order


def test(label, n, prerequisites, want):
    ok = can_finish(n, prerequisites) == (len(want) > 0)
    ok = ok and find_order(n, prerequisites) == want
    print(label, "PASS" if ok else "FAIL")


def main():
    test("diamond", 4, [[1, 0], [2, 0], [3, 1], [3, 2]], [0, 1, 2, 3])
    test("cycle", 2, [[1, 0], [0, 1]], [])
    test("partial-cycle", 3, [[1, 0], [2, 1], [1, 2]], [])
    test("no-prereqs", 2, [], [0, 1])


main()

# follow-up: DFS instead of Kahn -> 3-color DFS (0 new, 1 on path, 2 done); reaching a
#   1 = cycle; reversed post-order = a valid order, O(V+E); in Python mind the default
#   recursion limit (1000) on long chains
# follow-up: min semesters, no per-term limit (LC 1136) -> Kahn level by level; answer =
#   number of levels (longest path in the DAG)
