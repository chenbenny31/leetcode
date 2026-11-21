// 10 · Minimum Replacements To Sort The Array (Moveworks OA, Hard) — LC 2366, greedy
// one operation replaces x by two positive ints summing to x; return the min operations
// that make the array non-decreasing. [3, 9, 3] -> 2, [3, 4, 3] -> 2
//
// clarify:
// values positive, up to 1e9; n up to 1e5 -> answer may pass 2^31, use long long
// pieces stay in place (x -> a, b at x's spot) -> yes
// already sorted -> 0

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// core idea: the last element never needs splitting; walk right to left w/ prev = the
// smallest value allowed after x. Split x into the fewest pieces <= prev:
// parts = ceil(x / prev); make the pieces as even as possible so the first (smallest)
// piece, x / parts, is as large as possible -> it becomes the new prev
// T: O(n), S: O(1)
long long minimumReplacement(const vector<int>& nums) {
    int n = nums.size();
    long long ops = 0;
    long long prev = nums[n - 1];
    for (int i = n - 2; i >= 0; i--) {
        long long x = nums[i];
        if (x <= prev) {
            prev = x;
            continue;
        }
        long long parts = (x + prev - 1) / prev;
        ops += parts - 1;
        prev = x / parts;
    }
    return ops;
}

void test(const string& label, const vector<int>& nums, long long want) {
    cout << label << (minimumReplacement(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("split-9", {3, 9, 3}, 2);
    test("split-4", {3, 4, 3}, 2);
    test("sorted", {1, 2, 3}, 0);
    test("chain", {12, 9, 7, 6, 17, 19, 21}, 6);
    return 0;
}

// follow-up:
// why the first piece is x / parts -> even split; any other split w/ the same count
//   has a smaller minimum and only makes earlier elements split more
// why right to left -> the last element is never split, so it fixes the first bound
// return the final array -> push pieces (x / parts, plus 1 on the last x % parts ones)
