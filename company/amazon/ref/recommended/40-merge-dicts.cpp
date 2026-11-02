// Merge two dictionaries into one; on a key collision the later dictionary's value wins
// (clarify this first). Follow-up: merge k dictionaries.

#include <iostream>
#include <vector>
#include <string>
#include <map>

using std::cout;
using std::string;
using std::vector;
using std::map;

// straightforward: copy a, then write b over it. T: O(|a| + |b|) map operations.
map<string, int> mergeTwo(const map<string, int>& a, const map<string, int>& b) {
    map<string, int> result = a;
    for (const auto& [k, v] : b) { result[k] = v; }
    return result;
}

// k dictionaries by folding mergeTwo: every step copies everything merged so far again.
// T: O(k * total) map operations.
map<string, int> mergeAllFold(const vector<map<string, int>>& dicts) {
    map<string, int> result;
    for (const auto& d : dicts) { result = mergeTwo(result, d); }
    return result;
}

// improved: write every dictionary into one result in order, so each entry is touched
// once. T: O(total) map operations, S: O(distinct keys).
map<string, int> mergeAll(const vector<map<string, int>>& dicts) {
    map<string, int> result;
    for (const auto& d : dicts) {
        for (const auto& [k, v] : d) { result[k] = v; }
    }
    return result;
}

void test(const string& label, const vector<map<string, int>>& dicts,
          const map<string, int>& want) {
    bool ok = mergeAllFold(dicts) == want && mergeAll(dicts) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("disjoint", {{{"a", 1}}, {{"b", 2}}}, {{"a", 1}, {"b", 2}});
    test("later-wins", {{{"a", 1}}, {{"a", 2}}, {{"a", 3}}}, {{"a", 3}});
    test("empty", {{}, {{"x", 5}}}, {{"x", 5}});
    test("none", {}, {});
}
