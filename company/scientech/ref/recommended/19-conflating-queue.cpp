// conflating queue: quotes arrive faster than the consumer reads them; the consumer only
// cares about the newest price of each symbol, so older unread quotes are overwritten
// push(quote) from the feed; pop(quote) hands out one symbol's newest unread quote

// usage:
// g++ -std=c++17 -Wall -Wextra 19-conflating-queue.cpp -o a && ./a
//
// ConflatingQueue queue;  queue.push({"AAPL", 1, 100});  queue.pop(quote);

// clarify / assumptions
// one unread quote per symbol at most; a newer one replaces it in place
// pop order: the symbol that has been waiting the longest goes first; an update
//   does not send a waiting symbol to the back
// quotes can arrive out of order: time not newer than the last one seen -> dropped
// one thread here; with a feed thread and a consumer thread, one mutex around both ops

// data:
// unread: symbol -> its newest quote that was not popped yet
// waiting: symbols that have an unread quote, longest waiting first
// lastTime: symbol -> time of the newest quote ever pushed, to drop late ones

// T: push / pop O(1) average, S: O(num of symbols), no matter how many quotes arrive

#include <cstdint>
#include <queue>
#include <string>
#include <unordered_map>
#include <iostream>

using std::int64_t;
using std::cout;
using std::queue;
using std::string;
using std::unordered_map;

struct Quote {
    string symbol;
    int64_t time;
    int64_t price; // in ticks
};

class ConflatingQueue {
private:
    unordered_map<string, Quote> unread; // symbol -> newest quote not popped yet
    queue<string> waiting; // symbols with an unread quote, longest waiting first
    unordered_map<string, int64_t> lastTime; // symbol -> newest time ever pushed
public:
    void push(const Quote& quote) {
        auto seen = lastTime.find(quote.symbol);
        if (seen != lastTime.end() && quote.time <= seen->second) { return; } // late
        lastTime[quote.symbol] = quote.time;

        if (unread.count(quote.symbol) == 0) { // not waiting yet: joins the line
            waiting.push(quote.symbol);
        }
        unread[quote.symbol] = quote; // replaces the older unread quote, if any
    }

    // return false if nothing is unread
    bool pop(Quote& quote) {
        if (waiting.empty()) { return false; }

        string symbol = waiting.front();
        waiting.pop();
        auto found = unread.find(symbol);
        quote = found->second;
        unread.erase(found);
        return true;
    }
};

void testPop(const string& label, ConflatingQueue& queue, const string& wantSymbol,
             int64_t wantPrice) {
    Quote quote;
    bool popped = queue.pop(quote);
    bool ok = (popped && quote.symbol == wantSymbol && quote.price == wantPrice);
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void check(const string& label, bool ok) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    ConflatingQueue queue;
    Quote quote;
    check("empty", !queue.pop(quote));

    // test valid cases: AAPL gets 3 quotes before anyone reads
    queue.push({"AAPL", 1, 100});
    queue.push({"MSFT", 2, 400});
    queue.push({"AAPL", 3, 101});
    queue.push({"AAPL", 4, 102});
    testPop("newest-only", queue, "AAPL", 102);
    testPop("keeps-its-turn", queue, "MSFT", 400);
    check("drained", !queue.pop(quote));

    queue.push({"AAPL", 5, 103});
    testPop("waits-again", queue, "AAPL", 103);

    // test dropped cases
    queue.push({"MSFT", 6, 401});
    queue.push({"MSFT", 5, 399});
    testPop("late-quote-dropped", queue, "MSFT", 401);
    queue.push({"MSFT", 6, 402});
    check("same-time-dropped", !queue.pop(quote));
}
