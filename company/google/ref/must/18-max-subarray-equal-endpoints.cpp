// Maximum Subarray Sum With Equal Endpoint Values (Google, interview report)
// (1) non-empty a: return (i, j), i <= j, a[i] == a[j], w/ max sum of a[i..j]
// (2) LC 3026: k >= 1; max sum of nums[i..j] w/ |nums[i] - nums[j]| == k; 0 if no pair
//
// clarify:
// i == j -> allowed in (1)
// return pair or sum? -> pair in (1), sum in (2)
// ties -> any best pair
// sums past int -> long long
// (2) best may be negative -> LLONG_MIN sentinel, not 0

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

// (1) solution 1: brute force; O(1) space
// core idea: fix i, extend j, keep running sum of a[i..j]
// T: O(n^2), S: O(1)
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

// (1) solution 2: prefix sum + hash map; one pass
// core idea: sum(i..j) = prefix[j + 1] - prefix[i]; per value keep its min prefix before
// best start for j = occurrence of a[j] (at or before j) w/ smallest prefix before it
// T: O(n), S: O(n)
pair<int, int> bestEqualEndsFast(const vector<int>& a) {
    int n = a.size();
    // value -> {smallest prefix sum before an occurrence of value, that occurrence}
    unordered_map<int, pair<long long, int>> bestStart;
    // sum of a[0..j-1]
    long long prefix = 0;
    pair<int, int> best = {0, 0};
    long long bestSum = LLONG_MIN;
    for (int j = 0; j < n; j++) {
        // offer j as a start before using the map, so i == j is covered
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

// (2) |nums[i] - nums[j]| == k (LC 3026)
// core idea: same map, 2 lookups per j: start value = nums[j] - k or nums[j] + k
// long long keys: nums[j] + k can pass 2^31
// T: O(n), S: O(n)
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
    test("span", {2, 5, 2}, {0, 2});
    test("later-start", {3, -10, 3, 1, 3}, {2, 4});
    test("all-neg", {-2, -1, -3, -2}, {1, 1});
    test("long-sum", vector<int>(3000, 1000000), {0, 2999});

    testGood("good-span", {1, 5, 3}, 2, 9);
    testGood("good-none", {4, 4}, 1, 0);
    testGood("good-negative", {-1, -5, -3}, 2, -8);
    testGood("good-wide", {-1000000000, 0, 1000000000}, 1000000000, 1000000000);
}

// follow-up:
// O(1) extra space -> brute-force double loop (the map costs O(n))
// i < j required (no pair -> {-1, -1}) -> score j against map BEFORE offering j as start
// stream of values -> already one pass; keep prefix, bestStart, best pair between calls
