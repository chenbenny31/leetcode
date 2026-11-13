// Boolean Expression Tree Leaf Flips (Google, interview report)
// vector pool (idx = id): AND / OR / XOR (2 children), NOT (1 child, left), or leaf "VAL"
// (1) per leaf, left to right: flip it alone (undone after), record root value
// (2) cumulative flips (flips[i] = leaf id), root value after each; no parent pointers
//
// clarify:
// tree well-formed -> yes
// output order -> leaves left to right
// root value or "did it change" -> root value
// leaf flipped twice in (2) -> back to its original value
// n, depth -> recursion ok; else reverse BFS order

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::pair;
using std::min;

struct Node {
    // "AND", "OR", "XOR", "NOT" or "VAL" (leaf)
    string op;
    // child ids, -1 if absent (NOT uses only left)
    int left = -1;
    int right = -1;
    // leaf value, unused for operators
    bool value = false;
};

// operator of node applied to its children's values in val
bool combine(const Node& node, const vector<bool>& val) {
    if (node.op == "NOT") { return !val[node.left]; }
    bool a = val[node.left];
    bool b = val[node.right];
    if (node.op == "AND") { return a && b; }
    if (node.op == "OR") { return a || b; }
    return a != b;
}

// post-order: val[id] = value of subtree at id; returns val[id]
bool computeValues(const vector<Node>& tree, int id, vector<bool>& val) {
    const Node& node = tree[id];
    if (node.op == "VAL") {
        val[id] = node.value;
        return val[id];
    }
    computeValues(tree, node.left, val);
    if (node.right != -1) { computeValues(tree, node.right, val); }
    val[id] = combine(node, val);
    return val[id];
}

void collectLeaves(const vector<Node>& tree, int id, vector<int>& leaves) {
    const Node& node = tree[id];
    if (node.op == "VAL") {
        leaves.push_back(id);
        return;
    }
    collectLeaves(tree, node.left, leaves);
    if (node.right != -1) { collectLeaves(tree, node.right, leaves); }
}

// (1) brute force
// core idea: flip each leaf, re-evaluate the whole tree, flip back
// T: O(n^2), S: O(n)
vector<bool> flipEachNaive(vector<Node> tree, int root) {
    vector<int> leaves;
    collectLeaves(tree, root, leaves);
    vector<bool> val(tree.size());
    vector<bool> result;
    for (int leaf : leaves) {
        tree[leaf].value = !tree[leaf].value;
        result.push_back(computeValues(tree, root, val));
        tree[leaf].value = !tree[leaf].value;
    }
    return result;
}

// matters = flipping this subtree's value flips the root; a leaf appends its answer
void spread(const vector<Node>& tree, int id, const vector<bool>& val, bool matters,
            bool rootValue, vector<bool>& result) {
    const Node& node = tree[id];
    if (node.op == "VAL") {
        result.push_back(matters ? !rootValue : rootValue);
        return;
    }
    if (node.op == "NOT") {
        spread(tree, node.left, val, matters, rootValue, result);
        return;
    }
    bool toLeft = matters;
    bool toRight = matters;
    if (node.op == "AND") {
        toLeft = matters && val[node.right];
        toRight = matters && val[node.left];
    }
    if (node.op == "OR") {
        toLeft = matters && !val[node.right];
        toRight = matters && !val[node.left];
    }
    spread(tree, node.left, val, toLeft, rootValue, result);
    spread(tree, node.right, val, toRight, rootValue, result);
}

// (1) one pass
// core idea: values bottom-up, then push "matters" (a flip here flips the root) top-down
// XOR / NOT always pass it on; AND only if sibling is true; OR only if sibling is false
// T: O(n), S: O(n)
vector<bool> flipEach(const vector<Node>& tree, int root) {
    // val[id] = value of subtree at id
    vector<bool> val(tree.size());
    bool rootValue = computeValues(tree, root, val);
    vector<bool> result;
    spread(tree, root, val, true, rootValue, result);
    return result;
}

