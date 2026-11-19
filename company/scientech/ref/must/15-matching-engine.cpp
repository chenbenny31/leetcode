// matching engine: one symbol, limit orders, price-time priority
// submit(id, side, price, qty): trade against the other side while prices cross
//   (buy price >= best ask, sell price <= best bid): best price first, and the oldest
//   order first inside one price; the trade price is the resting order's price;
//   what is left of the new order rests in the book
// cancel(id): take a resting order out
// modify(id, newPrice, newQty): less qty at the same price keeps its place in the queue;
//   anything else is cancel + submit, so it goes to the back and may trade

// usage:
// g++ -std=c++17 -Wall -Wextra 15-matching-engine.cpp -o a && ./a
//
// MatchingEngine engine;  trades = engine.submit(5, Side::BUY, 101, 7);
// engine.cancel(5);  engine.modify(5, 102, 3, trades);

// clarify / assumptions
// limit orders only (market / IOC / FOK are follow-ups); partial fills are fine
// price is an int num of ticks; an id is unique among resting orders
// "time" = arrival order, kept by the queue itself, no timestamp needed
// bad order (qty <= 0, id already resting) -> no trades, no change

// data:
// bids / asks: price -> queue of resting orders, oldest first
// where: id -> the order's node in its queue, so cancel does not search

// T: submit / modify O(log L + num of fills), cancel O(log L), L = num of price levels
// S: O(num of resting orders)

#include <cstdint>
#include <algorithm>
#include <iterator>
#include <list>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>

using std::int64_t;
using std::cout;
using std::list;
using std::map;
using std::min;
using std::prev;
using std::string;
using std::unordered_map;
using std::vector;

enum class Side { BUY, SELL };

struct Order {
    int id;
    Side side;
    int64_t price; // in ticks
    int64_t qty; // open qty
};

struct Trade {
    int makerId; // the order that was resting
    int takerId; // the order that came in
    int64_t price; // the maker's price
    int64_t qty;
};

class MatchingEngine {
private:
    map<int64_t, list<Order>> bids; // price -> resting buys, oldest first
    map<int64_t, list<Order>> asks; // price -> resting sells, oldest first
    unordered_map<int, list<Order>::iterator> where; // id -> its node in a queue

    // fill the taker from one price level, oldest maker first
    // return the taker's qty that is still open
    int64_t fillLevel(list<Order>& queue, int takerId, int64_t qty,
                      vector<Trade>& trades) {
        while (qty > 0 && !queue.empty()) {
            Order& maker = queue.front();
            int64_t traded = min(qty, maker.qty);
            trades.push_back({maker.id, takerId, maker.price, traded});
            maker.qty -= traded;
            qty -= traded;

            if (maker.qty == 0) { // maker is done, the next oldest moves up
                where.erase(maker.id);
                queue.pop_front();
            }
        }
        return qty;
    }
public:
    // return the trades this order caused, in the order they happened
    vector<Trade> submit(int id, Side side, int64_t price, int64_t qty) {
        vector<Trade> trades;
        if (qty <= 0 || where.count(id) > 0) { return trades; }

        if (side == Side::BUY) {
            // lowest ask first, as long as it is not above the buy price
            while (qty > 0 && !asks.empty() && asks.begin()->first <= price) {
                auto best = asks.begin();
                qty = fillLevel(best->second, id, qty, trades);
                if (best->second.empty()) { asks.erase(best); }
            }
        } else {
            // highest bid first, as long as it is not below the sell price
            while (qty > 0 && !bids.empty() && prev(bids.end())->first >= price) {
                auto best = prev(bids.end());
                qty = fillLevel(best->second, id, qty, trades);
                if (best->second.empty()) { bids.erase(best); }
            }
        }

        if (qty > 0) { // what is left joins the back of its price level
            list<Order>& queue = (side == Side::BUY) ? bids[price] : asks[price];
            queue.push_back({id, side, price, qty});
            where[id] = prev(queue.end());
        }
        return trades;
    }

    // return false if the id is not resting (unknown, filled or cancelled)
    bool cancel(int id) {
        auto found = where.find(id);
        if (found == where.end()) { return false; }

        list<Order>::iterator node = found->second;
        map<int64_t, list<Order>>& levels = (node->side == Side::BUY) ? bids : asks;
        auto level = levels.find(node->price);
        level->second.erase(node); // list erase by iterator is O(1)
        if (level->second.empty()) { levels.erase(level); }
        where.erase(found);
        return true;
    }

