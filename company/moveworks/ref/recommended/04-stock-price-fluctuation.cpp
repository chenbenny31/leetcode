// 04 · Stock Price Fluctuation (phone screen, Medium) — LC 2034
// records (timestamp, price) arrive out of order; a repeated timestamp corrects the
// earlier price. current() = price at the max timestamp; maximum() / minimum() = over
// the current records
//
// clarify:
// queries before any update -> not called (given)
// timestamps, prices -> up to 1e9, fit int
// correction of the latest timestamp -> current() returns the corrected price
// calls -> up to 1e5, so each op must be ~O(log n)

#include <iostream>
#include <string>
#include <unordered_map>
#include <set>
#include <algorithm>

using std::cout;
using std::string;
using std::unordered_map;
using std::multiset;
using std::max;

// core idea: map time -> price for corrections; a multiset of live prices gives min/max;
// a correction erases one copy of the old price
// T: update O(log n), current O(1), maximum / minimum O(1); S: O(n)
class StockPrice {
    // timestamp -> its latest price
    unordered_map<int, int> priceAt;
    // one entry per timestamp, sorted, duplicates kept
    multiset<int> prices;
    int latest = 0;

public:
    void update(int timestamp, int price) {
        auto it = priceAt.find(timestamp);
        if (it != priceAt.end()) {
            // erase(value) would remove every copy; erase one iterator only
            prices.erase(prices.find(it->second));
        }
        priceAt[timestamp] = price;
        prices.insert(price);
        latest = max(latest, timestamp);
    }

    int current() { return priceAt[latest]; }
    int maximum() { return *prices.rbegin(); }
    int minimum() { return *prices.begin(); }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    StockPrice stock;
    stock.update(1, 10);
    stock.update(2, 5);
    check(stock.current() == 5, "current-latest");
    check(stock.maximum() == 10, "max");
    stock.update(1, 3);
    check(stock.maximum() == 5, "max-after-correction");
    stock.update(4, 2);
    check(stock.minimum() == 2, "min");

    StockPrice same;
    same.update(1, 7);
    same.update(2, 7);
    same.update(1, 9);
    check(same.minimum() == 7 && same.maximum() == 9, "duplicate-price-kept");
    return 0;
}

// follow-up:
// no multiset (Java / Python) -> max-heap and min-heap of (price, time); on query pop
//   tops while priceAt[time] != price (stale), lazy deletion
// price range over a time window -> ordered map time -> price + segment tree
// many readers, one writer -> one mutex around all three structures (change together)