// (2) cumulative flips
// core idea: cache every node's value; a flip recomputes ancestors via a parent vector
// stop at the first value that does not change
// T: O(n + q * h), S: O(n)
vector<bool> rootAfterFlips(const vector<Node>& tree, int root,
                            const vector<int>& flips) {
    int n = tree.size();
    // val[id] = current value of subtree at id
    vector<bool> val(n);
    computeValues(tree, root, val);
    // parent[id] = parent node id, -1 for the root
    vector<int> parent(n, -1);
    for (int id = 0; id < n; id++) {
        if (tree[id].left != -1) { parent[tree[id].left] = id; }
        if (tree[id].right != -1) { parent[tree[id].right] = id; }
    }
    vector<bool> result;
    for (int leaf : flips) {
        val[leaf] = !val[leaf];
        int cur = parent[leaf];
        while (cur != -1) {
            bool updated = combine(tree[cur], val);
            if (updated == val[cur]) { break; }
            val[cur] = updated;
            cur = parent[cur];
        }
        result.push_back(val[root]);
    }
    return result;
}

// {fewest leaf flips to make subtree at id false, fewest to make it true}
pair<int, int> dfsFlips(const vector<Node>& tree, int id) {
    const Node& node = tree[id];
    if (node.op == "VAL") { return {node.value ? 1 : 0, node.value ? 0 : 1}; }
    auto [leftF, leftT] = dfsFlips(tree, node.left);
    if (node.op == "NOT") { return {leftT, leftF}; }
    auto [rightF, rightT] = dfsFlips(tree, node.right);
    if (node.op == "AND") {
        return {min(leftF + min(rightF, rightT), leftT + rightF), leftT + rightT};
    }
    if (node.op == "OR") {
        return {leftF + rightF, min(leftT + min(rightF, rightT), leftF + rightT)};
    }
    return {min(leftF + rightF, leftT + rightT), min(leftF + rightT, leftT + rightF)};
}

// follow-up, coded (LC 2313): fewest leaf flips to make the root equal target
// core idea: tree DP, each node returns cost to become false / true
// an operator takes the cheapest pair of child results for each value
// T: O(n), S: O(h)
int minFlipsToResult(const vector<Node>& tree, int root, bool target) {
    auto [toFalse, toTrue] = dfsFlips(tree, root);
    return target ? toTrue : toFalse;
}

void test(const string& label, const vector<Node>& tree, int root,
          const vector<bool>& want) {
    bool ok = flipEachNaive(tree, root) == want && flipEach(tree, root) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void testFlips(const string& label, const vector<Node>& tree, int root,
               const vector<int>& flips, const vector<bool>& want) {
    cout << label << (rootAfterFlips(tree, root, flips) == want ? " PASS\n" : " FAIL\n");
}

void testMinFlips(const string& label, const vector<Node>& tree, int root, bool target,
                  int want) {
    bool ok = minFlipsToResult(tree, root, target) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    vector<Node> andTree = {{"AND", 1, 2}, {"VAL", -1, -1, true}, {"VAL", -1, -1, false}};
    vector<Node> mixed = {{"AND", 1, 4}, {"OR", 2, 3}, {"VAL", -1, -1, true},
                          {"VAL", -1, -1, false}, {"XOR", 5, 6}, {"VAL", -1, -1, true},
                          {"VAL", -1, -1, false}};
    vector<Node> notTree = {{"XOR", 1, 3}, {"NOT", 2}, {"VAL", -1, -1, true},
                            {"VAL", -1, -1, false}};
    vector<Node> leaf = {{"VAL", -1, -1, true}};

    test("and", andTree, 0, {false, true});
    test("mixed", mixed, 0, {false, true, false, false});
    test("xor-not", notTree, 0, {true, true});
    test("single-leaf", leaf, 0, {false});

    testFlips("cumulative", andTree, 0, {2, 1, 2}, {true, false, false});
    testFlips("early-stop", mixed, 0, {3, 6, 2, 5}, {true, false, false, true});
    testFlips("not", notTree, 0, {2, 2}, {true, false});

    vector<Node> bothFalse = {{"AND", 1, 2}, {"VAL", -1, -1, false},
                              {"VAL", -1, -1, false}};
    vector<Node> orCheapSide = {{"OR", 1, 4}, {"AND", 2, 3}, {"VAL", -1, -1, false},
                                {"VAL", -1, -1, false}, {"VAL", -1, -1, false}};
    testMinFlips("min-and-true", andTree, 0, true, 1);
    testMinFlips("min-mixed-false", mixed, 0, false, 1);
    testMinFlips("min-two-flips", bothFalse, 0, true, 2);
    testMinFlips("min-or-cheap-side", orCheapSide, 0, true, 1);
}

// follow-up:
// "segment-tree style" design -> rootAfterFlips as a class: build, update(leaf), query()
// very deep tree -> fill val w/o recursion in reverse BFS order (children first)
// (1) as single-leaf queries -> store matters[id]; matters[leaf] ? !rootValue : rootValue
