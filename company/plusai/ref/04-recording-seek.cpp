// PlusAI (JD): Recording Seek (Medium). Pattern: binary search (lower / upper bound).
// A recording's message timestamps are sorted and may repeat. seek(t) = index of the
// first message with timestamp >= t (n if none); countInRange(t1, t2) = number of
// messages with t1 <= ts <= t2 (t1 <= t2).

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// This is LC 34 First and Last Position: one lower-bound template on [lo, hi); for
// integers, "first ts > t2" is "first ts >= t2 + 1". T: O(log n), S: O(1).
int seek(const vector<int>& ts, int t) {
    int lo = 0;
    int hi = ts.size();
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (ts[mid] < t) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return lo;
}

int countInRange(const vector<int>& ts, int t1, int t2) {
    return seek(ts, t2 + 1) - seek(ts, t1);
}

void test(const string& label, const vector<int>& ts, int t1, int t2, int wantSeek,
          int wantCount) {
    bool ok = seek(ts, t1) == wantSeek && countInRange(ts, t1, t2) == wantCount;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("dups-exact", {1, 3, 3, 3, 7}, 3, 3, 1, 3);
    test("gap-between", {1, 3, 3, 3, 7}, 4, 6, 4, 0);
    test("past-end", {1, 3, 3, 3, 7}, 8, 10, 5, 0);
    test("covers-all", {1, 3, 3, 3, 7}, 0, 7, 0, 5);
}

// follow-up: the recording is a multi-GB file -> keep a sparse chunk index (first
//   timestamp + byte offset of each chunk, as MCAP does); binary search the index, then
//   scan one chunk
