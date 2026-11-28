// 33 · Matching engine — quant core (not reported)
// one symbol, limit orders, price-time priority
// submit(id, side, price, qty): trade against the other side while prices cross, best
//   price first and the oldest order first inside one price, at the resting order's
//   price; what is left of the new order rests in the book
// cancel(id): take a resting order out
// modify(id, newPrice, newQty): less qty at the same price keeps its place in the queue;
//   anything else is cancel + submit, so it goes to the back and may trade
//
// clarify:
// order types -> limit only; partial fills are fine
// price -> int num of ticks; an id is unique among resting orders
// time -> arrival order, kept by the queue itself, no timestamp needed
// bad order (qty <= 0, id already resting) -> no trades, no change

#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <iterator>

using std::cout;
using std::string;
using std::vector;
using std::list;
using std::map;
using std::unordered_map;
using std::min;
using std::prev;

enum class Side { BUY, SELL };

struct Order {
    int id;
    Side side;
    int price; // in ticks
    int qty;   // open qty
};

struct Trade {
    int makerId; // the order that was resting
    int takerId; // the order that came in
    int price;   // the maker's price
    int qty;
};

// core idea: per side a sorted map price -> queue of resting orders (oldest first),
// plus id -> the order's node in its queue so cancel does not search
// T: submit / modify O(log L + num of fills), cancel O(log L), L = num of price levels
// S: O(resting orders)
class MatchingEngine {
    map<int, list<Order>> bids;                      // price -> resting buys
    map<int, list<Order>> asks;                      // price -> resting sells
    unordered_map<int, list<Order>::iterator> where; // id -> its node in a queue

    // fill the taker from one price level, oldest maker first
    // return the taker's qty that is still open
    int fillLevel(list<Order>& level, int takerId, int qty, vector<Trade>& trades) {
        while (qty > 0 && !level.empty()) {
            Order& maker = level.front();
            int traded = min(qty, maker.qty);
            trades.push_back({maker.id, takerId, maker.price, traded});
            maker.qty -= traded;
            qty -= traded;
            if (maker.qty == 0) {
                where.erase(maker.id);
                level.pop_front();
            }
        }
        return qty;
    }

public:
    // return the trades this order caused, in the order they happened
    vector<Trade> submit(int id, Side side, int price, int qty) {
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

        // what is left joins the back of its price level
        if (qty > 0) {
            list<Order>& level = (side == Side::BUY) ? bids[price] : asks[price];
            level.push_back({id, side, price, qty});
            where[id] = prev(level.end());
        }
        return trades;
    }

    // false if the id is not resting (unknown, filled or cancelled)
    bool cancel(int id) {
        auto found = where.find(id);
        if (found == where.end()) { return false; }
        list<Order>::iterator node = found->second;
        map<int, list<Order>>& levels = (node->side == Side::BUY) ? bids : asks;
        auto level = levels.find(node->price);
        level->second.erase(node);
        if (level->second.empty()) { levels.erase(level); }
        where.erase(found);
        return true;
    }

    // false if the id is not resting or newQty <= 0
    bool modify(int id, int newPrice, int newQty, vector<Trade>& trades) {
        auto found = where.find(id);
        if (found == where.end() || newQty <= 0) { return false; }
        list<Order>::iterator node = found->second;
        if (newPrice == node->price && newQty <= node->qty) {
            node->qty = newQty;
            return true;
        }
        // copy the side before cancel frees the node
        Side side = node->side;
        cancel(id);
        trades = submit(id, side, newPrice, newQty);
        return true;
    }

    // false if there is no bid
    bool bestBid(int& price) const {
        if (bids.empty()) { return false; }
        price = prev(bids.end())->first;
        return true;
    }

    // false if there is no ask
    bool bestAsk(int& price) const {
        if (asks.empty()) { return false; }
        price = asks.begin()->first;
        return true;
    }
};

bool sameTrades(const vector<Trade>& got, const vector<Trade>& want) {
    if (got.size() != want.size()) { return false; }
    for (int i = 0; i < (int)got.size(); i++) {
        if (got[i].makerId != want[i].makerId || got[i].takerId != want[i].takerId) {
            return false;
        }
        if (got[i].price != want[i].price || got[i].qty != want[i].qty) { return false; }
    }
    return true;
}

void test(const string& label, MatchingEngine& engine, const Order& order,
          const vector<Trade>& want) {
    vector<Trade> got = engine.submit(order.id, order.side, order.price, order.qty);
    cout << label << (sameTrades(got, want) ? " PASS\n" : " FAIL\n");
}

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    MatchingEngine engine;
    int price = 0;
    test("rest-1", engine, {1, Side::SELL, 101, 5}, {});
    test("rest-2", engine, {2, Side::SELL, 101, 5}, {});
    test("rest-3", engine, {3, Side::SELL, 102, 5}, {});
    test("no-cross", engine, {4, Side::BUY, 100, 5}, {});
    test("oldest-first", engine, {5, Side::BUY, 101, 7},
         {{1, 5, 101, 5}, {2, 5, 101, 2}});
    test("sweep-levels", engine, {6, Side::BUY, 103, 10},
         {{2, 6, 101, 3}, {3, 6, 102, 5}});
    check(engine.bestBid(price) && price == 103, "rest-is-left");
    check(!engine.bestAsk(price), "asks-empty");
    test("sell-hits-bids", engine, {7, Side::SELL, 100, 4},
         {{6, 7, 103, 2}, {4, 7, 100, 2}});
    check(engine.cancel(4) && !engine.bestBid(price), "cancel");
    check(!engine.cancel(4) && !engine.cancel(1), "cancel-gone-or-filled");

    MatchingEngine amend;
    vector<Trade> trades;
    amend.submit(1, Side::SELL, 101, 5);
    amend.submit(2, Side::SELL, 101, 5);
    amend.submit(3, Side::SELL, 101, 5);
    check(amend.modify(1, 101, 2, trades) && trades.empty(), "modify-less-qty");
    check(amend.modify(2, 101, 6, trades) && trades.empty(), "modify-more-qty");
    test("less-keeps-place", amend, {4, Side::BUY, 101, 3},
         {{1, 4, 101, 2}, {3, 4, 101, 1}});

    amend.submit(5, Side::BUY, 100, 4);
    bool modified = amend.modify(5, 101, 4, trades);
    check(modified && sameTrades(trades, {{3, 5, 101, 4}}), "modify-price-trades");
}

// follow-up:
// 1. market order -> a limit with price +inf (buy) / 0 (sell); never rests
// 2. IOC -> submit, then drop what is left; FOK -> check the fillable qty first
// 3. depth snapshot -> sum the qty of each level, top k levels (20-order-book)
// 4. faster cancel -> where also stores the level iterator, no map lookup
// 5. many symbols -> hash map symbol -> engine, one thread per symbol shard
// 6. self-trade prevention -> skip or cancel when maker and taker share an owner
