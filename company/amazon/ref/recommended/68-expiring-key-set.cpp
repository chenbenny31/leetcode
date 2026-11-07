// Expiring key set: operations in nondecreasing time. add(key, ttl) inserts or refreshes
// key to expire at t + ttl; contains(key) reports presence; remove(key) deletes it. At
// the start of every operation, entries with expiry <= t are gone. Return the contains
// answers in order.

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <utility>
#include <functional>
#include <unordered_map>

using std::cout;
using std::string;
using std::vector;
using std::priority_queue;
using std::pair;
using std::greater;
using std::unordered_map;

// Map key -> expiry answers queries; a min-heap of (expiry, key) deletes expired keys
// in time order. A heap entry is stale if the key was refreshed or removed since.
// T: O(n log n), S: O(n).
vector<bool> runExpiringSet(const vector<string>& operations, const vector<string>& keys,
                            const vector<int>& timestamps, const vector<int>& ttl) {
    int n = operations.size();
    // expiry[key] = time the key disappears
    unordered_map<string, long long> expiry;
    // (expiry, key): top = next key to expire
    priority_queue<pair<long long, string>, vector<pair<long long, string>>,
                   greater<pair<long long, string>>> heap;
    vector<bool> result;
    for (int i = 0; i < n; i++) {
        long long now = timestamps[i];
        while (!heap.empty() && heap.top().first <= now) {
            auto [when, key] = heap.top();
            heap.pop();
            auto it = expiry.find(key);
            // only the entry matching the key's current expiry removes it
            if (it != expiry.end() && it->second == when) { expiry.erase(it); }
        }
        if (operations[i] == "add") {
            long long when = now + ttl[i];
            expiry[keys[i]] = when;
            heap.push({when, keys[i]});
        } else if (operations[i] == "contains") {
            result.push_back(expiry.count(keys[i]) > 0);
        } else {
            expiry.erase(keys[i]);
        }
    }
    return result;
}

void test(const string& label, const vector<string>& operations,
          const vector<string>& keys, const vector<int>& timestamps,
          const vector<int>& ttl, const vector<bool>& want) {
    bool ok = runExpiringSet(operations, keys, timestamps, ttl) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("gone-at-expiry", {"add", "contains", "contains"}, {"s", "s", "s"},
         {0, 2, 3}, {3, 0, 0}, {true, false});
    test("refresh", {"add", "add", "contains"}, {"s", "s", "s"},
         {0, 1, 3}, {2, 5, 0}, {true});
    test("remove", {"add", "remove", "contains"}, {"s", "s", "s"},
         {0, 1, 1}, {9, 0, 0}, {false});
}
