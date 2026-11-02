// LC 118 Pascal's Triangle: return the first numRows rows.

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// Each inner entry is the sum of the two entries above it. T: O(r^2), S: O(r^2).
vector<vector<int>> generate(int numRows) {
    vector<vector<int>> rows;
    for (int r = 0; r < numRows; r++) {
        vector<int> row(r + 1, 1);
        for (int i = 1; i < r; i++) { row[i] = rows[r - 1][i - 1] + rows[r - 1][i]; }
        rows.push_back(row);
    }
    return rows;
}

void test(const string& label, int numRows, const vector<vector<int>>& want) {
    cout << label << (generate(numRows) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("zero", 0, {});
    test("one", 1, {{1}});
    test("four", 4, {{1}, {1, 1}, {1, 2, 1}, {1, 3, 3, 1}});
}
