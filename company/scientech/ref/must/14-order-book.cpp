// order book (book building): keep one symbol's book in sync with an exchange feed
// feed msgs: add(id, side, price, qty), cancel(id), reduce(id, qty) = part of it traded
// queries: best bid / best ask (price + total qty), total qty at a price

// usage:
// g++ -std=c++17 -Wall -Wextra 14-order-book.cpp -o a && ./a
//
// OrderBook book;  book.add(1, Side::BUY, 100, 5);  book.bestBid(price, qty);

// clarify / assumptions
// one symbol; price is an int num of ticks (no double), qty in shares
// msgs arrive in order; this book never matches, the exchange already did
// bad msg (dup id, unknown id, qty <= 0, reduce more than open) -> false, no change

// data:
// orders: id -> {side, price, qty}       find an order by id in O(1)
// bids / asks: price -> total open qty   sorted by price, best price at one end

// T: add / cancel / reduce O(log L), best bid / ask O(1), L = num of price levels
// S: O(num of open orders + L)

#include <cstdint>
#include <string>
#include <map>
#include <unordered_map>
#include <iostream>

using std::int64_t;
using std::cout;
using std::map;
using std::string;
using std::unordered_map;

enum class Side { BUY, SELL };

struct Order {
    Side side;
    int64_t price; // in ticks
    int64_t qty; // open qty
};

class OrderBook {
private:
    map<int64_t, int64_t> bids; // price -> total open qty, best = highest = last
    map<int64_t, int64_t> asks; // price -> total open qty, best = lowest = first
    unordered_map<int, Order> orders; // order id -> open order

    // take qty off a price level, drop the level when nothing is left
    void removeQty(Side side, int64_t price, int64_t qty) {
        map<int64_t, int64_t>& levels = (side == Side::BUY) ? bids : asks;
        auto level = levels.find(price);
        level->second -= qty;
        if (level->second == 0) { levels.erase(level); }
    }
public:
    bool add(int id, Side side, int64_t price, int64_t qty) {
        if (qty <= 0 || orders.count(id) > 0) { return false; }

        orders[id] = {side, price, qty};
        if (side == Side::BUY) {
            bids[price] += qty;
        } else {
            asks[price] += qty;
        }
        return true;
    }

    bool cancel(int id) {
        auto found = orders.find(id);
        if (found == orders.end()) { return false; }

        Order& order = found->second;
        removeQty(order.side, order.price, order.qty);
        orders.erase(found);
        return true;
    }

    // qty of the order traded: comes off the order and off its level
    bool reduce(int id, int64_t qty) {
        auto found = orders.find(id);
        if (found == orders.end()) { return false; }

        Order& order = found->second;
        if (qty <= 0 || qty > order.qty) { return false; }

        removeQty(order.side, order.price, qty);
        order.qty -= qty;
        if (order.qty == 0) { orders.erase(found); } // fully traded
        return true;
    }

    // return false if there is no bid
    bool bestBid(int64_t& price, int64_t& qty) {
        if (bids.empty()) { return false; }
        auto best = bids.rbegin(); // highest price
        price = best->first;
        qty = best->second;
        return true;
    }

    // return false if there is no ask
    bool bestAsk(int64_t& price, int64_t& qty) {
        if (asks.empty()) { return false; }
        auto best = asks.begin(); // lowest price
        price = best->first;
        qty = best->second;
        return true;
    }

    // 0 if nobody is at that price
    int64_t qtyAt(Side side, int64_t price) {
        map<int64_t, int64_t>& levels = (side == Side::BUY) ? bids : asks;
        auto level = levels.find(price);
        if (level == levels.end()) { return 0; }
        return level->second;
    }
};

bool bidIs(OrderBook& book, int64_t wantPrice, int64_t wantQty) {
    int64_t price = 0;
    int64_t qty = 0;
    return book.bestBid(price, qty) && price == wantPrice && qty == wantQty;
}

bool askIs(OrderBook& book, int64_t wantPrice, int64_t wantQty) {
    int64_t price = 0;
    int64_t qty = 0;
    return book.bestAsk(price, qty) && price == wantPrice && qty == wantQty;
}

void check(const string& label, bool ok) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    OrderBook book;
    int64_t price = 0;
    int64_t qty = 0;
    check("empty", !book.bestBid(price, qty) && !book.bestAsk(price, qty));

    // test valid cases: bids 100 x 5, 100 x 3, 99 x 4; asks 102 x 6, 103 x 2
    book.add(1, Side::BUY, 100, 5);
    book.add(2, Side::BUY, 100, 3);
    book.add(3, Side::BUY, 99, 4);
    book.add(4, Side::SELL, 102, 6);
    book.add(5, Side::SELL, 103, 2);
    check("best-bid", bidIs(book, 100, 8));
    check("best-ask", askIs(book, 102, 6));
    check("qty-at", book.qtyAt(Side::BUY, 99) == 4 && book.qtyAt(Side::SELL, 101) == 0);

    book.reduce(1, 2);
    check("reduce", bidIs(book, 100, 6));
    book.cancel(2);
    check("cancel", bidIs(book, 100, 3));
    book.reduce(1, 3);
    check("level-gone", bidIs(book, 99, 4));

    // test invalid cases
    check("dup-id", !book.add(3, Side::BUY, 98, 1));
    check("unknown-id", !book.cancel(1) && !book.reduce(42, 1));
    check("reduce>open", !book.reduce(3, 5) && bidIs(book, 99, 4));
    check("qty<=0", !book.add(9, Side::SELL, 104, 0));
}
