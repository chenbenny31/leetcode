// Merge Trees By Name (Google, interview report): two ordered trees, each node has a
// name, an int value and ordered children; sibling names are unique within a tree. Merge:
// the roots merge (assumed equal names; the first tree's root name is kept), children
// with the same name merge recursively with values added, unmatched children are kept
// whole: first tree's children in order, then the second tree's new names in order.
// Nodes live in a vector pool (index = node id, root = 0); the result is a new pool.

#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <unordered_map>
#include <unordered_set>

using std::cout;
using std::string;
using std::vector;
using std::tuple;
using std::unordered_map;
using std::unordered_set;

struct Node {
    string name;
    int value = 0;
    // child node ids, in order
    vector<int> children;
};

// copy the subtree at id of `from` into out; returns its new id.
// T: O(subtree size), S: O(depth).
int copyTree(const vector<Node>& from, int id, vector<Node>& out) {
    int newId = out.size();
    out.push_back({from[id].name, from[id].value, {}});
    for (int child : from[id].children) {
        int newChild = copyTree(from, child, out);
        out[newId].children.push_back(newChild);
    }
    return newId;
}

// merge node x of a with node y of b into out; returns the merged node's new id.
// Index out[newId] after each recursive call: push_back may move the pool.
int mergeNodes(const vector<Node>& a, int x, const vector<Node>& b, int y,
               vector<Node>& out) {
    int newId = out.size();
    out.push_back({a[x].name, a[x].value + b[y].value, {}});
    // bChild[name] = id of y's child with that name
    unordered_map<string, int> bChild;
    for (int child : b[y].children) { bChild[b[child].name] = child; }
    // names of x's children
    unordered_set<string> aNames;
    for (int child : a[x].children) {
        aNames.insert(a[child].name);
        auto it = bChild.find(a[child].name);
        int newChild = it != bChild.end() ? mergeNodes(a, child, b, it->second, out)
                                          : copyTree(a, child, out);
        out[newId].children.push_back(newChild);
    }
    for (int child : b[y].children) {
        if (aNames.count(b[child].name)) { continue; }
        int newChild = copyTree(b, child, out);
        out[newId].children.push_back(newChild);
    }
    return newId;
}

// DFS from both roots at once. T: O(|a| + |b|), S: O(|a| + |b|).
vector<Node> mergeTrees(const vector<Node>& a, const vector<Node>& b) {
    vector<Node> out;
    mergeNodes(a, 0, b, 0, out);
    return out;
}

// test helper: pre-order (depth, name, value) of the tree at id
void preorder(const vector<Node>& tree, int id, int depth,
              vector<tuple<int, string, int>>& order) {
    order.push_back({depth, tree[id].name, tree[id].value});
    for (int child : tree[id].children) { preorder(tree, child, depth + 1, order); }
}

void test(const string& label, const vector<Node>& a, const vector<Node>& b,
          const vector<tuple<int, string, int>>& want) {
    vector<tuple<int, string, int>> got;
    preorder(mergeTrees(a, b), 0, 0, got);
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("match-adds", {{"/", 1, {1}}, {"docs", 2, {}}},
         {{"/", 1, {1}}, {"docs", 3, {}}},
         {{0, "/", 2}, {1, "docs", 5}});
    test("keep-order", {{"/", 0, {1, 2}}, {"src", 4, {}}, {"docs", 2, {}}},
         {{"/", 0, {1, 2}}, {"docs", 1, {}}, {"bin", 3, {3}}, {"tools", 5, {}}},
         {{0, "/", 0}, {1, "src", 4}, {1, "docs", 3}, {1, "bin", 3}, {2, "tools", 5}});
    test("deep-merge", {{"/", 0, {1}}, {"src", 1, {2}}, {"main", 1, {}}},
         {{"/", 0, {1}}, {"src", 2, {2, 3}}, {"util", 2, {}}, {"main", 4, {}}},
         {{0, "/", 0}, {1, "src", 3}, {2, "main", 5}, {2, "util", 2}});
    test("lone-root", {{"/", 1, {1}}, {"docs", 2, {}}}, {{"/", 0, {}}},
         {{0, "/", 1}, {1, "docs", 2}});
}

// follow-up: root names differ -> add a virtual root whose children are the two roots
// follow-up: duplicate sibling names -> match the i-th occurrence with the i-th one:
//   bChild maps name -> list of ids, consumed front to back
// follow-up: merge in place into a (no copy) -> append b-only subtrees into a's pool and
//   add values into matched a nodes; same recursion
// follow-up: very deep trees -> explicit stack of (x, y, newId) instead of recursion
