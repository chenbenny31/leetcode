// Merge Trees By Name (Google, interview report): two ordered trees, each node has a
// name, an int value and ordered children; sibling names are unique within a tree. Merge:
// the roots merge (assumed equal names; the first tree's root name is kept), children
// with the same name merge recursively with values added, unmatched children are kept
// whole: first tree's children in order, then the second tree's new names in order.
// Nodes live in a vector pool (index = node id, root = 0); the result is a new pool.
//
// Clarify:
// - tree format -> vector pool, root id 0, children ids in order; both trees non-empty
// - roots with different names? -> assumed equal (virtual-root follow-up)
// - sibling names unique within one tree -> at most one match per child (duplicates are
//   a follow-up)
// - order of merged children -> a's children in a's order, then b's unmatched children
//   in b's order
// - values int; matched values add (assume no overflow, else long long)
// - return a new pool (root id 0) and leave the inputs alone; in place is a follow-up
// - up to ~1e5 nodes; a deep path recurses that deep (explicit-stack follow-up)
// Idea: DFS from both roots at once. Two same-name nodes become one node with the values
//   added; their children are matched by name through a hash map of b's children, so
//   each node is visited once. Unmatched subtrees are copied whole.
// Pitfalls:
// - out grows during the recursion: hold no Node& into out across a call, and never
//   write out[newId].children.push_back(copyTree(...)): C++17 evaluates out[newId]
//   before the argument, and the push_back inside copyTree can move the pool. Store the
//   new id in a variable first.
// - skip b's children that were already merged (aNames), or they are copied again
// - the output order comes from the children vectors, never from the hash map
// - copy the whole unmatched subtree, not just its top node
// Tests to write: a matched child adds values; order = a's children, then b's new ones
//   (one with its own subtree); a merge two levels deep; b is a bare root.

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

// follow-up "duplicate sibling names": the i-th child of x named s merges with the i-th
// child of y named s; the rest are copied. Same DFS, but the name map holds a list of
// ids and a count of the ones used. Returns the merged node's new id.
// T: O(|a| + |b|), S: O(|a| + |b|).
int mergeNodesDup(const vector<Node>& a, int x, const vector<Node>& b, int y,
                  vector<Node>& out) {
    int newId = out.size();
    out.push_back({a[x].name, a[x].value + b[y].value, {}});
    // bChild[name] = ids of y's children with that name, in order
    unordered_map<string, vector<int>> bChild;
    for (int child : b[y].children) { bChild[b[child].name].push_back(child); }
    // matched[name] = how many of them are merged so far
    unordered_map<string, int> matched;
    for (int child : a[x].children) {
        const string& name = a[child].name;
        int k = matched[name];
        int newChild = -1;
        if (k < (int)bChild[name].size()) {
            matched[name]++;
            newChild = mergeNodesDup(a, child, b, bChild[name][k], out);
        } else {
            newChild = copyTree(a, child, out);
        }
        out[newId].children.push_back(newChild);
    }
    // seen[name] = y's children with that name passed so far; the first matched[name]
    // of them are merged already
    unordered_map<string, int> seen;
    for (int child : b[y].children) {
        const string& name = b[child].name;
        seen[name]++;
        if (seen[name] <= matched[name]) { continue; }
        int newChild = copyTree(b, child, out);
        out[newId].children.push_back(newChild);
    }
    return newId;
}

// follow-up "merge in place into a": add b's values into the matching nodes of a and
// append b-only subtrees to a's pool; a is never copied. T: O(|a| + |b|), S: O(|b| + h).
// Pitfall: copyTree(b, child, a) grows a, so store the new id before touching a[x].
void mergeInto(vector<Node>& a, int x, const vector<Node>& b, int y) {
    a[x].value += b[y].value;
    // aChild[name] = id of x's child with that name
    unordered_map<string, int> aChild;
    for (int child : a[x].children) { aChild[a[child].name] = child; }
    for (int child : b[y].children) {
        auto it = aChild.find(b[child].name);
        if (it != aChild.end()) {
            mergeInto(a, it->second, b, child);
            continue;
        }
        int newChild = copyTree(b, child, a);
        a[x].children.push_back(newChild);
    }
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
    // with unique names the duplicate-aware merge must agree
    vector<Node> dupOut;
    mergeNodesDup(a, 0, b, 0, dupOut);
    vector<tuple<int, string, int>> gotDup;
    preorder(dupOut, 0, 0, gotDup);
    vector<Node> inPlace = a;
    mergeInto(inPlace, 0, b, 0);
    vector<tuple<int, string, int>> gotInPlace;
    preorder(inPlace, 0, 0, gotInPlace);
    bool ok = got == want && gotDup == want && gotInPlace == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void testDup(const string& label, const vector<Node>& a, const vector<Node>& b,
             const vector<tuple<int, string, int>>& want) {
    vector<Node> out;
    mergeNodesDup(a, 0, b, 0, out);
    vector<tuple<int, string, int>> got;
    preorder(out, 0, 0, got);
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

    testDup("dup-pairs", {{"/", 0, {1, 2}}, {"x", 1, {}}, {"x", 2, {}}},
            {{"/", 0, {1, 2, 3}}, {"x", 10, {}}, {"x", 20, {}}, {"x", 30, {}}},
            {{0, "/", 0}, {1, "x", 11}, {1, "x", 22}, {1, "x", 30}});
    testDup("dup-more-in-a", {{"/", 0, {1, 2}}, {"x", 1, {}}, {"x", 2, {}}},
            {{"/", 0, {1}}, {"x", 5, {}}},
            {{0, "/", 0}, {1, "x", 6}, {1, "x", 2}});
    testDup("dup-mixed", {{"/", 0, {1, 2, 3}}, {"x", 1, {}}, {"y", 2, {}}, {"x", 3, {}}},
            {{"/", 0, {1, 2, 3, 4}}, {"x", 10, {}}, {"z", 5, {}}, {"x", 20, {}},
             {"x", 7, {}}},
            {{0, "/", 0}, {1, "x", 11}, {1, "y", 2}, {1, "x", 23}, {1, "z", 5},
             {1, "x", 7}});
}

// follow-up: root names differ -> a virtual root with both trees as its children:
//     out.push_back({"", 0, {}});
//     int x = copyTree(a, 0, out);
//     int y = copyTree(b, 0, out);
//     out[0].children = {x, y};
// follow-up: duplicate sibling names (i-th meets i-th) -> mergeNodesDup above
// follow-up: merge in place into a (no copy) -> mergeInto above
// follow-up: very deep trees -> mergeInto with an explicit stack of (x, y) pairs:
//     vector<pair<int, int>> todo = {{0, 0}};
//   pop (x, y), add the value, build aChild; a matched child pushes {aId, child} instead
//   of recursing; copyTree gets its own stack of (from id, new parent id)