    // return false if the id is not resting or newQty <= 0
    bool modify(int id, int64_t newPrice, int64_t newQty, vector<Trade>& trades) {
        auto found = where.find(id);
        if (found == where.end() || newQty <= 0) { return false; }

        list<Order>::iterator node = found->second;
        if (newPrice == node->price && newQty <= node->qty) {
            node->qty = newQty; // only smaller: keeps its place in the queue
            return true;
        }

        Side side = node->side; // copy before cancel frees the node
        cancel(id);
        trades = submit(id, side, newPrice, newQty); // back of the queue, may trade
        return true;
    }

    // return false if there is no bid
    bool bestBid(int64_t& price) {
        if (bids.empty()) { return false; }
        price = prev(bids.end())->first;
        return true;
    }

    // return false if there is no ask
    bool bestAsk(int64_t& price) {
        if (asks.empty()) { return false; }
        price = asks.begin()->first;
        return true;
    }
};

bool sameTrades(const vector<Trade>& got, const vector<Trade>& want) {
    if (got.size() != want.size()) { return false; }
    for (int i = 0; i < (int)got.size(); i++) {
        bool sameIds = (got[i].makerId == want[i].makerId
                        && got[i].takerId == want[i].takerId);
        bool sameFill = (got[i].price == want[i].price && got[i].qty == want[i].qty);
        if (!sameIds || !sameFill) { return false; }
    }
    return true;
}

void testSubmit(const string& label, MatchingEngine& engine, const Order& order,
                const vector<Trade>& want) {
    vector<Trade> got = engine.submit(order.id, order.side, order.price, order.qty);
    cout << label << (sameTrades(got, want) ? " PASS\n" : " FAIL\n");
}

void check(const string& label, bool ok) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    MatchingEngine engine;
    int64_t price = 0;

    // test valid cases: sells 1 and 2 rest at 101 (1 is older), sell 3 at 102
    testSubmit("rest-1", engine, {1, Side::SELL, 101, 5}, {});
    testSubmit("rest-2", engine, {2, Side::SELL, 101, 5}, {});
    testSubmit("rest-3", engine, {3, Side::SELL, 102, 5}, {});
    testSubmit("no-cross", engine, {4, Side::BUY, 100, 5}, {});

    testSubmit("oldest-first", engine, {5, Side::BUY, 101, 7},
               {{1, 5, 101, 5}, {2, 5, 101, 2}});
    testSubmit("sweep-levels", engine, {6, Side::BUY, 103, 10},
               {{2, 6, 101, 3}, {3, 6, 102, 5}});
    check("rest-what-is-left", engine.bestBid(price) && price == 103);
    check("asks-empty", !engine.bestAsk(price));

    testSubmit("sell-hits-bids", engine, {7, Side::SELL, 100, 4},
               {{6, 7, 103, 2}, {4, 7, 100, 2}});
    check("cancel", engine.cancel(4) && !engine.bestBid(price));

    // test invalid cases
    check("cancel-twice", !engine.cancel(4));
    check("cancel-filled", !engine.cancel(1));
    testSubmit("qty<=0", engine, {8, Side::BUY, 100, 0}, {});
    testSubmit("rest-9", engine, {9, Side::BUY, 100, 1}, {});
    testSubmit("dup-id", engine, {9, Side::SELL, 100, 1}, {});
    bool bidStays = (engine.bestBid(price) && price == 100);
    check("dup-no-change", bidStays && !engine.bestAsk(price));

    // test modify: sells 1, 2, 3 rest at 101 in that order
    MatchingEngine amend;
    vector<Trade> trades;
    amend.submit(1, Side::SELL, 101, 5);
    amend.submit(2, Side::SELL, 101, 5);
    amend.submit(3, Side::SELL, 101, 5);
    check("modify-less-qty", amend.modify(1, 101, 2, trades) && trades.empty());
    check("modify-more-qty", amend.modify(2, 101, 6, trades) && trades.empty());
    testSubmit("less-keeps-place", amend, {4, Side::BUY, 101, 3},
               {{1, 4, 101, 2}, {3, 4, 101, 1}});

    amend.submit(5, Side::BUY, 100, 4);
    check("modify-price-trades", amend.modify(5, 101, 4, trades)
          && sameTrades(trades, {{3, 5, 101, 4}}));
    check("modify-unknown", !amend.modify(42, 101, 1, trades));
}
