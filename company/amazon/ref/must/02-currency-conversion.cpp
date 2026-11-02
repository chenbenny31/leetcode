// Currency conversion: rates[i] = [from, to, rate] means one `from` buys `rate` of `to`;
// a rate may also be used in reverse (1 / rate). Return the query[0] -> query[1] rate
// formatted with exactly two decimals. Every query is connected.

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

// BFS from `from`, multiplying rates along the edges. T: O(V+E), S: O(V+E).
string findConversionRate(const vector<vector<string>>& rates,
                          const vector<string>& query) {
    // graph[a] = (b, how many b one a buys), both directions
    unordered_map<string, vector<pair<string, double>>> graph;
    for (const auto& r : rates) {
        double rate = stod(r[2]);
        graph[r[0]].push_back({r[1], rate});
        graph[r[1]].push_back({r[0], 1.0 / rate});
    }
    string from = query[0];
    string to = query[1];
    // value[c] = how many c one `from` buys
    unordered_map<string, double> value;
    value[from] = 1.0;
    queue<string> q;
    q.push(from);
    while (!q.empty()) {
        string cur = q.front();
        q.pop();
        for (const auto& [nxt, rate] : graph[cur]) {
            if (value.count(nxt)) { continue; }
            value[nxt] = value[cur] * rate;
            q.push(nxt);
        }
    }
    ostringstream out;
    out << fixed << setprecision(2) << value[to];
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
