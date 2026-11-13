// File System Size (Google, interview report)
// (1) fs tree in a vector pool (idx = node id); node = file w/ size or dir w/ child ids
// (1) return total size of node id: a file's own size, or sum of all files under a dir
// (2) many queries + file size changes
//
// clarify:
// tree format, root id -> vector pool, root 0
// valid tree? -> yes: no links, no cycles
// dir's own size field -> ignored; empty dir = 0
// sums past 2^31 -> long long
// nesting up to n deep -> recursion depth; explicit stack is a follow-up

#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::string;
using std::vector;

struct Node {
    bool isDir = false;
    // file size; unused for a dir
    long long size = 0;
    // child node ids, empty for a file
    vector<int> children;
};

// (1) one query
// core idea: total(dir) = sum of total(child); one DFS over the nodes under id
// T: O(subtree size) per query, S: O(depth)
long long totalSize(const vector<Node>& fs, int id) {
    if (!fs[id].isDir) { return fs[id].size; }
    long long sum = 0;
    for (int child : fs[id].children) { sum += totalSize(fs, child); }
    return sum;
}

// (2) cached totals + updates
// core idea: cache every node's total once; a file change only touches its parent chain
// T: build O(n), query O(1), update O(depth), S: O(n)
class SizeCache {
    vector<Node> fs;
    // parent[id] = parent dir id, -1 for the root
    vector<int> parent;
    // total[id] = cached total size of the subtree at id
    vector<long long> total;

    // post-order: total[id] = own file size + children's totals
    long long fill(int id) {
        total[id] = fs[id].isDir ? 0 : fs[id].size;
        for (int child : fs[id].children) { total[id] += fill(child); }
        return total[id];
    }

public:
    SizeCache(const vector<Node>& nodes, int root) : fs(nodes) {
        int n = fs.size();
        parent.assign(n, -1);
        total.assign(n, 0);
        for (int id = 0; id < n; id++) {
            for (int child : fs[id].children) { parent[child] = id; }
        }
        fill(root);
    }

    long long query(int id) { return total[id]; }

    void updateFile(int id, long long newSize) {
        // take the difference before overwriting the old size
        long long delta = newSize - fs[id].size;
        fs[id].size = newSize;
        int cur = id;
        while (cur != -1) {
            total[cur] += delta;
            cur = parent[cur];
        }
    }
};

void test(const string& label, const vector<Node>& fs, int id, long long want) {
    SizeCache cache(fs, 0);
    bool ok = totalSize(fs, id) == want && cache.query(id) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void testUpdate(const string& label, const vector<Node>& fs, int file, long long newSize,
                int id, long long want) {
    SizeCache cache(fs, 0);
    cache.updateFile(file, newSize);
    cout << label << (cache.query(id) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    vector<Node> fs = {{true, 0, {1, 2}}, {false, 100, {}}, {true, 0, {3, 4}},
                       {false, 200, {}}, {false, 300, {}}};
    test("root", fs, 0, 600);
    test("subdir", fs, 2, 500);
    test("empty-dir", {{true, 0, {}}}, 0, 0);
    test("dir-size-ignored", {{true, 4096, {1}}, {false, 10, {}}}, 0, 10);

    testUpdate("update-root", fs, 3, 50, 0, 450);
    testUpdate("update-subdir", fs, 4, 0, 2, 200);
    testUpdate("update-sibling", fs, 3, 50, 1, 100);
}

// follow-up:
// very deep tree (stack overflow) -> same sum w/ explicit stack; order does not matter
// deep tree, cache -> fill totals in reverse BFS order: children before parents
// queries by path ("/docs") -> nodes store a name; one DFS builds a map path -> id
// add file of size s under dir p -> append w/ size 0, link it in, then updateFile(id, s)
// delete a file -> updateFile(id, 0), then erase id from its parent's children
// updates >> queries -> update marks parent chain dirty, stops at a dirty node
// query on a dirty node -> recompute from children, recursing only into dirty ones
