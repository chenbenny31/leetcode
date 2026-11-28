// seq -> starts at 1, goes up by 1, never wraps
// msg comes early -> keep until the gap
// old or dup seq -> dropped, only first copy wins
// gap never fills -> not handled

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;

// wait for nextSeq, park early msgs in hash-map, after each new msg hand on the run at nextReq
class FeedSequencer {
private:
    long long nextSeq = 1; // the seq waiting for
    unordered_map<long long, int> pending; // seq -> payload of early msgs
    
public:
    vector<int> onMessage(long long seq, int payload) {
        vector<int> ready;
        if (seq < nextSeq || pending.count(seq) > 0) { return ready; }
        pending[seq] = payload;
        // stop at first missing seq
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
