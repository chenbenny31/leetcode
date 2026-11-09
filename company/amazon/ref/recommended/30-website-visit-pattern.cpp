// LC 1152 website visit pattern: visit i = (username[i], timestamp[i], website[i]).
// A pattern is 3 websites a user visited in that time order (not necessarily adjacent).
// Return the pattern visited by the most users; ties -> lexicographically smallest.
// Onsite version (fastprep, Sep 2026): only 3 consecutive visits of a user count.

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

// Sort visits by time, then list each user's sites. T: O(n log n), S: O(n).
unordered_map<string, vector<string>> sitesByUser(const vector<string>& username,
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
    return sites;
}

// First pattern with the highest count; map order makes it the smallest on ties.
// T: O(P) for P patterns, S: O(1).
vector<string> mostUsers(const map<vector<string>, int>& users) {
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

// Count each user's distinct triples once.
// T: O(n log n + sum of L^3) for L visits per user, S: O(sum of L^3).
vector<string> mostVisitedPattern(const vector<string>& username,
                                  const vector<int>& timestamp,
                                  const vector<string>& website) {
    auto sites = sitesByUser(username, timestamp, website);
    // users[pattern] = number of users with that pattern (map keeps patterns sorted)
    map<vector<string>, int> users;
    for (const auto& [user, list] : sites) {
        // this user's distinct patterns, so each user counts once
        set<vector<string>> mine;
        int m = list.size();
        for (int first = 0; first < m; first++) {
            for (int second = first + 1; second < m; second++) {
                for (int third = second + 1; third < m; third++) {
                    mine.insert({list[first], list[second], list[third]});
                }
            }
        }
        for (const auto& p : mine) { users[p]++; }
    }
    return mostUsers(users);
}

// Consecutive version: a user's patterns are the windows list[i..i+2] only.
// T: O(n log n), S: O(n).
vector<string> mostVisitedConsecutive(const vector<string>& username,
                                      const vector<int>& timestamp,
                                      const vector<string>& website) {
    auto sites = sitesByUser(username, timestamp, website);
    // users[pattern] = number of users with that pattern (map keeps patterns sorted)
    map<vector<string>, int> users;
    for (const auto& [user, list] : sites) {
        // this user's distinct windows, so each user counts once
        set<vector<string>> mine;
        int m = list.size();
        for (int start = 0; start + 2 < m; start++) {
            mine.insert({list[start], list[start + 1], list[start + 2]});
        }
        for (const auto& p : mine) { users[p]++; }
    }
    return mostUsers(users);
}

void test(const string& label, const vector<string>& username,
          const vector<int>& timestamp, const vector<string>& website,
          const vector<string>& want) {
    bool ok = mostVisitedPattern(username, timestamp, website) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void testConsecutive(const string& label, const vector<string>& username,
                     const vector<int>& timestamp, const vector<string>& website,
                     const vector<string>& want) {
    bool ok = mostVisitedConsecutive(username, timestamp, website) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("two-users-agree", {"a", "a", "a", "b", "b", "b"}, {1, 2, 3, 4, 5, 6},
         {"home", "cart", "pay", "home", "cart", "pay"}, {"home", "cart", "pay"});
    test("tie-smallest", {"a", "a", "a", "b", "b", "b"}, {1, 2, 3, 4, 5, 6},
         {"x", "y", "z", "c", "d", "e"}, {"c", "d", "e"});
    test("sort-by-time", {"a", "a", "a"}, {3, 1, 2}, {"z", "x", "y"}, {"x", "y", "z"});
    test("gap-allowed", {"a", "a", "a", "a", "b", "b", "b"}, {1, 2, 3, 4, 5, 6, 7},
         {"home", "ad", "cart", "pay", "home", "cart", "pay"}, {"home", "cart", "pay"});

    testConsecutive("gap-breaks", {"a", "a", "a", "a", "b", "b", "b"},
                    {1, 2, 3, 4, 5, 6, 7},
                    {"home", "ad", "cart", "pay", "home", "cart", "pay"},
                    {"ad", "cart", "pay"});
    testConsecutive("user-counts-once", {"a", "a", "a", "a", "a", "a", "b", "b", "b"},
                    {1, 2, 3, 4, 5, 6, 7, 8, 9},
                    {"x", "y", "z", "x", "y", "z", "y", "z", "x"}, {"y", "z", "x"});
    testConsecutive("sort-by-time", {"a", "a", "a"}, {3, 1, 2}, {"z", "x", "y"},
                    {"x", "y", "z"});
}
