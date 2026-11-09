# LC 743 Network Delay Time (Medium). Pattern: Dijkstra with a min-heap.
# Directed edges times[i] = [u, v, w] on nodes 1..n. A signal starts at node k; return
# the time for all nodes to receive it, or -1 if some never do.

import heapq

INF = float("inf")


# Dijkstra from k; the answer is the farthest shortest distance.
# T: O((V+E) log V), S: O(V+E).
def network_delay_time(times, n, k):
    # graph[u] = (v, w) for each edge u -> v taking w
    graph = [[] for _ in range(n + 1)]
    for edge in times:
        graph[edge[0]].append((edge[1], edge[2]))
    # dist[v] = best known time from k to v
    dist = [INF] * (n + 1)
    # (time, node), earliest first
    heap = [(0, k)]
    dist[k] = 0
    while heap:
        cur_time, cur = heapq.heappop(heap)
        # stale entry: cur was already settled with a smaller time
        if cur_time > dist[cur]:
            continue
        for nxt, delay in graph[cur]:
            nxt_time = cur_time + delay
            if nxt_time < dist[nxt]:
                dist[nxt] = nxt_time
                heapq.heappush(heap, (nxt_time, nxt))
    result = 0
    for v in range(1, n + 1):
        if dist[v] == INF:
            return -1
        result = max(result, dist[v])
    return result


def test(label, times, n, k, want):
    print(label, "PASS" if network_delay_time(times, n, k) == want else "FAIL")


def main():
    test("diamond", [[1, 2, 1], [1, 3, 4], [2, 4, 2], [3, 4, 1]], 4, 1, 4)
    test("shortcut", [[1, 2, 5], [1, 3, 1], [3, 2, 1]], 3, 1, 2)
    test("unreachable", [[1, 2, 1]], 3, 1, -1)
    test("single", [], 1, 1, 0)


main()

# follow-up: negative edge weights -> Bellman-Ford, n-1 rounds relaxing every edge,
#   O(V*E); an n-th round that still relaxes = negative cycle
# follow-up: dense graph -> O(V^2) array Dijkstra (pick min unvisited each round)
