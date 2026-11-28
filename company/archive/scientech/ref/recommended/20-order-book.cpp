// 20 · Order book — quant core (not reported)
// keep one symbol's book in sync with an exchange feed: add(id, side, price, qty),
// cancel(id), reduce(id, qty) = part of it traded
// queries: best bid / best ask (price + total qty), total qty at a price
//
// clarify:
// matching -> never; the exchange already did, msgs arrive in order
// price -> int num of ticks (no double); qty and level totals fit in int
// order id -> 64-bit on real feeds, so long long (same as 05-feed-parser)
// bad msg (dup id, unknown id, qty <= 0, reduce more than open) -> false, no change

#include <iostream>
#include <string>
#include <map>
#include <unordered_map>

using std::cout;
using std::string;
using std::map;
using std::unordered_map;

enum class Side { BUY, SELL };

struct Order {
    Side side;
    int price; // in ticks
    int qty;   // open qty
};

// core idea: id -> order finds an order in O(1); per side a sorted map price -> total
// open qty, so the best price sits at one end
// T: add / cancel / reduce O(log L), best bid / ask O(1), L = num of price levels
// S: O(open orders + L)
class OrderBook {
    map<int, int> bids;                     // price -> total open qty, best = highest
    map<int, int> asks;                     // price -> total open qty, best = lowest
    unordered_map<long long, Order> orders; // order id -> open order

    // take qty off a price level, drop the level when nothing is left
    void removeQty(Side side, int price, int qty) {
        map<int, int>& levels = (side == Side::BUY) ? bids : asks;
        auto level = levels.find(price);
        level->second -= qty;
        if (level->second == 0) { levels.erase(level); }
    }

public:
    bool add(long long id, Side side, int price, int qty) {
        if (qty <= 0 || orders.count(id) > 0) { return false; }
        orders[id] = {side, price, qty};
        if (side == Side::BUY) {
            bids[price] += qty;
        } else {
            asks[price] += qty;
        }
        return true;
    }

    bool cancel(long long id) {
        auto it = orders.find(id);
        if (it == orders.end()) { return false; }
        Order& order = it->second;
        removeQty(order.side, order.price, order.qty);
        orders.erase(it);
        return true;
    }

    // qty of the order traded: it comes off the order and off its level
    bool reduce(long long id, int qty) {
        auto it = orders.find(id);
        if (it == orders.end()) { return false; }
        Order& order = it->second;
        if (qty <= 0 || qty > order.qty) { return false; }
        removeQty(order.side, order.price, qty);
        order.qty -= qty;
        if (order.qty == 0) { orders.erase(it); }
        return true;
    }

    // false if there is no bid
    bool bestBid(int& price, int& qty) const {
        if (bids.empty()) { return false; }
        price = bids.rbegin()->first;
        qty = bids.rbegin()->second;
        return true;
    }

    // false if there is no ask
    bool bestAsk(int& price, int& qty) const {
        if (asks.empty()) { return false; }
        price = asks.begin()->first;
        qty = asks.begin()->second;
        return true;
    }

    // 0 if nobody is at that price
    int qtyAt(Side side, int price) const {
        const map<int, int>& levels = (side == Side::BUY) ? bids : asks;
        auto level = levels.find(price);
        if (level == levels.end()) { return 0; }
        return level->second;
    }
};

bool bidIs(const OrderBook& book, int wantPrice, int wantQty) {
    int price = 0;
    int qty = 0;
    return book.bestBid(price, qty) && price == wantPrice && qty == wantQty;
}

bool askIs(const OrderBook& book, int wantPrice, int wantQty) {
    int price = 0;
    int qty = 0;
    return book.bestAsk(price, qty) && price == wantPrice && qty == wantQty;
}

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    OrderBook book;
    int price = 0;
    int qty = 0;
    check(!book.bestBid(price, qty) && !book.bestAsk(price, qty), "empty");

    book.add(1, Side::BUY, 100, 5);
    book.add(2, Side::BUY, 100, 3);
    book.add(3, Side::BUY, 99, 4);
    book.add(4, Side::SELL, 102, 6);
    book.add(5, Side::SELL, 103, 2);
    check(bidIs(book, 100, 8) && askIs(book, 102, 6), "best-prices");
    check(book.qtyAt(Side::BUY, 99) == 4 && book.qtyAt(Side::SELL, 101) == 0, "qty-at");

    book.reduce(1, 2);
    book.cancel(2);
    check(bidIs(book, 100, 3), "reduce-and-cancel");
    book.reduce(1, 3);
    check(bidIs(book, 99, 4), "level-gone");

    check(!book.add(3, Side::BUY, 98, 1) && !book.cancel(1), "dup-and-unknown-id");
    check(!book.reduce(3, 5) && bidIs(book, 99, 4), "reduce-more-than-open");
}

// follow-up:
// 1. top k levels -> walk k steps from bids.rbegin() / asks.begin()
// 2. spread and mid -> best ask - best bid; (best bid + best ask) / 2.0
// 3. change an order's price -> cancel + add; it loses its place in the queue
// 4. match incoming orders -> a queue of orders per price level (33-matching-engine)
// 5. faster than map -> array indexed by price tick around the touch, O(1) update
// 6. many symbols -> hash map symbol -> OrderBook
