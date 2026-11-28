// 05 · Feed parser — JD practice (ingestion and data quality)
// one text line of an order feed -> a msg; a price becomes an int num of 0.01 ticks
//   A,<id>,<side B|S>,<price>,<qty>  add     A,7,B,100.25,50
//   X,<id>                           cancel  X,7
//   R,<id>,<qty>                     reduce  R,7,20
//
// clarify:
// separators -> ',' only, no spaces; nums are digits only, no sign
// price -> whole part needed, at most 2 decimals: "100.5" -> 10050, "100" -> 10000
// malformed line, or a num past 64 bits -> return false, leave msg alone
// why no double -> 100.01 is not exact in binary; integer ticks stay exact

#include <iostream>
#include <vector>
#include <string>
#include <climits>

using std::cout;
using std::string;
using std::vector;

struct Msg {
    char type = ' ';
    long long id = 0;
    char side = ' ';
    long long price = 0; // in ticks of 0.01
    long long qty = 0;
};

// split on ','; empty fields are kept, so "X,7," has 3 fields
vector<string> splitFields(const string& line) {
    vector<string> fields(1);
    for (char c : line) {
        if (c == ',') {
            fields.push_back("");
        } else {
            fields.back() += c;
        }
    }
    return fields;
}

// digits only, at least one; false when the value would pass LLONG_MAX
bool parseInt(const string& text, long long& val) {
    if (text.empty()) { return false; }
    long long result = 0;
    for (char c : text) {
        if (c < '0' || c > '9') { return false; }
        int digit = c - '0';
        if (result > (LLONG_MAX - digit) / 10) { return false; }
        result = result * 10 + digit;
    }
    val = result;
    return true;
}

// "100.25" -> 10025: pad the decimals to 2 digits, then read whole + decimals as one int
bool parsePrice(const string& text, long long& ticks) {
    auto dot = text.find('.');
    string whole = text.substr(0, dot);
    string frac;
    if (dot != string::npos) {
        frac = text.substr(dot + 1);
        if (frac.empty() || frac.size() > 2) { return false; }
    }
    frac.resize(2, '0');
    return !whole.empty() && parseInt(whole + frac, ticks);
}

// core idea: split once, check the field count and each field for the msg type; fill a
// local msg and copy it out only when the whole line is good
// T: O(line len), S: O(line len)
bool parseLine(const string& line, Msg& msg) {
    vector<string> fields = splitFields(line);
    int n = fields.size();
    if (n < 2 || fields[0].size() != 1) { return false; }
    Msg result;
    result.type = fields[0][0];
    if (!parseInt(fields[1], result.id)) { return false; }

    if (result.type == 'A') {
        if (n != 5 || (fields[2] != "B" && fields[2] != "S")) { return false; }
        result.side = fields[2][0];
        if (!parsePrice(fields[3], result.price)) { return false; }
        if (!parseInt(fields[4], result.qty)) { return false; }
    } else if (result.type == 'R') {
        if (n != 3 || !parseInt(fields[2], result.qty)) { return false; }
    } else if (result.type == 'X') {
        if (n != 2) { return false; }
    } else {
        return false;
    }
    msg = result;
    return true;
}

void test(const string& label, const string& line, bool wantOk, const Msg& want) {
    Msg got;
    bool ok = parseLine(line, got);
    bool same = got.type == want.type && got.id == want.id && got.side == want.side &&
                got.price == want.price && got.qty == want.qty;
    cout << label << (ok == wantOk && same ? " PASS\n" : " FAIL\n");
}

int main() {
    test("add", "A,7,B,100.25,50", true, {'A', 7, 'B', 10025, 50});
    test("one-decimal", "A,8,S,100.5,10", true, {'A', 8, 'S', 10050, 10});
    test("cancel", "X,7", true, {'X', 7, ' ', 0, 0});
    test("reduce", "R,7,20", true, {'R', 7, ' ', 0, 20});
    test("missing-field", "A,7,B,100.25", false, {});
    test("empty-field", "A,,B,100.25,50", false, {});
    test("bad-side", "A,7,Z,100.25,50", false, {});
    test("three-decimals", "A,7,B,100.255,50", false, {});
    test("not-a-num", "R,7,2x", false, {});
}

// follow-up:
// 1. a whole file -> parse line by line; count rejects, log line num + the raw line
// 2. no copy per field -> walk the line with two idxs (or string_view) instead of split
// 3. signed nums -> read an optional '-' first, then the digits; clamp like atoi
// 4. tick of 0.0001 -> pad the decimals to 4 digits instead of 2
// 5. apply the msgs -> feed them to the order book (20-order-book)
// 6. binary feed -> fixed-width fields read with shifts (15-endian-fixed-width)
