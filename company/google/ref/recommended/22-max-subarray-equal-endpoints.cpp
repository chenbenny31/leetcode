// Maximum Subarray Sum With Equal Endpoint Values (Google, interview report)
// Given a non-empty integer array a, return (i, j) with 0 <= i <= j < n and a[i] == a[j]
// that maximizes a[i] + ... + a[j]. i == j is allowed, so an answer always exists.
// Ties: any best pair may be returned. Sums can exceed int (n 1e5, |a[i]| 1e6).
// Part 2, LC 3026 Maximum Good Subarray Sum: k >= 1; return the max sum of nums[i..j]
// with |nums[i] - nums[j]| == k, or 0 if no such pair (|nums[i]|, k up to 1e9).

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <utility>
#include <algorithm>
#include <climits>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::pair;
using std::max;

// straightforward: fix i, extend j while keeping the running sum of a[i..j].
// T: O(n^2), S: O(1) (this is also the O(1)-extra-space follow-up).
pair<int, int> bestEqualEnds(const vector<int>& a) {
    int n = a.size();
    pair<int, int> best = {0, 0};
    long long bestSum = LLONG_MIN;
    for (int i = 0; i < n; i++) {
        // sum of a[i..j]
        long long sum = 0;
        for (int j = i; j < n; j++) {
            sum += a[j];
            if (a[i] == a[j] && sum > bestSum) {
                bestSum = sum;
                best = {i, j};
            }
        }
    }
    return best;
}

// improved: sum(i..j) = prefix[j + 1] - prefix[i], so for each j the best start is the
// earlier occurrence of a[j] with the smallest prefix before it. T: O(n), S: O(n).
pair<int, int> bestEqualEndsFast(const vector<int>& a) {
    int n = a.size();
    // value -> {smallest prefix sum before an occurrence of value, that occurrence}
    unordered_map<int, pair<long long, int>> bestStart;
    // sum of a[0..j-1]
    long long prefix = 0;
    pair<int, int> best = {0, 0};
    long long bestSum = LLONG_MIN;
    for (int j = 0; j < n; j++) {
        // offer j itself as a start before using the map, so i == j is covered
        if (!bestStart.count(a[j]) || prefix < bestStart[a[j]].first) {
            bestStart[a[j]] = {prefix, j};
        }
        auto [startPrefix, i] = bestStart[a[j]];
        prefix += a[j];
        if (prefix - startPrefix > bestSum) {
            bestSum = prefix - startPrefix;
            best = {i, j};
        }
    }
    return best;
}

// part 2: the same idea as bestEqualEndsFast with two lookups per j, the start value is
// nums[j] - k or nums[j] + k. Keys are long long since nums[j] + k can pass 2^31.
// T: O(n), S: O(n).
long long maxGoodSubarraySum(const vector<int>& nums, int k) {
    // value -> smallest prefix sum before an occurrence of value
    unordered_map<long long, long long> minPrefix;
    // sum of nums[0..j-1]
    long long prefix = 0;
    long long best = LLONG_MIN;
    for (int value : nums) {
        if (!minPrefix.count(value) || prefix < minPrefix[value]) {
            minPrefix[value] = prefix;
        }
        prefix += value;
        auto low = minPrefix.find((long long)value - k);
        if (low != minPrefix.end()) { best = max(best, prefix - low->second); }
        auto high = minPrefix.find((long long)value + k);
        if (high != minPrefix.end()) { best = max(best, prefix - high->second); }
    }
    return best == LLONG_MIN ? 0 : best;
}

void test(const string& label, const vector<int>& a, pair<int, int> want) {
    bool ok = bestEqualEnds(a) == want && bestEqualEndsFast(a) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void testGood(const string& label, const vector<int>& nums, int k, long long want) {
    cout << label << (maxGoodSubarraySum(nums, k) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("single", {7}, {0, 0});
    test("span", {2, 5, 2}, {0, 2});
    test("later-start", {3, -10, 3, 1, 3}, {2, 4});
    test("all-neg", {-2, -1, -3, -2}, {1, 1});
    test("long-sum", vector<int>(3000, 1000000), {0, 2999});

    testGood("good-span", {1, 5, 3}, 2, 9);
    testGood("good-none", {4, 4}, 1, 0);
    testGood("good-negative", {-1, -5, -3}, 2, -8);
    testGood("good-wide", {-1000000000, 0, 1000000000}, 1000000000, 1000000000);
}

// follow-up: O(1) extra space -> the straightforward double loop above (a hash map or
//   prefix array is what costs O(n); without them, n starts times n ends)
