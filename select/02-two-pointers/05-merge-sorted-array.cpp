// LC 88 Merge Sorted Array (Easy) — two pointers filling from the back
// nums1 = m sorted values + n free slots; merge sorted nums2 (n values) into it in place
//
// clarify:
// m or n = 0 -> ok
// dups across arrays -> ok
// extra space -> O(1)
// nums1 size -> exactly m + n

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// core idea: fill nums1 from the back w/ the larger tail value; nothing unread is lost
// T: O(m + n), S: O(1)
void merge(vector<int>& nums1, int m, const vector<int>& nums2, int n) {
    // read1 / read2 = idx of last unread value in nums1 / nums2
    int read1 = m - 1;
    int read2 = n - 1;
    // stop when nums2 is used up: rest of nums1 is already in place
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

// follow-up:
// why from the back -> free slots are at the back; front fill overwrites unread nums1
// descending inputs -> place the smaller tail value at the back instead
// merge two sorted linked lists (LC 21) -> dummy head, append the smaller node
