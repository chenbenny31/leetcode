// sep by ',' only, num in digits only, no sign
// price -> at most two decimals
// malformed line -> return false
// no double -> need int stays exact

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

// split on ',', keep empty fields
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

// digits only at least one, false if unable parse
bool parseInt(const string& text, long long& val) {
    if (text.empty()) { return false; }
    long long result = 0;
    for (char c : text) {
        if (c < '0' || c > '9') { return false; }
        int d = c - '0';
        if (result * 10 + d > LLONG_MAX) { return false; }
        result = result * 10 + d;
    }
    val = result;
    return true;
}

// "100.25" -> 10025, pad decimals to 2 digits
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

// split once, check field for msg, fill and copy when valid line
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

