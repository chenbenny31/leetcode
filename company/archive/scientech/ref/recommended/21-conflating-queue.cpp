// 21 · Conflating queue — quant core (reported at Citadel Securities, not here)
// quotes arrive faster than the consumer reads them, and the consumer only wants the
// newest price of each symbol: push(symbol, price) overwrites that symbol's unread
// price, pop hands out one symbol with its newest unread price
//
// clarify:
// pop order -> the symbol waiting the longest goes first; an update keeps its place
// nothing unread -> pop returns false
// quotes -> arrive in time order (late ones are follow-up 1)
// threads -> one here; a feed thread + a consumer thread need one mutex around both ops

#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>

using std::cout;
using std::string;
using std::queue;
using std::unordered_map;

// core idea: hash map symbol -> newest unread price, plus a queue of the symbols that
// have one; a symbol joins the queue only when it has nothing unread
// T: O(1) average per op, S: O(num of symbols) however many quotes arrive
class ConflatingQueue {
    unordered_map<string, int> unread; // symbol -> newest price not popped yet
    queue<string> waiting;             // symbols with an unread price, longest wait first

public:
    void push(const string& symbol, int price) {
        if (unread.count(symbol) == 0) { waiting.push(symbol); }
        unread[symbol] = price;
    }

    // false if nothing is unread
    bool pop(string& symbol, int& price) {
        if (waiting.empty()) { return false; }
        symbol = waiting.front();
        waiting.pop();
        price = unread[symbol];
        unread.erase(symbol);
        return true;
    }
};

bool popIs(ConflatingQueue& quotes, const string& wantSymbol, int wantPrice) {
    string symbol;
    int price = 0;
    return quotes.pop(symbol, price) && symbol == wantSymbol && price == wantPrice;
}

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    ConflatingQueue quotes;
    string symbol;
    int price = 0;
    check(!quotes.pop(symbol, price), "empty");

    quotes.push("AAPL", 100);
    quotes.push("MSFT", 400);
    quotes.push("AAPL", 101);
    quotes.push("AAPL", 102);
    check(popIs(quotes, "AAPL", 102), "newest-only");
    check(popIs(quotes, "MSFT", 400), "keeps-its-turn");
    check(!quotes.pop(symbol, price), "drained");

    quotes.push("AAPL", 103);
    check(popIs(quotes, "AAPL", 103), "waits-again");
}

// follow-up:
// 1. quotes out of order -> also keep symbol -> last time; drop a quote with time <= it
// 2. an update moves the symbol to the back -> list + map symbol -> list iterator (LRU)
// 3. bid and ask per quote -> the map value becomes a Quote struct, replaced as a whole
// 4. feed thread + consumer thread -> one mutex around push and pop
// 5. latest record per key in a batch file -> the same map, last write wins
