// LC 378 Kth Smallest Element in a Sorted Matrix: n x n matrix with rows and columns
// sorted ascending. Return the kth smallest element (counting duplicates).
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// How many cells are <= x: walk from the bottom-left, up when too big, right otherwise.
// T: O(n), S: O(1).
int countAtMost(const vector<vector<int>>& matrix, long long x) {
    int n = matrix.size();
    int r = n - 1;
    int c = 0;
    int cnt = 0;
    while (r >= 0 && c < n) {
        if (matrix[r][c] <= x) {
            cnt += r + 1;   // the whole column above is <= x too
            c++;
        } else {
            r--;
        }
    }
    return cnt;
}

// Binary search on the answer value: the smallest x with at least k cells <= x.
// T: O(n * log(max - min)), S: O(1).
int kthSmallest(const vector<vector<int>>& matrix, int k) {
    int n = matrix.size();
    long long lo = matrix[0][0];
    long long hi = matrix[n - 1][n - 1];
    while (lo < hi) {
        long long mid = lo + (hi - lo) / 2;
        if (countAtMost(matrix, mid) >= k) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    return lo;
}

void test(const string& label, const vector<vector<int>>& matrix, int k, int want) {
    if (kthSmallest(matrix, k) == want) {
        cout << label << " PASS\n";
    } else {
        cout << label << " FAIL\n";
    }
}

int main() {
    test("middle", {{1, 5}, {2, 6}}, 3, 5);
    test("last", {{1, 5}, {2, 6}}, 4, 6);
    test("duplicates", {{1, 1}, {1, 2}}, 3, 1);
    test("single-cell", {{-5}}, 1, -5);
}
