// File System Size (Google, interview report): a file-system tree whose nodes live in a
// vector pool (index = node id). A node is a file with a size >= 0 or a directory with a
// list of child ids. Return the total size of node id: a file's own size, or for a
// directory the sum of all files under it (an empty directory is 0).
// Follow-up: many queries plus file size changes -> cache every node's total and adjust
// the cached sums along the parent chain on each change.

#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::string;
using std::vector;

struct Node {
    bool isDir = false;
    // file size, unused for a directory
    long long size = 0;
    // child node ids, empty for a file
    vector<int> children;
};

// DFS summing the files below id. T: O(subtree size) per query, S: O(depth).
long long totalSize(const vector<Node>& fs, int id) {
    if (!fs[id].isDir) { return fs[id].size; }
    long long sum = 0;
    for (int child : fs[id].children) { sum += totalSize(fs, child); }
    return sum;
}

// follow-up: cache every node's total once; query O(1); a file size change adds the
// difference to every ancestor. T: build O(n), query O(1), update O(depth), S: O(n).
class SizeCache {
    vector<Node> fs;
    // parent[id] = parent directory id, -1 for the root
    vector<int> parent;
    // total[id] = cached total size of the subtree at id
    vector<long long> total;

    long long fill(int id) {
        total[id] = fs[id].size;
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
    // 0: "/" {1, 2}, 1: a.txt 100, 2: "docs" {3, 4}, 3: b.txt 200, 4: c.txt 300
    vector<Node> fs = {{true, 0, {1, 2}}, {false, 100, {}}, {true, 0, {3, 4}},
                       {false, 200, {}}, {false, 300, {}}};
    test("root", fs, 0, 600);
    test("subdir", fs, 2, 500);
    test("file", fs, 3, 200);
    test("empty-dir", {{true, 0, {}}}, 0, 0);

    testUpdate("update-root", fs, 3, 50, 0, 450);
    testUpdate("update-subdir", fs, 4, 0, 2, 200);
    testUpdate("update-sibling", fs, 3, 50, 1, 100);
}

// follow-up: very deep tree (stack overflow) -> iterative DFS with an explicit stack:
//   push id; pop cur, add fs[cur].size, push its children; sum is order-independent.
//   For the cache, fill totals in reverse BFS order (children before parents).
// follow-up: queries by path ("/docs") -> keep a map path -> node id next to the pool
// follow-up: add / delete a file -> same as an update: +size / -size along the parents
// follow-up: updates are frequent and queries rare -> invalidate cached totals on the
//   parent chain (mark dirty) and recompute lazily on the next query
