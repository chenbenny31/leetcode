# LC 1584 Min Cost to Connect All Points (Medium). Pattern: MST, Prim O(n^2).
# points[i] = [x, y]; connecting two points costs their Manhattan distance. Return the
# minimum total cost to connect all points (a minimum spanning tree).

INF = float("inf")


# Prim on the complete graph: keep adding the point cheapest to attach to the tree.
# Dense graph, so the array version beats a heap. T: O(n^2), S: O(n).
def min_cost_connect_points(points):
    n = len(points)
    # best[i] = cheapest edge from i into the tree so far
    best = [INF] * n
    # in_tree[i] = point i already attached
    in_tree = [False] * n
    best[0] = 0
    total = 0
    for _ in range(n):
        cur = -1
        for i in range(n):
            if not in_tree[i] and (cur == -1 or best[i] < best[cur]):
                cur = i
        in_tree[cur] = True
        total += best[cur]
        for i in range(n):
            if in_tree[i]:
                continue
            dx = abs(points[cur][0] - points[i][0])
            dy = abs(points[cur][1] - points[i][1])
            best[i] = min(best[i], dx + dy)
    return total


def test(label, points, want):
    print(label, "PASS" if min_cost_connect_points(points) == want else "FAIL")


def main():
    test("line", [[0, 0], [1, 0], [3, 0]], 3)
    test("square", [[0, 0], [0, 2], [2, 0], [2, 2]], 6)
    test("negative-coords", [[-1, -1], [1, 1], [0, 0]], 4)
    test("single", [[5, 5]], 0)


main()

# follow-up: Kruskal -> sort all n^2 / 2 edges, add if DSU find(u) != find(v), stop at
#   n-1 edges; O(n^2 log n)
# follow-up: sparse graph -> Prim with a heapq of (cost, node), O(E log V)
