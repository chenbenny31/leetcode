// CSV to ASCII table: each row is unquoted comma-separated fields (same count per row).
// Pad each field to its column's longest width, left-aligned, as "| a  | b |" with one
// space around every field. Return one string per row, no border rows.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;

// Split every row with a character loop, take each column's max width, then pad.
// T: O(total characters), S: O(total characters).
vector<string> formatCsvTable(const vector<string>& rows) {
    // table[r][c] = field c of row r
    vector<vector<string>> table;
    for (const string& row : rows) {
        vector<string> fields;
        string field;
        for (char ch : row) {
            if (ch == ',') {
                fields.push_back(field);
                field = "";
            } else {
                field += ch;
            }
        }
        fields.push_back(field);
        table.push_back(fields);
    }
    int cols = table[0].size();
    // width[c] = longest field in column c
    vector<int> width(cols, 0);
    for (const auto& fields : table) {
        for (int c = 0; c < cols; c++) {
            width[c] = max(width[c], (int)fields[c].size());
        }
    }
    vector<string> result;
    for (const auto& fields : table) {
        string line = "|";
        for (int c = 0; c < cols; c++) {
            string pad(width[c] - (int)fields[c].size(), ' ');
            line += " " + fields[c] + pad + " |";
        }
        result.push_back(line);
    }
    return result;
}

void test(const string& label, const vector<string>& rows, const vector<string>& want) {
    cout << label << (formatCsvTable(rows) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("header-wider", {"id,name", "1,Bo"}, {"| id | name |", "| 1  | Bo   |"});
    test("empty-field", {"a,", "b,c"}, {"| a |   |", "| b | c |"});
    test("one-column", {"x", "yy"}, {"| x  |", "| yy |"});
}
