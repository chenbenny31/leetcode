// arr1 have room for both? yes, size m + n, last n are placeholders
// return new or merged in-place -> in-place into arr1
// sort order -> ascending, follow-up to descending
// can m or n be 0? yes

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::sort;

// fill arr1 from back with larger tail val, no overwritten unread, T: O(m+n), S: O(1)
void merge(vector<int>& arr1, int m, const vector<int>& arr2, int n) {
    int first = m - 1;
    int second = n - 1;

    for (int write = m + n - 1; second >= 0; write--) {
        if (first >= 0 && arr1[first] > arr2[second]) {
            arr1[write] = arr1[first];
            first--;
        } else {
            arr1[write] = arr2[second];
            second--;
        }
    }
}

// follow-up: descending inputs, tail are now min vals, T: O(m+n), S: O(1)
void mergeDescending(vector<int>& arr1, int m, const vector<int>& arr2, int n) {
    int first = m - 1;
    int second = n - 1;

    for (int write = m + n - 1; second >= 0; write--) {
        if (first >= 0 && arr1[first] < arr2[second]) {
            arr1[write] = arr1[first];
            first--;
        } else {
            arr1[write] = arr2[second];
            second--;
        }
    }
}

// follow-up: unsorted inputs, T: O((m+n)log(m+n)), S: O(log(m+n)) for sort-recursion
void mergeUnsorted(vector<int>& arr1, int m, const vector<int>& arr2, int n) {
    for (int i = 0; i < n; i++) {
        arr1[m + i] = arr2[i];
    }
    sort(arr1.begin(), arr1.begin() + m + n); // could have un-used space
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
