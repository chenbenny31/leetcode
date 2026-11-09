// PlusAI (JD): Pre-Trigger Recorder (Easy). Pattern: circular array (ring buffer).
// Frames arrive with increasing timestamps; the ring keeps only the last `capacity`
// (capacity >= 1). trigger(t) returns, oldest first, the frames still buffered with
// timestamp in [t - before, t]: the context recorded just before an event at time t.

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// This is a fixed-size circular array (head + count), like LC 622 Design Circular Queue;
// trigger scans it oldest first. T: addFrame O(1), trigger O(capacity), S: O(capacity).
class Recorder {
    // ring storage: buf[(head + i) % cap] = timestamp of the i-th oldest frame
    vector<int> buf;
    // slots in the ring
    int cap;
    // slot of the oldest frame
    int head = 0;
    // frames currently held, <= cap
    int count = 0;
    // seconds of history an event asks for
    int before;

public:
    Recorder(int capacity, int before) : buf(capacity), cap(capacity), before(before) {}

    void addFrame(int ts) {
        if (count < cap) {
            buf[(head + count) % cap] = ts;
            count++;
            return;
        }
        // full: overwrite the oldest frame, so the next one becomes the oldest
        buf[head] = ts;
        head = (head + 1) % cap;
    }

    vector<int> trigger(int t) {
        vector<int> frames;
        for (int i = 0; i < count; i++) {
            int ts = buf[(head + i) % cap];
            if (ts >= t - before && ts <= t) { frames.push_back(ts); }
        }
        return frames;
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    Recorder rec(3, 5);
    check(rec.trigger(7).empty(), "empty-buffer");
    rec.addFrame(1);
    rec.addFrame(2);
    rec.addFrame(3);
    check(rec.trigger(3) == vector<int>{1, 2, 3}, "all-in-window");
    rec.addFrame(4);
    rec.addFrame(10);
    check(rec.trigger(10) == vector<int>{10}, "window-cuts-old");
    check(rec.trigger(4) == vector<int>{3, 4}, "overwritten-and-future");
}

// follow-up: trigger in O(log capacity + k) -> the ring is sorted by timestamp, so binary
//   search the logical index i over buf[(head + i) % cap] for the first ts >= t - before
// follow-up: also keep `after` seconds past the event -> store pending events and flush
//   [t - before, t + after] once a frame with ts > t + after arrives
