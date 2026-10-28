// Practical: lawyers bill in 6-minute units (0.1 hour). Each time entry is rounded UP to
// whole units on its own. Given entries (client, minutes), return the billed units per
// client, clients in name order. (Ask first: per-entry rounding vs rounding the sum.)
#include <iostream>
#include <vector>
#include <string>
#include <map>

using std::cout;
using std::string;
using std::vector;
using std::pair;
using std::map;

// ceil(minutes / 6) in integer math; 0 minutes bills nothing. T: O(1).
int billedUnits(int minutes) {
    return (minutes + 5) / 6;
}

// Round each entry, then sum per client. T: O(n log c) for c clients, S: O(c).
map<string, int> unitsByClient(const vector<pair<string, int>>& entries) {
    map<string, int> total;   // total[client] = billed units, ordered by client name
    for (const auto& [client, minutes] : entries) {
        total[client] += billedUnits(minutes);
    }
    return total;
}

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    check(billedUnits(6) == 1 && billedUnits(7) == 2, "round-up");
    check(billedUnits(0) == 0 && billedUnits(1) == 1, "edge");
    map<string, int> want = {{"acme", 3}, {"beta", 2}};
    check(unitsByClient({{"acme", 7}, {"beta", 12}, {"acme", 5}}) == want, "by-client");
}
