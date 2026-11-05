// ByteDance phone (FT): parse one valid CSV record into its fields. Fields are split by
// commas; a field wrapped in double quotes may contain commas, and "" inside it means
// one literal ". Everything else is kept as is; empty fields are kept ("" -> {""}).
//   parseCsvRecord(line) -> vector<string>

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// One index, field by field: quoted field -> copy until a lone quote, "" -> ";
// otherwise copy until the next comma. T: O(n), S: O(n).
vector<string> parseCsvRecord(const string& line) {
    int n = line.size();
    vector<string> fields;
    int i = 0;
    bool moreFields = true;
    while (moreFields) {
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

        // at a comma another field follows (maybe an empty last one); at the end we stop
        moreFields = i < n;
        i++;
    }
    return fields;
}

void test(const string& label, const string& line, const vector<string>& want) {
    cout << label << (parseCsvRecord(line) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("plain", " a ,bc,d", {" a ", "bc", "d"});
    test("quoted-comma", "\"x,y\",z", {"x,y", "z"});
    test("escaped-quote", "\"a\"\"b\"", {"a\"b"});
    test("empty-fields", ",,", {"", "", ""});
    test("empty-record", "", {""});
}

// follow-up: multi-line file, '\n' inside quotes -> state machine over the whole input;
//   '\n' ends a record only when not inQuotes
// follow-up: malformed input (stray '"', text after closing quote) -> explicit states
//   START / UNQUOTED / QUOTED / AFTER_QUOTE, reject on bad transition
// follow-up: write fields back as CSV -> quote a field holding ',' '"' or '\n', double
//   every '"' inside
