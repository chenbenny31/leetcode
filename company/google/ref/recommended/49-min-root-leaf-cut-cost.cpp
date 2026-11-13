// Min Cost to Cut the Root From All Leaves (Google, interview report)
// binary tree, root 0, child edges w/ weight >= 0; cut edges so no leaf reaches the root
// return min total cut weight; root only -> 0
//
// clarify:
// tree format -> vector pool, child id -1 if none
// one-child root -> not a leaf
// weight range -> sums in long long
// n, depth -> recursion ok; iterative version below
// return -> total; the cut edges in minCutEdges

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <climits>
#include <queue>

using std::cout;
using std::string;
using std::vector;
using std::min;
using std::queue;

struct Node {
    // child index, -1 if none
    int left;
    int leftWeight;
    int right;
    int rightWeight;
};

// a leaf cannot be cut from itself -> cost "infinite"
const long long INF = LLONG_MAX / 4;

// min weight to cut node off from every leaf below it (INF for a leaf)
long long dfs(const vector<Node>& tree, int node) {
    const Node& cur = tree[node];
    if (cur.left == -1 && cur.right == -1) { return INF; }
    long long cost = 0;
    if (cur.left != -1) {
        cost += min((long long)cur.leftWeight, dfs(tree, cur.left));
    }
    if (cur.right != -1) {
        cost += min((long long)cur.rightWeight, dfs(tree, cur.right));
    }
    return cost;
}

// core idea: tree DP, cost(node) = sum over children of min(edge weight, cost(child))
// leaf costs INF, which forces a cut somewhere above it
// T: O(n), S: O(h)
long long minCutCost(const vector<Node>& tree) {
    const Node& root = tree[0];
    if (root.left == -1 && root.right == -1) { return 0; }
    return dfs(tree, 0);
}

// follow-up, coded: return the cut edges (child id below each cut), no recursion
// core idea: reverse BFS order fills cost[]; from root, cut child edge if weight <= cost
// T: O(n), S: O(n)
vector<int> minCutEdges(const vector<Node>& tree) {
    // node ids reachable from root, parents before children
    vector<int> order = {0};
    for (int i = 0; i < (int)order.size(); i++) {
        const Node& cur = tree[order[i]];
        if (cur.left != -1) { order.push_back(cur.left); }
        if (cur.right != -1) { order.push_back(cur.right); }
    }
    // cost[id] = min weight to cut id off from every leaf below it, INF for a leaf
    vector<long long> cost(tree.size(), 0);
    for (int i = (int)order.size() - 1; i >= 0; i--) {
        int id = order[i];
        const Node& cur = tree[id];
        if (cur.left == -1 && cur.right == -1) {
            cost[id] = INF;
            continue;
        }
        if (cur.left != -1) {
            cost[id] += min((long long)cur.leftWeight, cost[cur.left]);
        }
        if (cur.right != -1) {
            cost[id] += min((long long)cur.rightWeight, cost[cur.right]);
        }
    }
    vector<int> cuts;
    // nodes still connected to the root
    queue<int> q;
    q.push(0);
    while (!q.empty()) {
        int id = q.front();
        q.pop();
        const Node& cur = tree[id];
        if (cur.left != -1 && cur.leftWeight <= cost[cur.left]) {
            cuts.push_back(cur.left);
        } else if (cur.left != -1) {
            q.push(cur.left);
        }
        if (cur.right != -1 && cur.rightWeight <= cost[cur.right]) {
            cuts.push_back(cur.right);
        } else if (cur.right != -1) {
            q.push(cur.right);
        }
    }
    return cuts;
}

void test(const string& label, const vector<Node>& tree, long long want) {
    cout << label << (minCutCost(tree) == want ? " PASS\n" : " FAIL\n");
}

void testCuts(const string& label, const vector<Node>& tree, const vector<int>& want) {
    cout << label << (minCutEdges(tree) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("root-only", {{-1, 0, -1, 0}}, 0);
    test("cut-below",
         {{1, 10, 2, 4}, {3, 2, 4, 3}, {-1, 0, -1, 0}, {-1, 0, -1, 0}, {-1, 0, -1, 0}},
         9);
    test("cut-above",
         {{1, 1, -1, 0}, {2, 5, 3, 5}, {-1, 0, -1, 0}, {-1, 0, -1, 0}}, 1);
    test("chain", {{1, 3, -1, 0}, {-1, 0, 2, 2}, {3, 7, -1, 0}, {-1, 0, -1, 0}}, 2);

    testCuts("cuts-root-only", {{-1, 0, -1, 0}}, {});
    testCuts("cuts-below",
             {{1, 10, 2, 4}, {3, 2, 4, 3}, {-1, 0, -1, 0}, {-1, 0, -1, 0},
              {-1, 0, -1, 0}},
             {2, 3, 4});
    testCuts("cuts-above",
             {{1, 1, -1, 0}, {2, 5, 3, 5}, {-1, 0, -1, 0}, {-1, 0, -1, 0}}, {1});
    testCuts("cuts-tie",
             {{1, 5, -1, 0}, {2, 2, 3, 3}, {-1, 0, -1, 0}, {-1, 0, -1, 0}}, {1});
}

// follow-up:
// n-ary tree -> children as (child, weight) pairs; same sum of min(w, cost(child))
// only some leaves must be cut off -> a leaf that may stay returns 0 instead of INF
// very deep tree -> reverse-BFS-order pass of minCutEdges (no recursion)
// general graph -> min s-t cut (max flow) w/ every leaf joined to one sink
// negative weights -> cut every negative edge; answer = their sum + DP w/ them adding 0
