// Loyal customers: logs of [customerId, productId] for two consecutive days. A customer
// is loyal if seen on both days and touched at least 2 distinct products across both
// days. Return loyal ids sorted.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>

using std::cout;
using std::string;
using std::vector;
using std::sort;
using std::unordered_map;
using std::unordered_set;

// Sets of day-one / day-two customers plus each customer's distinct products.
// T: O(n + r log r) for r loyal ids, S: O(n).
vector<string> loyalCustomers(const vector<vector<string>>& dayOneLogs,
                              const vector<vector<string>>& dayTwoLogs) {
    // products[c] = distinct products customer c touched on either day
    unordered_map<string, unordered_set<string>> products;
    // customers seen on day one / day two
    unordered_set<string> dayOne;
    unordered_set<string> dayTwo;
    for (const auto& log : dayOneLogs) {
        dayOne.insert(log[0]);
        products[log[0]].insert(log[1]);
    }
    for (const auto& log : dayTwoLogs) {
        dayTwo.insert(log[0]);
        products[log[0]].insert(log[1]);
    }
    vector<string> result;
    for (const string& c : dayOne) {
        if (dayTwo.count(c) && products[c].size() >= 2) { result.push_back(c); }
    }
    sort(result.begin(), result.end());
    return result;
}

void test(const string& label, const vector<vector<string>>& dayOneLogs,
          const vector<vector<string>>& dayTwoLogs, const vector<string>& want) {
    bool ok = loyalCustomers(dayOneLogs, dayTwoLogs) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("both-rules", {{"zed", "pen"}, {"amy", "pen"}}, {{"zed", "ink"}, {"amy", "ink"}},
         {"amy", "zed"});
    test("one-product", {{"bob", "pen"}}, {{"bob", "pen"}}, {});
    test("one-day-only", {{"cy", "pen"}, {"cy", "ink"}}, {}, {});
}
