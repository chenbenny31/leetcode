// Cloudflare (reported): Ring Buffer RTT Average (Easy). Pattern: circular array + sum.
// An edge probe keeps the last N round-trip-time samples (ms) of a link. add(rtt) stores
// a sample, overwriting the oldest once N are held; average() returns the integer mean of
// the samples currently held (0 when empty).

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// This is a fixed-size circular array plus a running sum. T: O(1) per op, S: O(N).
class RttWindow {
    // samples[(head + i) % cap] = i-th oldest sample held
    vector<int> samples;
    // N: max samples held
    int cap;
    // slot of the oldest sample
    int head = 0;
    // samples currently held, <= cap
    int count = 0;
    // sum of the samples currently held
    long long sum = 0;

public:
    RttWindow(int n) : samples(n, 0), cap(n) {}

    void add(int rtt) {
        if (count < cap) {
            samples[(head + count) % cap] = rtt;
            count++;
        } else {
            // full: the oldest slot is reused, so the next oldest becomes head
            sum -= samples[head];
            samples[head] = rtt;
            head = (head + 1) % cap;
        }
        sum += rtt;
    }

    long long average() {
        if (count == 0) { return 0; }
        return sum / count;
    }
};

void test(const string& label, int n, const vector<int>& rtts,
          const vector<long long>& want) {
    RttWindow window(n);
    vector<long long> got;
    for (int rtt : rtts) {
        window.add(rtt);
        got.push_back(window.average());
    }
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("filling", 3, {10, 20, 30}, {10, 15, 20});
    test("wrap-twice", 2, {10, 20, 40, 60, 80}, {10, 15, 30, 50, 70});
    test("size-one", 1, {5, 7}, {5, 7});
}

// follow-up: window = last 10 s instead of last N -> deque of (time, rtt); pop old ones.
// follow-up: live with one writer + many readers -> mutex, or SPSC ring + atomic head.
