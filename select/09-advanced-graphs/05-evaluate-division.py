# LC 399 Evaluate Division (Medium). Pattern: weighted graph DFS, multiply along path.
# equations a / b = v. For each query x / y return its value, or -1.0 if it cannot be
# determined (unknown variable or no chain linking x and y).

from collections import defaultdict


# returns cur / target along some path, or -1.0 if target is unreachable
def dfs(graph, cur, target, seen):
    if cur == target:
        return 1.0
    seen.add(cur)
    for nxt, ratio in graph[cur]:
        if nxt in seen:
            continue
        rest = dfs(graph, nxt, target, seen)
        if rest >= 0:
            return ratio * rest
    return -1.0


# Edge num -> den weighs num / den (den -> num weighs the inverse); DFS per query.
# T: O(Q * (V+E)), S: O(V+E).
def calc_equation(equations, values, queries):
    # graph[num] = (den, num / den) for every known ratio, both directions
    graph = defaultdict(list)
    for i in range(len(equations)):
        num = equations[i][0]
        den = equations[i][1]
        graph[num].append((den, values[i]))
        graph[den].append((num, 1.0 / values[i]))
    result = []
    for query in queries:
        num = query[0]
        den = query[1]
        # x / x = 1 only for a known x; an unknown variable has no value at all
        if num not in graph or den not in graph:
            result.append(-1.0)
            continue
        # variables visited in this query's DFS
        seen = set()
        result.append(dfs(graph, num, den, seen))
    return result


def test(label, equations, values, queries, want):
    got = calc_equation(equations, values, queries)
    ok = len(got) == len(want)
    for i in range(len(want)):
        if ok and abs(got[i] - want[i]) >= 1e-9:
            ok = False
    print(label, "PASS" if ok else "FAIL")


def main():
    test("chain", [["x", "y"], ["y", "z"]], [2.0, 4.0], [["x", "z"], ["z", "x"]],
         [8.0, 0.125])
    test("two-groups", [["x", "y"], ["p", "q"]], [2.0, 5.0], [["x", "q"], ["q", "p"]],
         [-1.0, 0.2])
    test("unknown-var", [["x", "y"]], [3.0], [["x", "x"], ["w", "w"], ["x", "w"]],
         [1.0, -1.0, -1.0])
    test("join-late", [["a", "b"], ["c", "d"], ["b", "d"]], [2.0, 3.0, 5.0],
         [["a", "c"]], [10.0 / 3.0])


main()

# follow-up: many queries -> weighted union-find, weight[x] = x / root; x / y =
#   weight[x] / weight[y] when same root, ~O(alpha) per query
# follow-up: dense, fixed variables -> Floyd-Warshall on ratios, O(V^3) then O(1)
