// Cloudflare (JD): TCP Segment Reassembly (Medium). Pattern: ordered map of intervals.
// Segments (seq, len) cover bytes [seq, seq + len) and arrive in any order, possibly
// overlapping or duplicated; the stream starts at seq 0. After each arrival return the
// number of contiguous bytes deliverable from 0 (the next expected seq).

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <utility>
#include <algorithm>

using std::cout;
using std::map;
using std::max;
using std::min;
using std::pair;
using std::string;
using std::vector;

// This is merge-intervals on an ordered map: each segment absorbs the buffered intervals
// it overlaps or touches; a merged run starting at `delivered` is handed over.
// T: O(log n) amortized per segment, S: O(n) buffered intervals.
class Reassembler {
    // out-of-order bytes held: start -> end (exclusive); disjoint, all start > delivered
    map<int, int> buffered;
    // bytes [0, delivered) are contiguous and delivered
    int delivered = 0;

public:
    int receive(int seq, int len) {
        // bytes before delivered are duplicates
        int start = max(seq, delivered);
        int end = seq + len;
        if (start >= end) { return delivered; }
        // the interval just before start may still reach into [start, end)
        auto it = buffered.lower_bound(start);
        if (it != buffered.begin()) {
            auto before = it;
            before--;
            if (before->second >= start) { it = before; }
        }
        // <= end: an interval that only touches end is merged too
        while (it != buffered.end() && it->first <= end) {
            start = min(start, it->first);
            end = max(end, it->second);
            it = buffered.erase(it);
        }
        if (start == delivered) {
            delivered = end;
        } else {
            buffered[start] = end;
        }
        return delivered;
    }
};

void test(const string& label, const vector<pair<int, int>>& segments,
          const vector<int>& want) {
    Reassembler stream;
    vector<int> got;
    for (const auto& [seq, len] : segments) {
        got.push_back(stream.receive(seq, len));
    }
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("in-order", {{0, 3}, {3, 2}}, {3, 5});
    test("gap-then-fill", {{5, 5}, {0, 5}}, {0, 10});
    test("overlap-and-dup", {{0, 4}, {2, 4}, {2, 4}}, {4, 6, 6});
    test("bridges-two-held", {{2, 2}, {6, 2}, {3, 4}, {0, 2}}, {0, 0, 0, 8});
}

// follow-up: real 32-bit seq wraps around -> compare with (int32_t)(a - b) < 0 (serial
//   arithmetic); bound memory by dropping bytes beyond delivered + receive window.
// follow-up: return the bytes too -> store the payload per interval, slice on merge.
