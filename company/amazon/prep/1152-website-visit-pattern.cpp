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


// sort visits by time, then list each user's sites, T: O(nlogn), S: O(n)
unordered_map<string, vector<string> sitesByUser(const vector<string>& username,
                                                 const vector<int>& timestamp,
                                                 const vector<string>& website) {
    int n = username.size();
    vector<tuple<int, string, string>> visits; // (time, user, site) in time order
    for (int i = 0; i < n; i++) {
        visits.push_back({timestamp[i], username[i], website[i]});
    }
    sort(visits.begin(), visits.end());
    unordered_map<string, vector<string>> sites;
    for (const auto& [time, user, site] : visits) {
        sites[user].push_back(site);
    }
    return sites;
}

// first pattern with max count, map order make smaller on tie
vector<string> mostUsers(const map<vector<string>, int>& users) {
}
