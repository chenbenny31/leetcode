// rolling VWAP: trades (time, price, qty) arrive in time order
// (1) vwap(now) = sum(price * qty) / sum(qty) over the trades of the last `window`
// (2) highest trade price over the same window

// usage:
// g++ -std=c++17 -Wall -Wextra 14-rolling-vwap.cpp -o a && ./a
//
// RollingVwap stats(60);  stats.add(time, price, qty);  stats.vwap(now, result);

// clarify / assumptions
// times never go down; a trade at time t counts while now - t < window
// price is an int num of ticks; price * qty summed over a window fits in int64
// no trade in the window -> return false (there is no price to report)

// data:
// trades: the trades still in the window, oldest first; expired ones leave the front
// notional / volume: running sums over trades, so vwap is one division
// (2) maxPrices: only trades that can still become the max, prices go down front to
//     back; a new trade removes every lower-or-equal price before it

// T: add / vwap / maxPrice O(1) amortized (each trade enters and leaves once)
// S: O(num of trades in the window)

#include <cmath>
#include <cstdint>
#include <deque>
#include <string>
#include <iostream>

using std::deque;
using std::fabs;
using std::int64_t;
using std::cout;
using std::string;

struct Trade {
    int64_t time;
    int64_t price; // in ticks
    int64_t qty;
};

class RollingVwap {
private:
    int64_t window;
    deque<Trade> trades; // trades inside the window, oldest first
    int64_t notional; // sum of price * qty over trades
    int64_t volume; // sum of qty over trades
    deque<Trade> maxPrices; // (2) prices go down from front to back, front = max

    // drop what left the window: a trade stays while now - time < window
    void expire(int64_t now) {
        while (!trades.empty() && now - trades.front().time >= window) {
            notional -= trades.front().price * trades.front().qty;
            volume -= trades.front().qty;
            trades.pop_front();
        }
        while (!maxPrices.empty() && now - maxPrices.front().time >= window) {
            maxPrices.pop_front();
        }
    }
public:
    RollingVwap(int64_t windowLen) : window(windowLen), notional(0), volume(0) {}

    void add(int64_t time, int64_t price, int64_t qty) {
        expire(time);
        trades.push_back({time, price, qty});
        notional += price * qty;
        volume += qty;

        // (2) this trade is newer and not lower: the ones it beats can never be max
        while (!maxPrices.empty() && maxPrices.back().price <= price) {
            maxPrices.pop_back();
        }
        maxPrices.push_back({time, price, qty});
    }

    // (1) return false if no trade is in the window
    bool vwap(int64_t now, double& result) {
        expire(now);
        if (volume == 0) { return false; }
        result = (double)notional / volume;
        return true;
    }

    // (2) return false if no trade is in the window
    bool maxPrice(int64_t now, int64_t& result) {
        expire(now);
        if (maxPrices.empty()) { return false; }
        result = maxPrices.front().price;
        return true;
    }
};

void testVwap(const string& label, RollingVwap& stats, int64_t now, double want) {
    double got = 0;
    bool ok = (stats.vwap(now, got) && fabs(got - want) < 1e-9);
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void testMax(const string& label, RollingVwap& stats, int64_t now, int64_t want) {
    int64_t got = 0;
    bool ok = (stats.maxPrice(now, got) && got == want);
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void check(const string& label, bool ok) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    RollingVwap stats(10);
    double vwap = 0;
    int64_t maxPrice = 0;
    check("no-trades", !stats.vwap(0, vwap) && !stats.maxPrice(0, maxPrice));

    // test valid cases
    stats.add(1, 100, 10);
    stats.add(5, 102, 30);
    testVwap("two-trades", stats, 5, 101.5);
    testMax("max-two", stats, 5, 102);
    testVwap("first-expired", stats, 11, 102);

    stats.add(12, 101, 10);
    testVwap("after-add", stats, 12, 101.75);
    testMax("max-older-trade", stats, 12, 102);
    testVwap("second-expired", stats, 15, 101);
    testMax("max-moves-on", stats, 15, 101);

    check("all-expired", !stats.vwap(30, vwap) && !stats.maxPrice(30, maxPrice));
}
