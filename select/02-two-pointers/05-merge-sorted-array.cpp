// LC 88 Merge Sorted Array (Easy) — two pointers filling from the back
// nums1 holds m sorted values followed by n free slots; merge the n sorted values of
// nums2 into nums1 in place, ascending.
// Clarify: m or n may be 0; duplicates across the arrays; O(1) extra space; nums1 has
//   exactly m + n slots.

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// Core idea: fill nums1 from the back with the larger tail value, so nothing unread is
// overwritten. T: O(m + n), S: O(1).
void merge(vector<int>& nums1, int m, const vector<int>& nums2, int n) {
    // last unread value in nums1 / nums2
    int read1 = m - 1;
    int read2 = n - 1;
    // stop when nums2 is used up: the rest of nums1 is already in place
    for (int write = m + n - 1; read2 >= 0; write--) {
        if (read1 >= 0 && nums1[read1] > nums2[read2]) {
            nums1[write] = nums1[read1];
            read1--;
        } else {
            nums1[write] = nums2[read2];
            read2--;
        }
    }
}

void test(const string& label, vector<int> nums1, int m, const vector<int>& nums2,
          const vector<int>& want) {
    merge(nums1, m, nums2, nums2.size());
    cout << label << (nums1 == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("interleave", {1, 4, 0, 0}, 2, {2, 3}, {1, 2, 3, 4});
    test("all-smaller", {5, 6, 0}, 2, {1}, {1, 5, 6});
    test("empty-first", {0, 0}, 0, {1, 2}, {1, 2});
    test("empty-second", {3}, 1, {}, {3});
    return 0;
}

// follow-up (ideas only):
// - why from the back -> the free slots are at the back; filling from the front would
//   overwrite unread nums1 values
// - descending inputs -> place the smaller tail value at the back instead
// - merge two sorted linked lists (LC 21) -> dummy head, append the smaller node
