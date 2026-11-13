// File System Size (Google, interview report): a file-system tree whose nodes live in a
// vector pool (index = node id). A node is a file with a size >= 0 or a directory with a
// list of child ids. Return the total size of node id: a file's own size, or for a
// directory the sum of all files under it (an empty directory is 0).
// Follow-up: many queries plus file size changes -> cache every node's total and adjust
// the cached sums along the parent chain on each change.
//
// Clarify:
// - tree format -> vector pool, id = index; a directory lists child ids; root id given
//   (0 in the tests); assume a valid tree: each node in one directory, no cycles
// - query a file -> its own size; an empty directory -> 0; a directory's own size field
//   is ignored (no per-directory overhead)
// - sizes >= 0, but sums can pass 2^31 -> long long
// - n up to ~1e5 and nesting can be n deep -> recursion depth (iterative follow-up)
// - links (one node under two directories)? -> assume none, else it counts twice
// - many queries? -> one DFS per query first, then the cache (follow-up)
// Idea: total(dir) = sum of total(child), a post-order sum; one DFS visits each node
//   under id once. For many queries, cache every node's total once; a file size change
//   only changes the totals on its parent chain, so add the delta there.
// Pitfalls:
// - a directory adds only its children's totals, never its own size field
// - long long for sizes and totals
// - updateFile: take delta = newSize - old size before overwriting fs[id].size, and
//   start the walk at the file itself (its own cached total changes too)
// - parent[] is built from the children lists once; the root's parent stays -1
// Tests to write: the root; a subdirectory; a file; an empty directory; a directory
//   with a nonzero size field; after an update: the root, the parent dir, a sibling.

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

// follow-up "very deep tree": the same sum with an explicit stack instead of recursion;
// a sum does not care about visit order. T: O(subtree size), S: O(subtree size).
long long totalSizeIterative(const vector<Node>& fs, int id) {
    // nodes still to visit
    vector<int> st = {id};
    long long sum = 0;
    while (!st.empty()) {
        int cur = st.back();
        st.pop_back();
        if (!fs[cur].isDir) { sum += fs[cur].size; }
        for (int nxt : fs[cur].children) { st.push_back(nxt); }
    }
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
        // a directory's own size field is unused, as in totalSize
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
    ok = ok && totalSizeIterative(fs, id) == want;
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
    test("dir-size-ignored", {{true, 4096, {1}}, {false, 10, {}}}, 0, 10);

    testUpdate("update-root", fs, 3, 50, 0, 450);
    testUpdate("update-subdir", fs, 4, 0, 2, 200);
    testUpdate("update-sibling", fs, 3, 50, 1, 100);
}

// follow-up: very deep tree (stack overflow) -> totalSizeIterative above. For the cache,
//   fill totals in reverse BFS order (children before parents):
//     vector<int> order = {root};
//     for (int i = 0; i < (int)order.size(); i++) { push fs[order[i]].children }
//     for (int i = order.size() - 1; i >= 0; i--) { total[order[i]] = own + children }
// follow-up: queries by path ("/docs") -> nodes also store a name; build
//   unordered_map<string, int> idOf once by a DFS that passes path + "/" + name down,
//   then query(idOf[path])
// follow-up: add a file of size s under directory p -> append it with size 0, then the
//   update walk adds s to it and every ancestor:
//     int id = fs.size();
//     fs.push_back({false, 0, {}});
//     fs[p].children.push_back(id);
//     parent.push_back(p);
//     total.push_back(0);
//     updateFile(id, s);
//   delete a file -> updateFile(id, 0), then erase id from fs[parent[id]].children
// follow-up: updates frequent, queries rare -> an update sets the size and marks the
//   parent chain dirty, stopping at a node already dirty (its ancestors are dirty too):
//     for (int cur = id; cur != -1 && !dirty[cur]; cur = parent[cur]) {
//         dirty[cur] = true;
//     }
//   a query on a dirty node recomputes it from its children (recursing only into dirty
//   ones) and clears the flag
