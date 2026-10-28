// Hubs 0..n-1 sit on a ring. cost[i] is the cost of the edge between hub i and hub
// (i + 1) % n, in either direction. A drone starts at hubs[0] and visits hubs in order,
// taking the cheaper direction each time. Return the minimum total cost.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::min;

// clockwise cost from a to b by walking the edges one by one
long long walkCw(const vector<int>& cost, int a, int b) {
    int n = cost.size();
    long long sum = 0;
    for (int i = a; i != b; i = (i + 1) % n) { sum += cost[i]; }
    return sum;
}

// straightforward: walk both directions for every leg (counter-clockwise a -> b uses the
// same edges as clockwise b -> a). T: O(t * n), S: O(1).
long long minCost(const vector<int>& hubs, const vector<int>& cost) {
    long long total = 0;
    int t = hubs.size();
    for (int i = 1; i < t; i++) {
        int a = hubs[i - 1], b = hubs[i];
        total += min(walkCw(cost, a, b), walkCw(cost, b, a));
    }
    return total;
}

// clockwise cost from a to b using prefix sums (a == b costs 0)
long long arcCw(const vector<long long>& prefix, int a, int b) {
    long long ring = prefix.back();
    if (a <= b) { return prefix[b] - prefix[a]; }
    return ring - (prefix[a] - prefix[b]);
}

// improved: prefix sums give any clockwise arc in O(1). T: O(n + t), S: O(n).
long long minCostPrefix(const vector<int>& hubs, const vector<int>& cost) {
    int n = cost.size();
    // prefix[i] = cost[0] + ... + cost[i - 1]
    vector<long long> prefix(n + 1, 0);
    for (int i = 0; i < n; i++) { prefix[i + 1] = prefix[i] + cost[i]; }
    long long total = 0;
    int t = hubs.size();
    for (int i = 1; i < t; i++) {
        int a = hubs[i - 1], b = hubs[i];
        total += min(arcCw(prefix, a, b), arcCw(prefix, b, a));
    }
    return total;
}

void test(const string& label, const vector<int>& hubs, const vector<int>& cost,
          long long want) {
    bool ok = minCost(hubs, cost) == want && minCostPrefix(hubs, cost) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("cw-cheap", {0, 2}, {1, 1, 10}, 2);
    test("ccw-cheap", {0, 2}, {5, 5, 1}, 1);
    test("back", {2, 0}, {1, 1, 10}, 2);
    test("same-hub", {1, 1}, {3, 4}, 0);
    test("one-hub", {1}, {3, 4}, 0);
}
