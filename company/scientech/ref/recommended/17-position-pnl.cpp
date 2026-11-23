// position and PnL: fills (side, price, qty) of one symbol arrive one by one
// (1) average cost: keep the signed position, its average entry price and the
//     realized PnL; unrealized PnL = what the open position makes at the mark price
// (2) FIFO: a closing fill is matched against the oldest open lot first

// usage:
// g++ -std=c++17 -Wall -Wextra 17-position-pnl.cpp -o a && ./a
//
// (1) PositionTracker book;  book.onFill(Side::BUY, 10, 100);  book.unrealized(mark);
// (2) FifoPnl fifo;  fifo.onFill(Side::BUY, 10, 100);  fifo.realizedPnl();

// clarify / assumptions
// the cost method decides the realized PnL: (1) and (2) give different numbers
// no fees
// short positions are allowed, a fill may cross through flat (long 150, sell 250)
// price is an int num of ticks; PnL is in ticks * shares

// rules:
// fill in the same direction as the position -> opens / adds: re-average the entry price
// fill in the other direction -> closes: realized += (exit - entry) * closed qty for a
//   long, (entry - exit) for a short; the entry price of what stays open does not change
// fill bigger than the position -> the rest opens a new position at the fill price

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <deque>
#include <string>
#include <iostream>

using std::abs;
using std::deque;
using std::fabs;
using std::int64_t;
using std::min;
using std::cout;
using std::string;

enum class Side { BUY, SELL };

class PositionTracker {
private:
    int64_t position; // signed shares: > 0 long, < 0 short, 0 flat
    double avgPrice; // average entry price of the open position, 0 when flat
    double realized; // PnL already locked in by closing fills
public:
    PositionTracker() : position(0), avgPrice(0), realized(0) {}

    // T: O(1), S: O(1)
    void onFill(Side side, int64_t price, int64_t qty) {
        int64_t signedQty = (side == Side::BUY) ? qty : -qty;
        bool adds = (position >= 0 && signedQty > 0) || (position <= 0 && signedQty < 0);

        if (adds) { // total cost / total shares
            double cost = avgPrice * abs(position) + (double)price * qty;
            position += signedQty;
            avgPrice = cost / abs(position);
            return;
        }

        int64_t closed = min(qty, abs(position)); // can't close more than is open
        if (position > 0) {
            realized += (price - avgPrice) * closed; // long: sold above entry = gain
        } else {
            realized += (avgPrice - price) * closed; // short: bought below entry = gain
        }
        position += signedQty;

        if (position == 0) {
            avgPrice = 0;
        } else if (closed < qty) { // went through flat: the rest is a new position
            avgPrice = price;
        }
    }

    // position is signed, so a short gains when the mark is below its entry
    double unrealized(int64_t mark) {
        return (mark - avgPrice) * position;
    }

    int64_t pos() { return position; }
    double avg() { return avgPrice; }
    double realizedPnl() { return realized; }
};

// (2) one fill that opened a position and is not fully closed yet
struct Lot {
    int64_t sign; // +1 = bought (long lot), -1 = sold (short lot)
    int64_t price;
    int64_t qty; // still open
};

class FifoPnl {
private:
    deque<Lot> lots; // open lots, oldest first; all of them have the same sign
    int64_t realized; // in ticks * shares, exact: no average, so no double
public:
    FifoPnl() : realized(0) {}

    // T: O(num of lots closed), amortized O(1) per fill; S: O(num of open lots)
    void onFill(Side side, int64_t price, int64_t qty) {
        int64_t sign = (side == Side::BUY) ? 1 : -1;

        // close the oldest lots while the fill goes against them
        while (qty > 0 && !lots.empty() && lots.front().sign != sign) {
            Lot& lot = lots.front();
            int64_t closed = min(qty, lot.qty);
            // long lot gains when the exit is above its price, short lot when below
            realized += (price - lot.price) * closed * lot.sign;
            lot.qty -= closed;
            qty -= closed;
            if (lot.qty == 0) { lots.pop_front(); }
        }

        if (qty > 0) { // what is left opens a new lot
            lots.push_back({sign, price, qty});
        }
    }

    int64_t pos() {
        int64_t total = 0;
        for (const Lot& lot : lots) {
            total += lot.sign * lot.qty;
        }
        return total;
    }

    int64_t realizedPnl() { return realized; }
};

void testState(const string& label, PositionTracker& book, int64_t wantPos,
               double wantAvg, double wantRealized) {
    bool ok = (book.pos() == wantPos && fabs(book.avg() - wantAvg) < 1e-9
               && fabs(book.realizedPnl() - wantRealized) < 1e-9);
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void check(const string& label, bool ok) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    PositionTracker book;
    testState("flat", book, 0, 0, 0);

    book.onFill(Side::BUY, 10, 100);
    testState("open-long", book, 100, 10, 0);
    book.onFill(Side::BUY, 12, 100);
    testState("add-re-average", book, 200, 11, 0);
    check("unrealized-long", fabs(book.unrealized(13) - 400) < 1e-9);

    book.onFill(Side::SELL, 13, 50);
    testState("close-part", book, 150, 11, 100);

    book.onFill(Side::SELL, 9, 250);
    testState("cross-to-short", book, -100, 9, -200);
    check("unrealized-short", fabs(book.unrealized(8) - 100) < 1e-9);

    book.onFill(Side::BUY, 9, 100);
    testState("back-to-flat", book, 0, 0, -200);

    // (2) same first three fills: FIFO closes the 10 lot first, so realized differs
    FifoPnl fifo;
    fifo.onFill(Side::BUY, 10, 100);
    fifo.onFill(Side::BUY, 12, 100);
    fifo.onFill(Side::SELL, 13, 50);
    check("fifo-oldest-lot", fifo.pos() == 150 && fifo.realizedPnl() == 150);

    fifo.onFill(Side::SELL, 9, 250);
    check("fifo-cross-to-short", fifo.pos() == -100 && fifo.realizedPnl() == -200);
    fifo.onFill(Side::BUY, 8, 100);
    check("fifo-close-short", fifo.pos() == 0 && fifo.realizedPnl() == -100);
}
