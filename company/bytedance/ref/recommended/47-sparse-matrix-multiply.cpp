// ByteDance (FastPrep, phone) / LC 311: mat1 is m x k and mat2 is k x n, mostly zeros.
// Return mat1 * mat2 as the full m x n matrix (zeros included), skipping zero entries
// instead of doing all m * k * n multiplications.
// Follow-up idea: store each matrix as compressed rows (list of (col, value) per row),
// which is exactly what the nonzero lists below are.

#include <iostream>
#include <vector>
#include <string>
#include <utility>

using std::cout;
using std::string;
using std::vector;
using std::pair;

// Precompute the nonzeros of each mat2 row. Then every nonzero mat1[i][p] only meets
// the nonzeros of mat2 row p. T: O(m * k + k * n + sum over nonzero mat1[i][p] of
// nnz(mat2 row p)), S: O(nnz(mat2)).
vector<vector<int>> multiply(const vector<vector<int>>& mat1,
                             const vector<vector<int>>& mat2) {
    int m = mat1.size();
    int k = mat2.size();
    int n = mat2[0].size();

    // rowNonzeros[p] = (column, value) pairs of the nonzero entries of mat2 row p
    vector<vector<pair<int, int>>> rowNonzeros(k);
    for (int p = 0; p < k; p++) {
        for (int j = 0; j < n; j++) {
            if (mat2[p][j] != 0) {
                rowNonzeros[p].push_back({j, mat2[p][j]});
            }
        }
    }

    vector<vector<int>> result(m, vector<int>(n, 0));
    for (int i = 0; i < m; i++) {
        for (int p = 0; p < k; p++) {
            if (mat1[i][p] == 0) {
                continue;
            }
            for (auto [j, value] : rowNonzeros[p]) {
                result[i][j] += mat1[i][p] * value;
            }
        }
    }
    return result;
}

void test(const string& label, const vector<vector<int>>& mat1,
          const vector<vector<int>>& mat2, const vector<vector<int>>& want) {
    cout << label << (multiply(mat1, mat2) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("one-term", {{0, 3}, {0, 0}}, {{9, 9}, {1, 0}}, {{3, 0}, {0, 0}});
    test("row-times-col", {{1, 2, 3}}, {{1}, {0}, {2}}, {{7}});
    test("all-zero", {{0, 0}}, {{0}, {0}}, {{0}});
    test("negatives", {{-1, 0}, {0, 2}}, {{0, 4}, {-3, 0}}, {{0, -4}, {-6, 0}});
}
