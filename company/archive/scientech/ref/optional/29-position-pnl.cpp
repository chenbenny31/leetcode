// 29 · Position and PnL — quant core (not reported)
// fills (side, price, qty) of one symbol arrive one by one
// (1) average cost: signed position, average entry price, realized PnL; unrealized PnL
//     = what the open position makes at the mark price
// (2) FIFO: a closing fill is matched against the oldest open lot first
//
// clarify:
// cost method -> it decides the realized PnL: (1) and (2) give different numbers
// shorts -> allowed; a fill may cross through flat (long 150, sell 250)
// units -> price in int ticks, PnL in ticks * shares; no fees

#include <iostream>
#include <string>
#include <deque>
#include <algorithm>
#include <cmath>
#include <cstdlib>

using std::cout;
using std::string;
using std::deque;
using std::min;
using std::abs;
using std::fabs;

enum class Side { BUY, SELL };

// ==================== (1) ====================
// average cost: position, average entry price, realized and unrealized PnL

// core idea: a fill in the position's direction adds and re-averages the entry price;
// a fill against it closes: realized += (exit - entry) * closed qty for a long, the
// reverse for a short; what goes past flat opens a new position at the fill price
// T: O(1) per fill, S: O(1)
class PositionTracker {
    int position = 0;    // signed shares: > 0 long, < 0 short, 0 flat
    double avgPrice = 0; // average entry price of the open position, 0 when flat
    double realized = 0; // PnL already locked in by closing fills

public:
    void onFill(Side side, int price, int qty) {
        int signedQty = (side == Side::BUY) ? qty : -qty;
        bool adds = (position >= 0 && signedQty > 0) || (position <= 0 && signedQty < 0);
        if (adds) {
            // total cost / total shares
            double cost = avgPrice * abs(position) + (double)price * qty;
            position += signedQty;
            avgPrice = cost / abs(position);
            return;
        }

        int closed = min(qty, abs(position));
        if (position > 0) {
            realized += (price - avgPrice) * closed;
        } else {
            realized += (avgPrice - price) * closed;
        }
        position += signedQty;

        if (position == 0) {
            avgPrice = 0;
        } else if (closed < qty) {
            // went through flat: the rest is a new position
            avgPrice = price;
        }
    }

    // position is signed, so a short gains when the mark is below its entry
    double unrealized(int mark) const { return (mark - avgPrice) * position; }

    int pos() const { return position; }
    double avg() const { return avgPrice; }
    double realizedPnl() const { return realized; }
};

// ==================== (2) ====================
// FIFO: a closing fill is matched against the oldest open lot first

// one fill that opened a position and is not fully closed yet
struct Lot {
    int sign; // +1 = bought (long lot), -1 = sold (short lot)
    int price;
    int qty;  // still open
};

// core idea: queue of open lots, all with the same sign; a fill against them closes
// from the front, and what is left of the fill opens a new lot at the back
// T: O(1) amortized per fill, S: O(open lots)
class FifoPnl {
    deque<Lot> lots;        // open lots, oldest first
    long long realized = 0; // in ticks * shares; exact, since there is no average

public:
    void onFill(Side side, int price, int qty) {
        int sign = (side == Side::BUY) ? 1 : -1;
        while (qty > 0 && !lots.empty() && lots.front().sign != sign) {
            Lot& lot = lots.front();
            int closed = min(qty, lot.qty);
            // a long lot gains when the exit is above its price, a short lot when below
            realized += 1LL * (price - lot.price) * closed * lot.sign;
            lot.qty -= closed;
            qty -= closed;
            if (lot.qty == 0) { lots.pop_front(); }
        }
        if (qty > 0) { lots.push_back({sign, price, qty}); }
    }

    int pos() const {
        int total = 0;
        for (const Lot& lot : lots) { total += lot.sign * lot.qty; }
        return total;
    }

    long long realizedPnl() const { return realized; }
};

// ==================== tests ====================
bool stateIs(const PositionTracker& book, int wantPos, double wantAvg,
             double wantRealized) {
    return book.pos() == wantPos && fabs(book.avg() - wantAvg) < 1e-9 &&
           fabs(book.realizedPnl() - wantRealized) < 1e-9;
}

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    PositionTracker book;
    book.onFill(Side::BUY, 10, 100);
    book.onFill(Side::BUY, 12, 100);
    check(stateIs(book, 200, 11, 0), "add-re-average");
    check(fabs(book.unrealized(13) - 400) < 1e-9, "unrealized-long");

    book.onFill(Side::SELL, 13, 50);
    check(stateIs(book, 150, 11, 100), "close-part");
    book.onFill(Side::SELL, 9, 250);
    check(stateIs(book, -100, 9, -200), "cross-to-short");
    check(fabs(book.unrealized(8) - 100) < 1e-9, "unrealized-short");
    book.onFill(Side::BUY, 9, 100);
    check(stateIs(book, 0, 0, -200), "back-to-flat");

    FifoPnl fifo;
    fifo.onFill(Side::BUY, 10, 100);
    fifo.onFill(Side::BUY, 12, 100);
    fifo.onFill(Side::SELL, 13, 50);
    check(fifo.pos() == 150 && fifo.realizedPnl() == 150, "fifo-oldest-lot");
    fifo.onFill(Side::SELL, 9, 250);
    check(fifo.pos() == -100 && fifo.realizedPnl() == -200, "fifo-cross-to-short");
    fifo.onFill(Side::BUY, 8, 100);
    check(fifo.pos() == 0 && fifo.realizedPnl() == -100, "fifo-close-short");
}

// follow-up:
// 1. LIFO -> close from the back of the deque instead of the front
// 2. fees -> subtract fee per fill from realized
// 3. many symbols -> hash map symbol -> tracker; total PnL = sum over the symbols
// 4. exact average cost -> keep total cost as an int, divide only when asked
// 5. daily PnL -> realized today + unrealized at the close - unrealized at prev close
