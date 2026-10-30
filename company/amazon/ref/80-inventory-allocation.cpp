// requests[i] = [customerId, quantity, bid, timestamp]. Serve higher bids first. Within
// one bid, hand out items round-robin (one per customer per round, by increasing time)
// until the group is filled or inventory runs out; lower bids get nothing until every
// higher bid is filled. Return the ids (ascending) of customers not fully served.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::sort;
using std::min;
using std::max;

// requests sorted by bid descending, then timestamp ascending
vector<vector<int>> byPriority(vector<vector<int>> requests) {
    auto higher = [](const vector<int>& a, const vector<int>& b) {
        if (a[2] != b[2]) { return a[2] > b[2]; }
        return a[3] < b[3];
    };
    sort(requests.begin(), requests.end(), higher);
    return requests;
}

// straightforward: simulate rounds item by item. T: O(n log n + inventory), S: O(n).
vector<int> unfulfilled(const vector<vector<int>>& requests, long long inventory) {
    vector<vector<int>> reqs = byPriority(requests);
    int n = reqs.size();
    // got[i] = items given to reqs[i]
    vector<long long> got(n, 0);
    int i = 0;
    while (i < n) {
        int j = i;
        while (j < n && reqs[j][2] == reqs[i][2]) { j++; }
        bool gave = true;
        while (gave && inventory > 0) {
            gave = false;
            for (int c = i; c < j && inventory > 0; c++) {
                if (got[c] < reqs[c][1]) {
                    got[c]++;
                    inventory--;
                    gave = true;
                }
            }
        }
        i = j;
    }
    vector<int> result;
    for (int c = 0; c < n; c++) {
        if (got[c] < reqs[c][1]) { result.push_back(reqs[c][0]); }
    }
    sort(result.begin(), result.end());
    return result;
}

// improved: per bid group, binary search the number of full rounds r that fit
// (sum of min(q, r) <= inventory); the leftover items (fewer than the customers still
// waiting) go one each in timestamp order. T: O(n log n + n log q), S: O(n).
vector<int> unfulfilledRounds(const vector<vector<int>>& requests, long long inventory) {
    vector<vector<int>> reqs = byPriority(requests);
    int n = reqs.size();
    vector<int> result;
    int i = 0;
    while (i < n) {
        int j = i;
        while (j < n && reqs[j][2] == reqs[i][2]) { j++; }
        long long need = 0, maxQ = 0;
        for (int c = i; c < j; c++) {
            need += reqs[c][1];
            maxQ = max<long long>(maxQ, reqs[c][1]);
        }
        if (need <= inventory) {
            inventory -= need;
            i = j;
            continue;
        }
        // largest r with used(r) <= inventory
        long long lo = 0, hi = maxQ;
        while (lo <= hi) {
            long long mid = lo + (hi - lo) / 2;
            long long used = 0;
            for (int c = i; c < j; c++) { used += min<long long>(reqs[c][1], mid); }
            if (used <= inventory) {
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }
        long long r = hi;
        for (int c = i; c < j; c++) { inventory -= min<long long>(reqs[c][1], r); }
        for (int c = i; c < j; c++) {
            long long q = reqs[c][1];
            if (q <= r) { continue; }
            long long got = r;
            if (inventory > 0) {
                got++;
                inventory--;
            }
            if (got < q) { result.push_back(reqs[c][0]); }
        }
        i = j;
    }
    sort(result.begin(), result.end());
    return result;
}

void test(const string& label, const vector<vector<int>>& requests, long long inventory,
          const vector<int>& want) {
    bool ok = unfulfilled(requests, inventory) == want &&
              unfulfilledRounds(requests, inventory) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("short", {{1, 2, 5, 0}, {2, 3, 5, 1}, {3, 1, 1, 0}}, 4, {2, 3});
    test("enough", {{1, 2, 5, 0}, {2, 3, 5, 1}, {3, 1, 1, 0}}, 6, {});
    test("extra-item", {{1, 3, 5, 0}, {2, 3, 5, 1}}, 5, {2});
    test("priority", {{1, 5, 1, 0}, {2, 1, 9, 5}}, 1, {1});
    test("none", {{7, 1, 3, 0}}, 0, {7});
}
