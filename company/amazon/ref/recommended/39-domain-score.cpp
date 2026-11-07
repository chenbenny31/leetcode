// Each entry is (domain, score); "mail.a.com" is a sub-domain of "a.com", which is a
// sub-domain of "com". A leaf is an input domain that is no other input domain's
// ancestor. For every leaf, return its score plus the scores of its ancestors that
// appear in the input.

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <utility>

using std::cout;
using std::string;
using std::vector;
using std::map;
using std::unordered_map;
using std::unordered_set;
using std::pair;

// proper suffixes after each '.': "x.a.com" -> {"a.com", "com"}
vector<string> ancestors(const string& domain) {
    vector<string> result;
    int n = domain.size();
    for (int i = 0; i < n; i++) {
        if (domain[i] == '.') { result.push_back(domain.substr(i + 1)); }
    }
    return result;
}

// Every ancestor of an input domain is not a leaf; a leaf's total adds the scores of its
// ancestors found in the input. T: O(n * L^2) for length L, S: O(n * L).
map<string, long long> leafScores(const vector<pair<string, int>>& entries) {
    // score[d] = score of input domain d
    unordered_map<string, int> score;
    // input domains that have a descendant in the input
    unordered_set<string> hasChild;
    for (const auto& [d, s] : entries) {
        score[d] = s;
        for (const string& a : ancestors(d)) { hasChild.insert(a); }
    }
    map<string, long long> result;
    for (const auto& [d, s] : entries) {
        if (hasChild.count(d)) { continue; }
        long long total = s;
        for (const string& a : ancestors(d)) {
            if (score.count(a)) { total += score[a]; }
        }
        result[d] = total;
    }
    return result;
}

void test(const string& label, const vector<pair<string, int>>& entries,
          const map<string, long long>& want) {
    cout << label << (leafScores(entries) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("chain", {{"com", 1}, {"a.com", 2}, {"x.a.com", 4}}, {{"x.a.com", 7}});
    test("branches", {{"com", 1}, {"a.com", 2}, {"b.com", 8}},
         {{"a.com", 3}, {"b.com", 9}});
    test("missing-middle", {{"com", 1}, {"x.a.com", 4}}, {{"x.a.com", 5}});
    test("lone-tld", {{"org", 16}}, {{"org", 16}});
}
