// feed parser: turn one text line of an order feed into a msg, without using double
//   A,<id>,<side B|S>,<price>,<qty>   add      e.g. A,7,B,100.25,50
//   X,<id>                            cancel   e.g. X,7
//   R,<id>,<qty>                      reduce   e.g. R,7,20
// price has at most 2 decimals and becomes an int num of ticks of 0.01

// usage:
// g++ -std=c++17 -Wall -Wextra 16-feed-parser.cpp -o a && ./a
//
// Msg msg;  if (parseLine(line, msg)) { apply msg to the order book (04-order-book) }

// clarify / assumptions
// fields are separated by ',' with no spaces; numbers are digits only, no sign
// any malformed line -> parseLine returns false and the line is skipped
// numbers fit in int64, overflow is not checked

// why no double:
// 100.25 is not exact in binary; ticks as an int are exact and safe as a map key
// "100.25" -> 100 * 100 + 25 = 10025, "100.5" -> 10050, "100" -> 10000

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <iostream>

using std::int64_t;
using std::size_t;
using std::cout;
using std::string;
using std::vector;

struct Msg {
    char type; // 'A' add, 'X' cancel, 'R' reduce
    int64_t id;
    char side; // 'B' or 'S', only for add
    int64_t price; // in ticks of 0.01, only for add
    int64_t qty; // only for add and reduce
};

// split on ',' with a plain char loop; "a,,b" gives an empty field in the middle
vector<string> splitFields(const string& line) {
    vector<string> fields;
    string field;
    for (char c : line) {
        if (c == ',') {
            fields.push_back(field);
            field = "";
        } else {
            field += c;
        }
    }
    fields.push_back(field);
    return fields;
}

// digits only -> val; false if empty or any other char
bool parseInt(const string& text, int64_t& val) {
    if (text.empty()) { return false; }

    int64_t result = 0;
    for (char c : text) {
        if (c < '0' || c > '9') { return false; }
        result = result * 10 + (c - '0');
    }
    val = result;
    return true;
}

// "100.25" -> 10025; whole part needed, 0 to 2 decimals
bool parsePrice(const string& text, int64_t& ticks) {
    string whole = text;
    string frac = "";
    size_t dot = text.find('.');
    if (dot != string::npos) {
        whole = text.substr(0, dot);
        frac = text.substr(dot + 1);
        if (frac.empty() || frac.size() > 2) { return false; } // "100." or "100.123"
    }
    while (frac.size() < 2) {
        frac += '0'; // "5" means 50 ticks, not 5
    }

    int64_t wholeVal = 0;
    int64_t fracVal = 0;
    if (!parseInt(whole, wholeVal) || !parseInt(frac, fracVal)) { return false; }
    ticks = wholeVal * 100 + fracVal;
    return true;
}

// T: O(len of line), S: O(len of line)
bool parseLine(const string& line, Msg& msg) {
    vector<string> fields = splitFields(line);
    if (fields[0].size() != 1) { return false; }

    Msg result = {fields[0][0], 0, ' ', 0, 0};
    if (result.type == 'A') {
        if (fields.size() != 5) { return false; }
        if (fields[2] != "B" && fields[2] != "S") { return false; }
        result.side = fields[2][0];
        if (!parseInt(fields[1], result.id)) { return false; }
        if (!parsePrice(fields[3], result.price)) { return false; }
        if (!parseInt(fields[4], result.qty)) { return false; }

    } else if (result.type == 'X') {
        if (fields.size() != 2) { return false; }
        if (!parseInt(fields[1], result.id)) { return false; }

    } else if (result.type == 'R') {
        if (fields.size() != 3) { return false; }
        if (!parseInt(fields[1], result.id)) { return false; }
        if (!parseInt(fields[2], result.qty)) { return false; }

    } else {
        return false;
    }

    msg = result;
    return true;
}

void testLine(const string& label, const string& line, const Msg& want) {
    Msg got = {' ', 0, ' ', 0, 0};
    bool parsed = parseLine(line, got);
    bool sameHead = (got.type == want.type && got.id == want.id && got.side == want.side);
    bool ok = (parsed && sameHead && got.price == want.price && got.qty == want.qty);
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void testBad(const string& label, const string& line) {
    Msg msg = {' ', 0, ' ', 0, 0};
    cout << label << (!parseLine(line, msg) ? " PASS\n" : " FAIL\n");
}

int main() {
    // test valid cases
    testLine("add", "A,7,B,100.25,50", {'A', 7, 'B', 10025, 50});
    testLine("add-1-decimal", "A,8,S,100.5,10", {'A', 8, 'S', 10050, 10});
    testLine("add-no-decimal", "A,9,S,100,10", {'A', 9, 'S', 10000, 10});
    testLine("cancel", "X,7", {'X', 7, ' ', 0, 0});
    testLine("reduce", "R,7,20", {'R', 7, ' ', 0, 20});

    // test invalid cases
    testBad("empty-line", "");
    testBad("unknown-type", "Q,7");
    testBad("missing-field", "A,7,B,100.25");
    testBad("empty-field", "A,,B,100.25,50");
    testBad("bad-side", "A,7,Z,100.25,50");
    testBad("3-decimals", "A,7,B,100.255,50");
    testBad("dot-no-digits", "A,7,B,100.,50");
    testBad("not-a-number", "R,7,2x");
}
