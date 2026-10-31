// ByteDance phone (FT): parse one valid CSV record and return its decoded fields in
// order. (The report only said "implement a data parser"; the CSV rules are
// FastPrep's.)
// - Fields are separated by commas.
// - A field may be wrapped in double quotes; a quoted field may contain commas, and
//   two double quotes "" inside it stand for one literal ".
// - An unquoted field contains no comma and no double quote; a quoted field closes
//   right before a comma or the end of the line.
// - Spaces and every other character are kept as is; backslash is not special.
// - Empty fields are kept (at the start, between commas, at the end); an empty record
//   is one empty field.
//   parseCsvRecord(line) -> vector<string>

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// Read field by field with one index. At the start of a field, a quote means "quoted
// field": copy characters until a lone quote closes it, turning "" into ". Otherwise
// copy until the next comma. After a field we are at a comma (skip it, another field
// follows - possibly an empty last one) or at the end of the line.
// T: O(n), S: O(n).
vector<string> parseCsvRecord(const string& line) {
    int n = line.size();
    vector<string> fields;
    int i = 0;
    while (true) {
        string field;
        if (i < n && line[i] == '"') {
            // quoted field: skip the opening quote
            i++;
            while (i < n) {
                if (line[i] != '"') {
                    field += line[i];
                    i++;
                } else if (i + 1 < n && line[i + 1] == '"') {
                    // escaped quote
                    field += '"';
                    i += 2;
                } else {
                    // closing quote
                    i++;
                    break;
                }
            }
        } else {
            while (i < n && line[i] != ',') {
                field += line[i];
                i++;
            }
        }
        fields.push_back(field);

        if (i == n) {
            break;
        }
        // line[i] is a comma: one more field follows
        i++;
    }
    return fields;
}

void test(const string& label, const string& line, const vector<string>& want) {
    cout << label << (parseCsvRecord(line) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("plain", "a,bc,d", {"a", "bc", "d"});
    test("quoted-comma", "\"x,y\",z", {"x,y", "z"});
    test("escaped-quote", "\"a\"\"b\"", {"a\"b"});
    test("empty-fields", ",,", {"", "", ""});
    test("empty-record", "", {""});
    test("empty-quoted", "\"\",1", {"", "1"});
    test("keep-spaces", " a ,\\b", {" a ", "\\b"});
}
