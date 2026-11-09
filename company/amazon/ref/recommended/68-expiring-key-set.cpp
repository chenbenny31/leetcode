// Expiring key set: operations in nondecreasing time. add(key, ttl) inserts or refreshes
// key to expire at t + ttl; contains(key) reports presence; remove(key) deletes it. At
// the start of every operation, entries with expiry <= t are gone. Return the contains
// answers in order.

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;

// A key is present iff its stored expiry is still in the future, so contains() just
// compares; expired entries are never touched. T: O(1) per operation, S: O(keys).
vector<bool> runExpiringSet(const vector<string>& operations, const vector<string>& keys,
                            const vector<int>& timestamps, const vector<int>& ttl) {
    int n = operations.size();
    // expiry[key] = time the key disappears
    unordered_map<string, long long> expiry;
    vector<bool> result;
    for (int i = 0; i < n; i++) {
        long long now = timestamps[i];
        if (operations[i] == "add") {
            expiry[keys[i]] = now + ttl[i];
        } else if (operations[i] == "contains") {
            auto it = expiry.find(keys[i]);
            result.push_back(it != expiry.end() && it->second > now);
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

// follow-up: memory must be freed on time -> also push (expiry, key) into a min-heap
//   and pop entries with expiry <= now each operation (skip stale ones)
