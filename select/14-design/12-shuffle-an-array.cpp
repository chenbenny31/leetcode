// LC 384 Shuffle an Array (Medium) — randomness: Fisher–Yates
// reset() returns the original array; shuffle() returns a uniform random permutation
//
// clarify:
// uniform over all n! permutations -> Fisher–Yates, not swap-with-any
// dups in the array -> fine, positions are shuffled
// shuffle the stored copy or a fresh one? -> fresh copy each call
// which random source? -> rand() here; mt19937 in production

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>

using std::cout;
using std::string;
using std::vector;
using std::sort;
using std::swap;
using std::rand;

// core idea: Fisher–Yates: for i = n - 1 down to 1, swap i w/ a uniform pick of 0..i
// n * (n - 1) * ... * 1 = n! equally likely outcomes
// T: O(n) per call, S: O(n)
class Shuffler {
    // the array as given, never modified
    vector<int> original;

public:
    Shuffler(const vector<int>& nums) : original(nums) {}

    vector<int> reset() { return original; }

    vector<int> shuffle() {
        vector<int> shuffled = original;
        for (int i = (int)shuffled.size() - 1; i >= 1; i--) {
            // % (i + 1), not % n: idx i itself must be a possible pick
            swap(shuffled[i], shuffled[rand() % (i + 1)]);
        }
        return shuffled;
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    vector<int> cards = {1, 2, 3, 4, 5};
    Shuffler shuffler(cards);
    vector<int> shuffled = shuffler.shuffle();
    sort(shuffled.begin(), shuffled.end());
    check(shuffled == cards, "shuffle-is-permutation");
    check(shuffler.reset() == cards, "reset-returns-original");
    vector<int> one = {7};
    Shuffler single(one);
    check(single.shuffle() == one, "single");
    return 0;
}

// follow-up:
// why swap i w/ any idx 0..n-1 is biased -> n^n paths, not a multiple of n!
// random node of a stream (LC 382) -> reservoir sampling: keep i-th item w/ prob 1 / i
// random pick idx of a target (LC 398) -> reservoir over matches, or map value -> idxs
// weighted pick (LC 528) -> prefix sums of weights, upper_bound on rand() % total
// O(1) insert / delete / getRandom (LC 380) -> 05-insert-delete-getrandom
// rand10 from rand7 (LC 470) -> (rand7() - 1) * 7 + rand7() uniform on 1..49, retry > 40
