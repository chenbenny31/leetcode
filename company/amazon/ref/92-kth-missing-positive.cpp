// LC 1539: arr is strictly increasing positive integers. Return the k-th positive
// integer missing from arr.

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// straightforward: walk 1, 2, 3, ... counting the missing ones. T: O(n + k), S: O(1).
int findKthPositive(const vector<int>& arr, int k) {
    int i = 0, n = arr.size();
    for (int x = 1;; x++) {
        if (i < n && arr[i] == x) {
            i++;
        } else {
            k--;
            if (k == 0) { return x; }
        }
    }
}

// improved: arr[i] - (i + 1) numbers are missing before arr[i] and that grows with i;
// binary search the first i where it reaches k, the answer is k + i.
// T: O(log n), S: O(1).
int findKthPositiveBinary(const vector<int>& arr, int k) {
    int lo = 0, hi = (int)arr.size() - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] - (mid + 1) < k) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return k + lo;
}

void test(const string& label, const vector<int>& arr, int k, int want) {
    bool ok = findKthPositive(arr, k) == want && findKthPositiveBinary(arr, k) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("inside", {2, 3, 7}, 3, 5);
    test("after", {1, 2, 3}, 2, 5);
    test("before", {5, 6}, 1, 1);
}
