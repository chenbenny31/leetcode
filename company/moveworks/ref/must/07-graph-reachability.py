# 07 · Reachable nodes (graph basics drill, 2026-11-27, Medium) — directed reachability
# edges = list of (u, v) pairs, each a directed edge u -> v; ids are any ints
# return every node reachable from start, in any order
#
# clarify:
# start itself -> counts as reachable (ask first: it decides every expected value)
# start in no edge -> still returns [start] (ask; the other reading returns [])
# cycles, self loops, repeated edges -> allowed; each node is returned once
# node ids -> any int, not 0 .. n - 1, so the graph is a dict, not a list
# order -> any, so the test sorts both sides before it compares


# core idea: adjacency dict, then a DFS with an explicit stack from start; a node is
# marked when it is pushed, so it is pushed once
# T: O(V + E), S: O(V + E)
def reachable(edges, start):
    # graph[u] = nodes v with an edge u -> v
    graph = {}
    for u, v in edges:
        if u not in graph:
            graph[u] = []
        graph[u].append(v)

    result = []
    seen = {start}
    # nodes seen but not expanded yet
    pending = [start]
    while pending:
        cur = pending.pop()
        result.append(cur)
        # .get, not []: a node with no out-edges has no entry, and that is fine
        for nxt in graph.get(cur, []):
            if nxt in seen:
                continue
            seen.add(nxt)
            pending.append(nxt)
    return result


def test(label, edges, start, want):
    ok = sorted(reachable(edges, start)) == sorted(want)
    print(label, "PASS" if ok else "FAIL")


def main():
    test("chain-from-head", [(1, 2), (2, 3)], 1, [1, 2, 3])
    test("sink-only-itself", [(1, 2)], 2, [2])
    test("cycle-and-negative-id", [(1, -2), (-2, 1), (5, 1)], 1, [1, -2])
    test("start-in-no-edge", [(1, 2)], 9, [9])


main()

# follow-up:
# 1. recursive DFS -> shorter, but a long chain hits Python's recursion limit (1000)
# 2. hop count to each node -> BFS with collections.deque, dist[nxt] = dist[cur] + 1
# 3. nodes that can reach start -> build the reversed graph, same search
# 4. start in no edge must return [] -> track whether start appears while building
# 5. undirected edges -> add both directions; then union-find works too
