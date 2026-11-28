// 07 · Rolling VWAP — JD practice (research data and analysis)
// trades (time, price, qty) arrive in time order
// (1) vwap(now) = sum(price * qty) / sum(qty) over the trades of the last `window`
// (2) highest trade price over the same window
//
// clarify:
// in the window -> now - time < window
// call times -> nondecreasing across add / vwap / maxPrice
// time -> ns timestamps, so long long; price -> int ticks, qty > 0
// sums of price * qty -> long long
// no trade in the window -> return 0 (ask: could be NaN or a bool instead)

#include <iostream>
#include <string>
#include <deque>
#include <cmath>

using std::cout;
using std::string;
using std::deque;
using std::fabs;

// ==================== (1) ====================
// volume-weighted average price of the trades with now - time < window

struct Trade {
    long long time; // ns
    int price;      // in ticks
    int qty;
};

// core idea: queue of the trades in the window + two running sums; expire from the
// front, divide only when asked
// T: O(1) amortized per op, S: O(trades in the window)
class VwapWindow {
    long long window;
    deque<Trade> trades;    // trades still in the window, oldest first
    long long notional = 0; // sum of price * qty over trades
    long long volume = 0;   // sum of qty over trades

    void expire(long long now) {
        while (!trades.empty() && now - trades.front().time >= window) {
            notional -= 1LL * trades.front().price * trades.front().qty;
            volume -= trades.front().qty;
            trades.pop_front();
        }
    }

public:
    explicit VwapWindow(long long windowLen) : window(windowLen) {}

    void add(long long time, int price, int qty) {
        expire(time);
        trades.push_back({time, price, qty});
        notional += 1LL * price * qty;
        volume += qty;
    }

    double vwap(long long now) {
        expire(now);
        if (volume == 0) { return 0; }
        return (double)notional / volume;
    }
};

// ==================== (2) ====================
// also return the highest trade price in the same window

// core idea: monotonic deque of max candidates; a trade that is older and not higher
// than a newer one can never be the max again, so it leaves from the back
// T: O(1) amortized per op, S: O(trades in the window)
class RollingStats {
    VwapWindow average;
    long long window;
    deque<Trade> highs; // max candidates, oldest first, prices strictly decreasing

    void expire(long long now) {
        while (!highs.empty() && now - highs.front().time >= window) {
            highs.pop_front();
        }
    }

public:
    explicit RollingStats(long long windowLen) : average(windowLen), window(windowLen) {}

    void add(long long time, int price, int qty) {
        average.add(time, price, qty);
        expire(time);
        while (!highs.empty() && highs.back().price <= price) { highs.pop_back(); }
        highs.push_back({time, price, qty});
    }

    double vwap(long long now) { return average.vwap(now); }

    int maxPrice(long long now) {
        expire(now);
        if (highs.empty()) { return 0; }
        return highs.front().price;
    }
};

// ==================== tests ====================
void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    RollingStats stats(10);
    check(stats.vwap(0) == 0 && stats.maxPrice(0) == 0, "no-trades");

    stats.add(1, 100, 10);
    stats.add(5, 102, 30);
    check(fabs(stats.vwap(5) - 101.5) < 1e-9 && stats.maxPrice(5) == 102, "two-trades");
    check(fabs(stats.vwap(11) - 102) < 1e-9, "first-expired");

    stats.add(12, 101, 10);
    check(fabs(stats.vwap(12) - 101.75) < 1e-9, "after-add");
    check(stats.maxPrice(12) == 102, "max-is-older-trade");
    check(stats.maxPrice(15) == 101, "max-expired");
    check(stats.vwap(30) == 0 && stats.maxPrice(30) == 0, "all-expired");
}

// follow-up:
// 1. window = last N trades -> ring buffer of N trades + the same two sums
// 2. min price too -> a second deque with prices strictly increasing
// 3. flag outlier trades -> also keep sum of price^2; z = (price - mean) / std
// 4. trades arrive late -> reorder buffer: min-heap by time, release after a delay
// 5. many symbols -> hash map symbol -> RollingStats
// 6. median price in the window -> two heaps with lazy deletion, or a multiset + iterator
