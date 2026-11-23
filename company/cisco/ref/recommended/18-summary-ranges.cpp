// 18 · Compress Consecutive Ranges (Cisco intern OA, Easy) — LC 228, scan runs
// print each run of consecutive values (next = prev + 1) as "a to b", a single value as
// "a", one per line: [1,2,3,6,7,8,10,15] -> "1 to 3", "6 to 8", "10", "15"
//
// clarify:
// sorted? -> NOT guaranteed (a hidden case was unsorted); keep input order, don't sort
// run = next value is exactly prev + 1 in input order -> assumption from the report
// output format -> "a to b" (LC 228 uses "a->b"); the OA checked the format strictly
// negative values -> allowed, to_string handles the sign
// empty input -> no lines

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;
using std::to_string;

// core idea: two idx; j runs to the last value of the run that starts at i
// T: O(n), S: O(n) — output
vector<string> compressRanges(const vector<int>& nums) {
    int n = nums.size();
    vector<string> out;
    int i = 0;
    while (i < n) {
        int j = i;
        // next - 1, not cur + 1: cur may be INT_MAX
        while (j + 1 < n && nums[j + 1] - 1 == nums[j]) { j++; }
        string range = to_string(nums[i]);
        if (j > i) { range += " to " + to_string(nums[j]); }
        out.push_back(range);
        i = j + 1;
    }
    return out;
}

void test(const string& label, const vector<int>& nums, const vector<string>& want) {
    cout << label << (compressRanges(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("sorted", {1, 2, 3, 6, 7, 8, 10, 15}, {"1 to 3", "6 to 8", "10", "15"});
    test("unsorted-input-order", {6, 7, 1, 2, 9}, {"6 to 7", "1 to 2", "9"});
    test("negative", {-3, -2, 0}, {"-3 to -2", "0"});
    test("empty", {}, {});
    return 0;
}

// follow-up:
// 1. printing -> for (const str& line : compressRanges(nums)) cout << line << "\n";
// 2. sort first -> sort, then skip equal neighbors inside a run
// 3. missing ranges in [lo, hi] (LC 163) -> walk prev = lo - 1, emit gap prev + 1..x - 1
// 4. values arrive as a stream (LC 352) -> ordered map start -> end, merge with neighbors
