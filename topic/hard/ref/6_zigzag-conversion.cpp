// LC 6 Zigzag Conversion: write s in a zigzag over numRows rows (down, then diagonally
// up, repeat), then read it row by row. Return the resulting string.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// Simulate the walk: the row index bounces between 0 and numRows - 1.
// T: O(n), S: O(n).
string convert(string s, int numRows) {
    if (numRows == 1) { return s; }
    vector<string> rows(numRows);   // rows[i] = chars that land on row i, in order
    int row = 0;
    int step = 1;                   // +1 going down, -1 going up
    for (char c : s) {
        rows[row] += c;
        if (row == 0) { step = 1; }
        if (row == numRows - 1) { step = -1; }
        row += step;
    }
    string result;
    for (const auto& r : rows) { result += r; }
    return result;
}

void test(const string& label, const string& s, int numRows, const string& want) {
    cout << label << (convert(s, numRows) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("three-rows", "ABCDEFGHI", 3, "AEIBDFHCG");
    test("two-rows", "ABCDE", 2, "ACEBD");
    test("one-row", "ABC", 1, "ABC");
    test("rows-exceed-length", "AB", 5, "AB");
}
