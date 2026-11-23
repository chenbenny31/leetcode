// feed sequencer: market data msgs carry seq 1, 2, 3, ...; the network may deliver
// them out of order or twice (e.g. two redundant feeds A and B with the same msgs)
// onMessage(seq, payload) -> the msgs that can be handed on now, in seq order, each once

// usage:
// g++ -std=c++17 -Wall -Wextra 15-feed-sequencer.cpp -o a && ./a
//
// FeedSequencer feed;  ready = feed.onMessage(seq, payload);  feed.hasGap();

// clarify / assumptions
// seq starts at 1 and goes up by 1; payload is one int here
// a msg that came too early is kept until the gap before it is filled
// old or repeated seq -> dropped, nothing is handed on
// a gap that never fills (lost msg) -> follow-up: time out, ask for a resend

// data:
// nextSeq: the seq we are waiting for
// pending: seq -> payload of msgs that came too early

// T: O(1) amortized per msg (each msg is stored and handed on once)
// S: O(num of msgs waiting behind a gap)

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>

using std::int64_t;
using std::uint64_t;
using std::cout;
using std::string;
using std::unordered_map;
using std::vector;

class FeedSequencer {
private:
    uint64_t nextSeq; // the seq we are waiting for
    unordered_map<uint64_t, int64_t> pending; // seq -> payload, came too early
public:
    FeedSequencer() : nextSeq(1) {}

    vector<int64_t> onMessage(uint64_t seq, int64_t payload) {
        vector<int64_t> ready;
        if (seq < nextSeq) { return ready; } // already handed on
        if (pending.count(seq) > 0) { return ready; } // already waiting

        pending[seq] = payload;

        // hand on the run that starts at nextSeq, stop at the first missing seq
        auto found = pending.find(nextSeq);
        while (found != pending.end()) {
            ready.push_back(found->second);
            pending.erase(found);
            nextSeq++;
            found = pending.find(nextSeq);
        }
        return ready;
    }

    // true while some msg waits behind a missing seq
    bool hasGap() {
        return !pending.empty();
    }
};

void testMsg(const string& label, FeedSequencer& feed, uint64_t seq, int64_t payload,
             const vector<int64_t>& want) {
    cout << label << (feed.onMessage(seq, payload) == want ? " PASS\n" : " FAIL\n");
}

void check(const string& label, bool ok) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    FeedSequencer feed;

    // test valid cases: payload = seq * 10
    testMsg("in-order", feed, 1, 10, {10});
    testMsg("too-early", feed, 3, 30, {});
    check("gap-open", feed.hasGap());
    testMsg("gap-filled", feed, 2, 20, {20, 30});
    check("gap-closed", !feed.hasGap());

    testMsg("two-early-6", feed, 6, 60, {});
    testMsg("two-early-5", feed, 5, 50, {});
    testMsg("run-of-three", feed, 4, 40, {40, 50, 60});

    // test dropped cases
    testMsg("old-seq", feed, 2, 20, {});
    testMsg("wait-8", feed, 8, 80, {});
    testMsg("dup-waiting", feed, 8, 81, {});
    testMsg("first-copy-wins", feed, 7, 70, {70, 80});
}
