// LC 1169: each transaction is "name,time,amount,city". It is invalid if amount > 1000,
// or if another transaction with the same name in a different city is within 60
// minutes (inclusive). Return every invalid transaction (here: in input order).
// PointOne angle: billing-rule validation that reports all violations, not just one.
#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;
using std::stoi;

struct Txn {
    string name;
    int time;
    int amount;
    string city;
};

// split "name,time,amount,city" with a plain character loop
Txn parse(const string& s) {
    vector<string> parts;
    string cur;
    for (char c : s) {
        if (c == ',') {
            parts.push_back(cur);
            cur.clear();
        } else {
            cur += c;
        }
    }
    parts.push_back(cur);
    return {parts[0], stoi(parts[1]), stoi(parts[2]), parts[3]};
}

// Check every rule for every transaction; the pair rule compares all pairs.
// T: O(n^2), S: O(n).
vector<string> invalidTransactions(const vector<string>& transactions) {
    int n = transactions.size();
    vector<Txn> txns;   // txns[i] = parsed transactions[i]
    for (const auto& s : transactions) { txns.push_back(parse(s)); }
    vector<string> result;
    for (int i = 0; i < n; i++) {
        bool bad = txns[i].amount > 1000;
        for (int j = 0; j < n && !bad; j++) {
            bool sameName = txns[i].name == txns[j].name;
            bool otherCity = txns[i].city != txns[j].city;
            int gap = txns[i].time - txns[j].time;
            if (sameName && otherCity && gap >= -60 && gap <= 60) { bad = true; }
        }
        if (bad) { result.push_back(transactions[i]); }
    }
    return result;
}

void test(const string& label, const vector<string>& transactions,
          const vector<string>& want) {
    cout << label << (invalidTransactions(transactions) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("amount", {"a,0,1200,ny"}, {"a,0,1200,ny"});
    test("two-city", {"a,0,10,ny", "a,60,10,sf"}, {"a,0,10,ny", "a,60,10,sf"});
    test("far", {"a,0,10,ny", "a,61,10,sf"}, {});
    test("same-city", {"a,0,10,ny", "a,30,10,ny", "b,30,10,sf"}, {});
}
