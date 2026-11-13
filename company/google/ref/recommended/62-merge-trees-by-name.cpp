// Merge Trees By Name (Google, interview report)
// (1) two ordered trees in vector pools; same-name children merge recursively, values add
// (2) dup sibling names
// (3) merge in place into a
//
// clarify:
// roots w/ different names -> assumed equal
// dup sibling names -> assumed unique; follow-up (2)
// order of merged children -> a's children in order, then b's new names
// new pool or in place -> new pool; follow-up (3)
// node cnt and depth -> recursion depth

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

// copies the subtree at id of `from` into out; returns its new id
int copyTree(const vector<Node>& from, int id, vector<Node>& out) {
    int newId = out.size();
    out.push_back({from[id].name, from[id].value, {}});
    for (int child : from[id].children) {
        int newChild = copyTree(from, child, out);
        out[newId].children.push_back(newChild);
    }
    return newId;
}

// merges x of a w/ y of b into out, returns the new id; out may reallocate during a call
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

// (1) merge into a new pool
// core idea: DFS both roots at once; two same-name nodes -> one node w/ values added
// children matched by name via hash map of b's children; unmatched subtrees copied whole
// T: O(|a| + |b|), S: O(|a| + |b|)
vector<Node> mergeTrees(const vector<Node>& a, const vector<Node>& b) {
    vector<Node> out;
    mergeNodes(a, 0, b, 0, out);
    return out;
}

// (2) dup sibling names
// core idea: i-th child of x named s merges w/ i-th child of y named s; the rest copied
// T: O(|a| + |b|), S: O(|a| + |b|)
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
    // seen[name] = y's children w/ that name passed so far (first matched[name] merged)
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

// (3) merge in place into a
// core idea: add b's values into matching nodes of a, append b-only subtrees to a's pool
// copyTree grows a -> store the new id before touching a[x]
// T: O(|a| + |b|), S: O(|b| + h)
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

// follow-up:
// root names differ -> virtual root w/ both (copied) trees as its two children
// very deep trees -> mergeInto w/ explicit stack of (x, y) pairs; copyTree gets its own
