// input format: [from, to, rate]
// rev used or rate? yes, to->from is 1/rate
// non connected query -> all connected OR "-1.00" or ""
// same concurrency -> "1.00"
// diff path yield diff rate? assume non, otherwise want best rate?
// output format? -> two decimal e.g. "1.88"
// one query or many? -> many for pre-compute
// rate be 0? no

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <utility>
#include <unordered_map>
#include <sstream>
#include <iomanip>

using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::pair;
using std::unordered_map;
using std::stod;
using std::ostringstream;
using std::fixed;
using std::setprecision;

// BFS from `from`, multiply rates along the path, T: O(V+E), S: O(V+E)
string findConversionRate(const vector<vector<string>>& rates,
                          const vector<string>& query) {
    unordered_map<string, vector<pair<string, double>>> graph; // from -> (to, rate)
    for (const auto& rate : rates) {
        string from = rate[0];
        string to = rate[1];
        double r = stod(rate[2]);
        graph[from].push_back({to, r});
        graph[to].push_back({from, 1.0 / r});
    }

    string from = query[0];
    string to = query[1];
    unordered_map<string, double> vals; // vals[c] = `from` accum rate to `c`
    vals[from] = 1.0;
    queue<string> q;
    q.push(from);
    while (!q.empty()) {
        string cur = q.front();
        q.pop();
        for (const auto& [nxt, rate] : graph[cur]) {
            if (vals.count(nxt)) { continue; }
            vals[nxt] = vals[cur] * rate;
            q.push(nxt);
        }
    }
    if (!vals.count(to)) { return "-1.00"; }
    ostringstream out;
    out << fixed << setprecision(2) << vals[to];
    return out.str();
}

void test(const string& label, const vector<vector<string>>& rates,
          const vector<string>& query, const string& want) {
    cout << label << (findConversionRate(rates, query) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("direct", {{"USD", "EUR", "0.90"}}, {"USD", "EUR"}, "0.90");
    test("reverse", {{"USD", "EUR", "0.90"}}, {"EUR", "USD"}, "1.11");
    test("two-hops-back", {{"USD", "CAD", "2"}, {"CAD", "MXN", "10"}}, {"MXN", "USD"},
         "0.05");
    test("self", {{"USD", "EUR", "0.90"}}, {"USD", "USD"}, "1.00");
}
