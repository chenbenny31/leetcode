// Boolean Expression Tree Leaf Flips (Google, interview report). Nodes live in a vector
// pool (index = node id); an internal node is AND / OR / XOR (two children) or NOT (one
// child, `left`); a leaf ("VAL") holds true / false.
// Part 1: for each leaf, left to right, flip it alone and record the root value (each
//   flip is undone before the next).
// Part 2: flips are cumulative (flips[i] = a leaf id); return the root value after each.
//   Nodes may not store parent pointers, so parents are kept in a separate vector.
//
// Clarify (1):
// - tree format -> vector pool, root id given; well-formed: AND / OR / XOR have both
//   children, NOT only left, a leaf none
// - output order -> one entry per leaf, leaves left to right
// - each flip is undone before the next -> the answers are independent
// - return the root value after the flip (not "did it change")
// - n up to ~1e5 -> naive O(n^2) is too slow, two passes are O(n); recursion depth is
//   the height
// Idea (1): pass 1 bottom-up computes every subtree value. Pass 2 top-down carries
//   matters = "flipping this subtree flips the root": XOR / NOT always pass it on, AND
//   only to a child whose sibling is true (a false sibling pins AND at false), OR only to
//   a child whose sibling is false. A leaf records matters ? !root : root.
// Pitfalls (1):
// - AND / OR test the sibling's value, not the child's own
// - NOT has no right child: check right != -1 before recursing or reading val[right]
// - recurse left before right so the leaves come out left to right
// - naive version: flip the leaf back after each evaluation
// Clarify (2): flips accumulate; flips[i] is a leaf id and may repeat (a second flip
//   undoes the first); Node holds no parent pointer -> a separate parent vector.
// Idea (2): cache every node's value; a flip changes only the leaf's ancestors, so walk
//   up recomputing each from its children and stop at the first one whose value stays
//   the same (nothing above it can change).
// Pitfalls (2): flip the cached val[leaf] (tree[leaf].value is read only once); compare
//   the recomputed value before storing it; the walk ends at the root's parent, -1.
// Tests to write: (1) AND with one false leaf; a mixed AND / OR / XOR tree; XOR over NOT;
//   a single leaf. (2) cumulative flips on AND; flips that stop early; one leaf twice.

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

// {fewest leaf flips that make the subtree at id false, fewest that make it true}
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

// follow-up (LC 2313): fewest leaf flips that make the root equal target. Tree DP: each
// node returns its cost to become false and to become true; an operator takes the
// cheapest pair of child results that produces each value. T: O(n), S: O(h) recursion.
// Pitfall: AND false = one false side is enough (min over three pairs), AND true needs
// both; OR is the mirror; NOT swaps the pair.
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
    testMinFlips("min-already", mixed, 0, true, 0);
    testMinFlips("min-mixed-false", mixed, 0, false, 1);
    testMinFlips("min-two-flips", bothFalse, 0, true, 2);
    testMinFlips("min-or-cheap-side", orCheapSide, 0, true, 1);
    testMinFlips("min-xor-not", notTree, 0, true, 1);
}

// follow-up: "segment-tree style" design -> rootAfterFlips above as a class: the
//   constructor builds val and the parent vector (or a map node -> parent) in O(n),
//   update(leaf) is the loop body, query() returns val[root]
// follow-up: very deep tree -> fill val without recursion in reverse BFS order
//   (children before parents):
//     vector<int> order = {root};
//     for (int i = 0; i < (int)order.size(); i++) { push order[i]'s non -1 children }
//     for (int i = order.size() - 1; i >= 0; i--) {
//         const Node& node = tree[order[i]];
//         val[order[i]] = node.op == "VAL" ? node.value : combine(node, val);
//     }
// follow-up: part 1 queries for single leaves -> spread stores matters[id] instead of
//   pushing a result; each query is then O(1):
//     bool after = matters[leaf] ? !rootValue : rootValue;
// follow-up: fewest flips to make the root a given value (LC 2313) -> minFlipsToResult
//   above
