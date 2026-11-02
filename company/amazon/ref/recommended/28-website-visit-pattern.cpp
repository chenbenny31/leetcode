// LC 1152 website visit pattern: visit i = (username[i], timestamp[i], website[i]).
// A pattern is 3 websites a user visited in that time order (not necessarily adjacent).
// Return the pattern visited by the most users; ties -> lexicographically smallest.
// Follow-up (consecutive variant): count only 3 adjacent visits per user.

#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <map>
#include <set>
#include <algorithm>
#include <unordered_map>

using std::cout;
using std::string;
using std::vector;
using std::tuple;
using std::map;
using std::set;
using std::sort;
using std::unordered_map;

// Sort visits by time, list each user's sites, count each user's distinct triples once.
// T: O(n log n + sum of L^3) for L visits per user, S: O(sum of L^3).
vector<string> mostVisitedPattern(const vector<string>& username,
                                  const vector<int>& timestamp,
                                  const vector<string>& website) {
    int n = username.size();
    // (time, user, site) in time order
    vector<tuple<int, string, string>> visits;
    for (int i = 0; i < n; i++) {
        visits.push_back({timestamp[i], username[i], website[i]});
    }
    sort(visits.begin(), visits.end());
    // sites[user] = sites that user visited, in time order
    unordered_map<string, vector<string>> sites;
    for (const auto& [time, user, site] : visits) { sites[user].push_back(site); }
    // users[pattern] = number of users with that pattern (map keeps patterns sorted)
    map<vector<string>, int> users;
    for (const auto& [user, list] : sites) {
        // this user's distinct patterns, so each user counts once
        set<vector<string>> mine;
        int m = list.size();
        for (int i = 0; i < m; i++) {
            for (int j = i + 1; j < m; j++) {
                for (int k = j + 1; k < m; k++) {
                    mine.insert({list[i], list[j], list[k]});
                }
            }
        }
        for (const auto& p : mine) { users[p]++; }
    }
    vector<string> best;
    int bestCount = 0;
    for (const auto& [pattern, count] : users) {
        if (count > bestCount) {
            best = pattern;
            bestCount = count;
        }
    }
    return best;
}

void test(const string& label, const vector<string>& username,
          const vector<int>& timestamp, const vector<string>& website,
          const vector<string>& want) {
    bool ok = mostVisitedPattern(username, timestamp, website) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("two-users-agree", {"a", "a", "a", "b", "b", "b"}, {1, 2, 3, 4, 5, 6},
         {"home", "cart", "pay", "home", "cart", "pay"}, {"home", "cart", "pay"});
    test("tie-smallest", {"a", "a", "a", "b", "b", "b"}, {1, 2, 3, 4, 5, 6},
         {"x", "y", "z", "c", "d", "e"}, {"c", "d", "e"});
    test("sort-by-time", {"a", "a", "a"}, {3, 1, 2}, {"z", "x", "y"}, {"x", "y", "z"});
}
