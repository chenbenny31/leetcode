#include <iostream>
#include <string>
#include <deque>
#include <cmath>

using std::cout;
using std::string;
using std::deque;
using std::fabs;

// vol-weighted avg price pf trade (now - time < window)

struct Trade {
    int time;
    int price;
    int qty;
};

class VwapWindow {
private:
    int window;
    deque<Trade> trades; // still in window, oldest first
    long long notional = 0; // sum of price * qty
    long long volume = 0; // sum of qty over trades

    void expire(int now) {
        while (!trades.empty() && now - trades.front().time >= window) {
            notional -= 1LL * trades.front().price * trades.front().qty;
            volume -= trades.front().qty;
            trades.pop_front();
        }
    }

public:
    VwapWindow(int windowLen) : window(windowLen) {}

    void add(int time, int price, int qty) {
        expire(time);
        trades.push_back({time, price, qty});
        notional += 1LL * price * qty;
        volume += qty;
    }

    double vwap(int now) {
        expire(now);
        if (volume == 0) { return 0; }
        return (double)notional / volume;
    }
};

// return the highest trade price in window

class RollingStats {
private:
    VwapWindow avg;
    int window;
    deque<Trade> highs; // mono deque, strictly decre

    void expire(int now) {
        while (!highs.empty() && now - highs.front().time >= window) {
            highs.pop_front();
        }
    }

public:
    RollingStats(int windowLen) : avg(windowLen), window(windowLen) {}

    void add(int time, int price, int qty) {
        avg.add(time, price, qty);
        expire(time);
        while (!highs.empty() && highs.back().price <= price) {
            highs.pop_back();
        }
        highs.push_back({time, price, qty});
    }

    double vwap(int now) { return avg.vwap(now); }
    
    int maxPrice(int now) {
        expire(now);
        if (highs.empty()) { return 0; }
        return highs.front().price;
    }
};

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
