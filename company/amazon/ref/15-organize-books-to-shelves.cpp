// numShelves shelves start empty. Book i must go on the shelf with the fewest books among
// shelves 0..limit[i]-1 (smallest index on ties). Return the shelf of every book.

#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::pair;
using std::min;

// straightforward: scan the allowed prefix for each book. T: O(b * s), S: O(s).
vector<int> organize(int numShelves, const vector<int>& limit) {
    // books[i] = books on shelf i
    vector<int> books(numShelves, 0);
    vector<int> result;
    for (int lim : limit) {
        int pick = 0;
        for (int i = 1; i < lim; i++) {
            if (books[i] < books[pick]) { pick = i; }
        }
        books[pick]++;
        result.push_back(pick);
    }
    return result;
}

// improved: segment tree over shelves storing min (books, index); prefix query plus point
// update. pair comparison breaks ties by smaller index. T: O(b log s), S: O(s).
class SegTree {
    int n;
    // tree[node] = min (books, shelf) over the node's range
    vector<pair<int, int>> tree;

    void build(int node, int lo, int hi) {
        if (lo == hi) {
            tree[node] = {0, lo};
            return;
        }
        int mid = (lo + hi) / 2;
        build(2 * node, lo, mid);
        build(2 * node + 1, mid + 1, hi);
        tree[node] = min(tree[2 * node], tree[2 * node + 1]);
    }

    pair<int, int> query(int node, int lo, int hi, int r) {
        if (hi <= r) { return tree[node]; }
        int mid = (lo + hi) / 2;
        if (r <= mid) { return query(2 * node, lo, mid, r); }
        return min(tree[2 * node], query(2 * node + 1, mid + 1, hi, r));
    }

    void add(int node, int lo, int hi, int i) {
        if (lo == hi) {
            tree[node].first++;
            return;
        }
        int mid = (lo + hi) / 2;
        if (i <= mid) {
            add(2 * node, lo, mid, i);
        } else {
            add(2 * node + 1, mid + 1, hi, i);
        }
        tree[node] = min(tree[2 * node], tree[2 * node + 1]);
    }

public:
    SegTree(int n) : n(n), tree(4 * n) { build(1, 0, n - 1); }

    // least-loaded shelf among 0..r
    int minPrefix(int r) { return query(1, 0, n - 1, r).second; }

    void addBook(int i) { add(1, 0, n - 1, i); }
};

vector<int> organizeTree(int numShelves, const vector<int>& limit) {
    SegTree st(numShelves);
    vector<int> result;
    for (int lim : limit) {
        int pick = st.minPrefix(lim - 1);
        st.addBook(pick);
        result.push_back(pick);
    }
    return result;
}

void test(const string& label, int numShelves, const vector<int>& limit,
          const vector<int>& want) {
    bool ok = organize(numShelves, limit) == want &&
              organizeTree(numShelves, limit) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("spread", 3, {3, 3, 3, 3}, {0, 1, 2, 0});
    test("narrow", 3, {1, 1, 2}, {0, 0, 1});
    test("mixed", 4, {2, 4, 1, 4}, {0, 1, 0, 2});
    test("one-shelf", 1, {1, 1}, {0, 0});
}
