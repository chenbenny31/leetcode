// LC 88: arr1 holds m sorted values followed by room for n more; merge the n sorted
// values of arr2 into arr1 in place, sorted ascending.
// Follow-ups: both arrays sorted descending; both arrays unsorted.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::sort;

// Fill arr1 from the back with the larger tail value, so nothing unread is overwritten.
// T: O(m + n), S: O(1).
void merge(vector<int>& arr1, int m, const vector<int>& arr2, int n) {
    // read1 / read2 = last unread value of arr1 / arr2; write = next slot to fill
    int read1 = m - 1;
    int read2 = n - 1;
    for (int write = m + n - 1; read2 >= 0; write--) {
        if (read1 >= 0 && arr1[read1] > arr2[read2]) {
            arr1[write] = arr1[read1];
            read1--;
        } else {
            arr1[write] = arr2[read2];
            read2--;
        }
    }
}

// follow-up: descending inputs; the tails are now the smallest values, so place the
// smaller one at the back. T: O(m + n), S: O(1).
void mergeDescending(vector<int>& arr1, int m, const vector<int>& arr2, int n) {
    int read1 = m - 1;
    int read2 = n - 1;
    for (int write = m + n - 1; read2 >= 0; write--) {
        if (read1 >= 0 && arr1[read1] < arr2[read2]) {
            arr1[write] = arr1[read1];
            read1--;
        } else {
            arr1[write] = arr2[read2];
            read2--;
        }
    }
}

// follow-up: unsorted inputs; nothing to merge, so copy arr2 into the free slots and
// sort. T: O((m + n) log(m + n)), S: O(log(m + n)) for sort's recursion.
void mergeUnsorted(vector<int>& arr1, int m, const vector<int>& arr2, int n) {
    for (int idx = 0; idx < n; idx++) { arr1[m + idx] = arr2[idx]; }
    sort(arr1.begin(), arr1.begin() + m + n);
}

void test(const string& label, vector<int> arr1, int m, const vector<int>& arr2,
          const vector<int>& want) {
    merge(arr1, m, arr2, arr2.size());
    cout << label << (arr1 == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("interleave", {1, 4, 0, 0}, 2, {2, 3}, {1, 2, 3, 4});
    test("all-smaller", {5, 6, 0}, 2, {1}, {1, 5, 6});
    test("empty-first", {0, 0}, 0, {1, 2}, {1, 2});
    test("empty-second", {3}, 1, {}, {3});
}
