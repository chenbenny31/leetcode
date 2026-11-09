# LC 785 Is Graph Bipartite? (Medium). Pattern: BFS 2-coloring.
# graph[u] = neighbors of node u (undirected, maybe disconnected). Can the nodes be
# split into two sets so every edge joins nodes from different sets?

from collections import deque


# BFS from every uncolored node; neighbors get the other color. T: O(V+E), S: O(V).
def is_bipartite(graph):
    n = len(graph)
    # color[u] = 0 / 1, -1 = not visited yet
    color = [-1] * n
    for start in range(n):
        if color[start] != -1:
            continue
        color[start] = 0
        # colored nodes whose neighbors are unchecked
        q = deque([start])
        while q:
            cur = q.popleft()
            for nxt in graph[cur]:
                if color[nxt] == -1:
                    color[nxt] = 1 - color[cur]
                    q.append(nxt)
                elif color[nxt] == color[cur]:
                    return False
    return True


def test(label, graph, want):
    print(label, "PASS" if is_bipartite(graph) == want else "FAIL")


def main():
    test("square", [[1, 3], [0, 2], [1, 3], [0, 2]], True)
    test("triangle", [[1, 2], [0, 2], [0, 1]], False)
    test("odd-far", [[1], [0], [3, 4], [2, 4], [2, 3]], False)
    test("isolated", [[]], True)


main()

# follow-up: edge-list input / dislike pairs (LC 886) -> build graph both ways, same BFS
# follow-up: union-find -> for each u, union all its neighbors; u in the same set as a
#   neighbor = not bipartite, O(E * alpha)
