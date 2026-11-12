// Boolean Expression Tree Leaf Flips (Google, interview report). Nodes live in a vector
// pool (index = node id); an internal node is AND / OR / XOR (two children) or NOT (one
// child, `left`); a leaf ("VAL") holds true / false.
// Part 1: for each leaf, left to right, flip it alone and record the root value (each
//   flip is undone before the next).
// Part 2: flips are cumulative (flips[i] = a leaf id); return the root value after each.
//   Nodes may not store parent pointers, so parents are kept in a separate vector.

#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::string;
using std::vector;

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

// post-order: val[id] = value of the subtree at id. Returns val[id].
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

// part 1, straightforward: flip each leaf, re-evaluate the whole tree, flip back.
// T: O(n^2), S: O(n).
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

// pre-order, left before right: matters = flipping this subtree's value flips the root.
// XOR / NOT pass it to children; AND passes it to a child only if the other child is
// true, OR only if the other child is false. A leaf appends the root after its flip.
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

// part 1, improved: values bottom-up, then "matters" top-down. T: O(n), S: O(n).
vector<bool> flipEach(const vector<Node>& tree, int root) {
    // val[id] = value of the subtree at id
    vector<bool> val(tree.size());
    bool rootValue = computeValues(tree, root, val);
    vector<bool> result;
    spread(tree, root, val, true, rootValue, result);
    return result;
}

// part 2: cache every node's value; a flip recomputes ancestors via the parent vector and
// stops early once a value does not change. T: O(n + q * height), S: O(n).
vector<bool> rootAfterFlips(const vector<Node>& tree, int root,
                            const vector<int>& flips) {
    int n = tree.size();
    // val[id] = current value of the subtree at id
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

void test(const string& label, const vector<Node>& tree, int root,
          const vector<bool>& want) {
    bool ok = flipEachNaive(tree, root) == want && flipEach(tree, root) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void testFlips(const string& label, const vector<Node>& tree, int root,
               const vector<int>& flips, const vector<bool>& want) {
    cout << label << (rootAfterFlips(tree, root, flips) == want ? " PASS\n" : " FAIL\n");
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
}

// follow-up: "segment-tree style" design -> same as part 2: each node caches its value,
//   update(leaf) walks up recomputing; with parent pointers forbidden, a parent vector
//   (or a map node -> parent) is built once in O(n)
// follow-up: very deep tree -> recursion depth O(height); compute values iteratively in
//   reverse BFS order (children before parents) instead
// follow-up: part 1 queries for one arbitrary leaf -> keep the matters flags from spread
//   and answer each query in O(1)
