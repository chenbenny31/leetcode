// Each log entry is (userId, timestamp). Count actions per user and return (user, count)
// sorted by count descending, then user id ascending.

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <utility>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::pair;
using std::sort;

// hash-map counts, then one sort. T: O(n + u log u), S: O(u).
vector<pair<string, int>> aggregate(const vector<pair<string, string>>& logs) {
    // counts[user] = actions of user
    unordered_map<string, int> counts;
    for (const auto& [user, ts] : logs) { counts[user]++; }
    vector<pair<string, int>> result(counts.begin(), counts.end());
    sort(result.begin(), result.end(), [](const auto& a, const auto& b) {
        if (a.second != b.second) { return a.second > b.second; }
        return a.first < b.first;
    });
    return result;
}

void test(const string& label, const vector<pair<string, string>>& logs,
          const vector<pair<string, int>>& want) {
    cout << label << (aggregate(logs) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("by-count", {{"bo", "t1"}, {"al", "t2"}, {"bo", "t3"}}, {{"bo", 2}, {"al", 1}});
    test("tie-id", {{"bo", "t1"}, {"al", "t2"}}, {{"al", 1}, {"bo", 1}});
    test("empty", {}, {});
}
