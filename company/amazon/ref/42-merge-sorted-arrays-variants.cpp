// LC 88: arr1 holds m sorted values followed by room for n more; merge the n sorted
// values of arr2 into arr1 in place, sorted ascending.
// Follow-ups: both arrays sorted descending; both arrays unsorted.

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// Fill arr1 from the back with the larger tail value, so nothing unread is overwritten.
// T: O(m + n), S: O(1).
void merge(vector<int>& arr1, int m, const vector<int>& arr2, int n) {
    int i = m - 1, j = n - 1;
    for (int k = m + n - 1; j >= 0; k--) {
        if (i >= 0 && arr1[i] > arr2[j]) {
            arr1[k] = arr1[i];
            i--;
        } else {
            arr1[k] = arr2[j];
            j--;
        }
    }
}

// follow-up: descending inputs; the tails are now the smallest values, so place the
// smaller one at the back. T: O(m + n), S: O(1).
void mergeDescending(vector<int>& arr1, int m, const vector<int>& arr2, int n) {
    int i = m - 1, j = n - 1;
    for (int k = m + n - 1; j >= 0; k--) {
        if (i >= 0 && arr1[i] < arr2[j]) {
            arr1[k] = arr1[i];
            i--;
        } else {
            arr1[k] = arr2[j];
            j--;
        }
    }
}

// follow-up: unsorted inputs; append arr2, then insertion-sort in place.
// T: O((m + n)^2), S: O(1).
void mergeUnsorted(vector<int>& arr1, int m, const vector<int>& arr2, int n) {
    for (int j = 0; j < n; j++) { arr1[m + j] = arr2[j]; }
    for (int i = 1; i < m + n; i++) {
        int x = arr1[i];
        int k = i - 1;
        while (k >= 0 && arr1[k] > x) {
            arr1[k + 1] = arr1[k];
            k--;
        }
        arr1[k + 1] = x;
    }
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
