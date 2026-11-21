// 19 · K-Step Bit Flip With Retention (Cisco phone screen 2026-11, Medium) — cycle skip
// binary string s; run k steps labelled k, k-1, ..., 1. A step w/ an odd label keeps the
// leftmost '1', an even label keeps the leftmost '0' (none -> keep the last bit); every
// other bit flips. k up to 1e9: return s after all k steps
//
// clarify:
// statement known only from a summary -> confirm "odd / even" refers to the label
// len n -> assumed small enough to store O(n) states of len n
// none of the wanted bit -> keep the last bit (given)
// k == 0 -> s unchanged

#include <iostream>
#include <string>
#include <unordered_map>

using std::cout;
using std::string;
using std::unordered_map;

// one step. T: O(n), S: O(n)
string flipStep(const string& s, long long label) {
    int n = s.size();
    char want = label % 2 == 1 ? '1' : '0';
    int keep = n - 1;
    for (int i = 0; i < n; i++) {
        if (s[i] == want) {
            keep = i;
            break;
        }
    }
    string out = s;
    for (int i = 0; i < n; i++) {
        if (i != keep) { out[i] = out[i] == '0' ? '1' : '0'; }
    }
    return out;
}

// solution 1: simulate; only for small k. T: O(k * n), S: O(n)
string simulate(string s, long long k) {
    for (long long label = k; label >= 1; label--) { s = flipStep(s, label); }
    return s;
}

// solution 2: the next state depends only on (string, label parity), so once that pair
// repeats the steps cycle; skip whole cycles, simulate the rest
// T: O(n * S), S: O(n * S) — S = distinct states before the repeat; the kept bit moves
// right each step, so S is about O(n)
string kStepFlip(string s, long long k) {
    // (string + parity tag) -> step idx when it was seen
    unordered_map<string, long long> seen;
    long long i = 0;
    while (i < k) {
        long long label = k - i;
        string key = s + (label % 2 == 1 ? 'o' : 'e');
        auto it = seen.find(key);
        if (it != seen.end()) {
            // same parity at both ends, so the cycle len is even and parities line up
            long long cycle = i - it->second;
            long long rest = (k - i) % cycle;
            for (long long t = 0; t < rest; t++) { s = flipStep(s, label - t); }
            return s;
        }
        seen[key] = i;
        s = flipStep(s, label);
        i++;
    }
    return s;
}

void test(const string& label, const string& got, const string& want) {
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("one-odd-step", kStepFlip("0101", 1), "1110");
    test("two-steps", kStepFlip("0101", 2), "1111");
    test("none-keeps-last", kStepFlip("0000", 1), "1110");
    test("zero-steps", kStepFlip("10", 0), "10");
    test("big-k-matches-sim", kStepFlip("0110100", 1001), simulate("0110100", 1001));
    test("even-big-k", kStepFlip("1011", 1000), simulate("1011", 1000));
    return 0;
}

// follow-up:
// why the key needs the parity -> the same string at an odd and an even label steps
//   differently, so it is not the same state
// huge n -> store a hash of the string, then confirm the match w/ a direct compare
// closed form -> after the kept bit reaches the end the string alternates w/ period 2;
//   find that step and use (remaining steps) % 2
