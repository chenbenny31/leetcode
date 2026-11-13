// LC 384 Shuffle an Array (Medium) — randomness: Fisher–Yates
// reset() returns the original array; shuffle() returns a random permutation of it in
// which each of the n! permutations is equally likely.
// Clarify: uniform over all permutations; duplicates in the array; shuffle the stored
//   copy or a fresh one each call; which random source is allowed.

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

// Core idea: Fisher–Yates: for i from n - 1 down to 1, position i gets a uniform pick
// among the i + 1 values not placed yet, giving n * (n - 1) * ... * 1 = n! equally likely
// outcomes. T: O(n) per call, S: O(n).
class Shuffler {
    // the array as given, never modified
    vector<int> original;

public:
    Shuffler(const vector<int>& nums) : original(nums) {}

    vector<int> reset() { return original; }

    vector<int> shuffle() {
        vector<int> shuffled = original;
        for (int i = (int)shuffled.size() - 1; i >= 1; i--) {
            // % (i + 1), not % n: index i itself must be a possible pick
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

// follow-up (ideas only):
// - why swapping every i with any index 0..n-1 is biased -> it has n^n equally likely
//   paths, and n^n is not a multiple of n!, so some permutations get more paths
// - random node of a stream / linked list of unknown length (LC 382) -> reservoir
//   sampling: keep the i-th item with probability 1 / i
// - random pick index of a target (LC 398) -> reservoir over the matching indices, or a
//   map value -> indices when there are many queries
// - weighted pick (LC 528) -> prefix sums of the weights, upper_bound on rand() % total
// - O(1) insert / delete / getRandom (LC 380) -> 05-insert-delete-getrandom
// - rand10 from rand7 (LC 470) -> rejection sampling: (rand7() - 1) * 7 + rand7() is
//   uniform on 1..49; retry above 40, else answer 1 + (x - 1) % 10
