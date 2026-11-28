// 06 · Feed sequencer — JD practice (duplicates and ordering)
// msgs carry seq 1, 2, 3, ...; the network may deliver them out of order or twice
// onMessage(seq, payload) returns the payloads that can be handed on now, in seq order,
// each exactly once
//
// clarify:
// seq -> starts at 1, goes up by 1, never wraps; payload is one int here
// msg that came too early -> kept until the gap before it is filled
// old or repeated seq -> dropped, the first copy wins
// gap that never fills -> not handled here (follow-up 1)

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;

// core idea: wait for nextSeq; park early msgs in a hash map, and after each new msg
// hand on the run that starts at nextSeq
// T: O(1) amortized per msg (each msg is stored and handed on once), S: O(msgs waiting)
class FeedSequencer {
    long long nextSeq = 1;                 // the seq we are waiting for
    unordered_map<long long, int> pending; // seq -> payload of msgs that came too early

public:
    vector<int> onMessage(long long seq, int payload) {
        vector<int> ready;
        if (seq < nextSeq || pending.count(seq) > 0) { return ready; }
        pending[seq] = payload;
        // stop at the first missing seq
        while (pending.count(nextSeq) > 0) {
            ready.push_back(pending[nextSeq]);
            pending.erase(nextSeq);
            nextSeq++;
        }
        return ready;
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    FeedSequencer feed;
    check(feed.onMessage(1, 10) == vector<int>{10}, "in-order");
    check(feed.onMessage(3, 30).empty(), "too-early");
    check(feed.onMessage(3, 31).empty(), "dup-waiting");
    check(feed.onMessage(2, 20) == vector<int>{20, 30}, "gap-filled");
    check(feed.onMessage(2, 21).empty(), "old-seq");
}

// follow-up:
// 1. gap never fills -> timestamp the gap; after a timeout request a resend or skip it
// 2. bound the memory -> cap pending size; past it, declare the gap lost, jump nextSeq
// 3. is there a gap now -> !pending.empty(); missing seqs = nextSeq .. min pending - 1
// 4. redundant feeds A and B -> same object for both, the first copy of each seq wins
// 5. data quality report -> counters for dups, late msgs and the longest gap
// 6. records keyed by (symbol, time), not seq -> hash set of seen keys to drop repeats
