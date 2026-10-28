// LC 2916: for every subarray take (number of distinct values)^2; return the sum over all
// subarrays modulo 1e9 + 7.

#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>

using std::cout;
using std::string;
using std::vector;
using std::unordered_set;
using std::unordered_map;

const long long MOD = 1000000007;

// straightforward: grow each subarray from its left end with a set. T: O(n^2), S: O(n).
long long sumCounts(const vector<int>& nums) {
    int n = nums.size();
    long long total = 0;
    for (int i = 0; i < n; i++) {
        unordered_set<int> seen;
        for (int j = i; j < n; j++) {
            seen.insert(nums[j]);
            long long d = seen.size();
            total = (total + d * d) % MOD;
        }
    }
    return total;
}

// Segment tree over left ends l: f(l) = distinct count of nums[l..r] for the current r.
// Moving r to a value last seen at p adds 1 to f(l) for l in (p, r]. Each node keeps
// sum f and sum f^2; adding a to a range of len cells turns sq into
// sq + 2a*sum + a^2*len and sum into sum + a*len.
class SegTree {
    int n;
    // sum[node] = sum of f, sq[node] = sum of f^2, lazy[node] = pending add
    vector<long long> sum, sq, lazy;

    void apply(int node, int len, long long a) {
        sq[node] = (sq[node] + 2 * a % MOD * sum[node] + a * a % MOD * len) % MOD;
        sum[node] = (sum[node] + a * len) % MOD;
        lazy[node] = (lazy[node] + a) % MOD;
    }

    void push(int node, int lo, int hi) {
        if (lazy[node] == 0) { return; }
        int mid = (lo + hi) / 2;
        apply(2 * node, mid - lo + 1, lazy[node]);
        apply(2 * node + 1, hi - mid, lazy[node]);
        lazy[node] = 0;
    }

    void add(int node, int lo, int hi, int l, int r) {
        if (r < lo || hi < l) { return; }
        if (l <= lo && hi <= r) {
            apply(node, hi - lo + 1, 1);
            return;
        }
        push(node, lo, hi);
        int mid = (lo + hi) / 2;
        add(2 * node, lo, mid, l, r);
        add(2 * node + 1, mid + 1, hi, l, r);
        sum[node] = (sum[2 * node] + sum[2 * node + 1]) % MOD;
        sq[node] = (sq[2 * node] + sq[2 * node + 1]) % MOD;
    }

public:
    SegTree(int n) : n(n), sum(4 * n), sq(4 * n), lazy(4 * n) {}
    void addOne(int l, int r) { add(1, 0, n - 1, l, r); }
    long long squares() const { return sq[1]; }
};

// improved: for each right end, one range add, then add the root's sum of squares
// (f(l) = 0 for l > r, so only real subarrays count). T: O(n log n), S: O(n).
long long sumCountsTree(const vector<int>& nums) {
    int n = nums.size();
    SegTree st(n);
    // last[x] = last index where x appeared
    unordered_map<int, int> last;
    long long total = 0;
    for (int r = 0; r < n; r++) {
        int p = last.count(nums[r]) ? last[nums[r]] : -1;
        st.addOne(p + 1, r);
        last[nums[r]] = r;
        total = (total + st.squares()) % MOD;
    }
    return total;
}

void test(const string& label, const vector<int>& nums, long long want) {
    bool ok = sumCounts(nums) == want && sumCountsTree(nums) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("repeat", {1, 2, 1}, 15);
    test("same", {2, 2}, 3);
    test("distinct", {1, 2, 3}, 20);
    test("single", {7}, 1);
}
