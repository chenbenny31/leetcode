# LC 787 Cheapest Flights Within K Stops (Medium). Pattern: Bellman-Ford, k+1 rounds.
# n cities, flights[i] = [from, to, price]. Return the cheapest price from src to dst
# with at most k stops, or -1 if there is no such route.

INF = float("inf")


# Bellman-Ford with k+1 rounds (k stops = k+1 edges). T: O(k*E), S: O(V).
def find_cheapest_price(n, flights, src, dst, k):
    # after r rounds, dist[city] = cheapest price to city using at most r flights
    dist = [INF] * n
    dist[src] = 0
    for _ in range(k + 1):
        # relax from last round's copy so a path grows by at most one edge per round
        prev = list(dist)
        for flight in flights:
            # "from" is a Python keyword
            from_city = flight[0]
            to_city = flight[1]
            price = flight[2]
            if prev[from_city] == INF:
                continue
            dist[to_city] = min(dist[to_city], prev[from_city] + price)
    if dist[dst] == INF:
        return -1
    return dist[dst]


def test(label, n, flights, src, dst, k, want):
    ok = find_cheapest_price(n, flights, src, dst, k) == want
    print(label, "PASS" if ok else "FAIL")


def main():
    test("one-stop", 3, [[0, 1, 100], [1, 2, 100], [0, 2, 500]], 0, 2, 1, 200)
    test("zero-stops", 3, [[0, 1, 100], [1, 2, 100], [0, 2, 500]], 0, 2, 0, 500)
    test("copy-matters", 4, [[0, 1, 1], [1, 2, 1], [2, 3, 1], [0, 3, 10]], 0, 3, 1, 10)
    test("unreachable", 2, [], 0, 1, 1, -1)


main()

# follow-up: why the copy -> without prev, one round can chain several edges in edge
#   order and use more than k+1 flights
# follow-up: Dijkstra variant -> heap of (cost, city, stops); skip when stops > k; keep
#   best stops per city to prune, O(k * E log(k*V))
