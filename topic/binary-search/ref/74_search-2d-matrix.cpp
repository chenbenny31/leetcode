// LC 74 Search a 2D Matrix: m x n matrix with each row sorted and each row's first value
// greater than the previous row's last. Is target in it? O(log(m * n)).
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// Treat the matrix as one sorted array of m * n cells; cell k is (k / n, k % n).
// Closed-interval binary search. T: O(log(m * n)), S: O(1).
bool searchMatrix(const vector<vector<int>>& matrix, int target) {
    int m = matrix.size();
    int n = matrix[0].size();
    int lo = 0;
    int hi = m * n - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        int val = matrix[mid / n][mid % n];
        if (val == target) { return true; }
        if (val < target) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return false;
}

void test(const string& label, const vector<vector<int>>& matrix, int target, bool want) {
    if (searchMatrix(matrix, target) == want) {
        cout << label << " PASS\n";
    } else {
        cout << label << " FAIL\n";
    }
}

int main() {
    test("middle", {{1, 3, 5}, {7, 9, 11}}, 9, true);
    test("last-cell", {{1, 3, 5}, {7, 9, 11}}, 11, true);
    test("gap-between-rows", {{1, 3, 5}, {7, 9, 11}}, 6, false);
    test("single-cell", {{4}}, 5, false);
}
