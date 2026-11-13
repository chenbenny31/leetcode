// 110 · All Nodes Distance K in Binary Tree — LC 863
// Mother: select/optional/07-trees/08-nodes-distance-k.cpp
// return values of all nodes at distance k from target node, any order
//
// clarify:
// values unique? -> yes, used as graph keys
// target in tree? -> yes
// k beyond the tree -> {}
// output order -> any

#include <algorithm>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using std::cout;
using std::queue;
using std::sort;
using std::string;
using std::unordered_map;
using std::unordered_set;
using std::vector;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x, TreeNode* l = nullptr, TreeNode* r = nullptr)
        : val(x), left(l), right(r) {}
};

// add undirected edge between each node and each of its children
void dfs(TreeNode* node, unordered_map<int, vector<int>>& graph) {
    if (!node) { return; }
    if (node->left) {
        graph[node->val].push_back(node->left->val);
        graph[node->left->val].push_back(node->val);
        dfs(node->left, graph);
    }
    if (node->right) {
        graph[node->val].push_back(node->right->val);
        graph[node->right->val].push_back(node->val);
        dfs(node->right, graph);
    }
}

// core idea: w/ parent edges tree = undirected graph; build by dfs, BFS k levels out
// queue left after k levels = answer
// T: O(n), S: O(n)
vector<int> distanceK(TreeNode* root, TreeNode* target, int k) {
    // graph[v] = values of v's parent and children (edge = tree edge)
    unordered_map<int, vector<int>> graph;
    dfs(root, graph);
    // nodes at the current distance from target
    queue<int> q;
    q.push(target->val);
    // nodes already reached
    unordered_set<int> seen = {target->val};
    for (int dist = 0; dist < k && !q.empty(); dist++) {
        int levelSize = q.size();
        for (int i = 0; i < levelSize; i++) {
            int cur = q.front();
            q.pop();
            for (int nxt : graph[cur]) {
                if (seen.count(nxt)) { continue; }
                seen.insert(nxt);
                q.push(nxt);
            }
        }
    }
    vector<int> out;
    while (!q.empty()) {
        out.push_back(q.front());
        q.pop();
    }
    return out;
}

void test(const string& label, TreeNode* root, TreeNode* target, int k,
          const vector<int>& want) {
    vector<int> got = distanceK(root, target, k);
    sort(got.begin(), got.end());
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    TreeNode n4(4);
    TreeNode n5(5);
    TreeNode n6(6);
    TreeNode n7(7);
    TreeNode n2(2, &n4, &n5);
    TreeNode n3(3, &n6, &n7);
    TreeNode n1(1, &n2, &n3);
    test("k-zero", &n1, &n2, 0, {2});
    test("up-and-down", &n1, &n2, 1, {1, 4, 5});
    test("across-root", &n1, &n4, 3, {3});
    test("too-far", &n1, &n2, 5, {});
}

// follow-up:
// parent map only -> map child -> parent, BFS over left / right / parent w/ seen set
// closest leaf to target (LC 742) -> same graph, BFS until a leaf is popped
