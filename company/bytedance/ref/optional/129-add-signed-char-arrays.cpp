// ByteDance phone (FT): left and right are signed decimal integers as char arrays
// (optional '+'/'-', >= 1 digit, leading zeros allowed), up to 2e5 chars. Return the
// exact sum: no leading zeros, '-' only if negative, zero is {'0'}. No int conversion.
// (recommended/44-add-strings-char-arrays.cpp is the unsigned version.)
//   addSignedNumbers(left, right) -> vector<char>

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::reverse;

struct Number {
    // sign; "-0" is stored as non-negative
    bool negative = false;
    // digits with the least significant first, no leading zeros ("0" -> {0})
    vector<int> digits;
};

Number parse(const vector<char>& chars) {
    Number num;
    int i = 0;
    if (chars[0] == '+' || chars[0] == '-') {
        num.negative = chars[0] == '-';
        i = 1;
    }
    // skip leading zeros but keep at least one digit
    while (i + 1 < (int)chars.size() && chars[i] == '0') {
        i++;
    }
    for (int j = (int)chars.size() - 1; j >= i; j--) {
        num.digits.push_back(chars[j] - '0');
    }
    if (num.digits.size() == 1 && num.digits[0] == 0) {
        num.negative = false;
    }
    return num;
}

// -1 / 0 / 1 for |a| < / = / > |b| (digits stored least significant first).
int compareMagnitude(const vector<int>& a, const vector<int>& b) {
    if (a.size() != b.size()) {
        return a.size() < b.size() ? -1 : 1;
    }
    for (int i = (int)a.size() - 1; i >= 0; i--) {
        if (a[i] != b[i]) {
            return a[i] < b[i] ? -1 : 1;
        }
    }
    return 0;
}

vector<int> addMagnitude(const vector<int>& a, const vector<int>& b) {
    vector<int> sum;
    int carry = 0;
    for (int i = 0; i < (int)a.size() || i < (int)b.size() || carry > 0; i++) {
        int d = carry;
        if (i < (int)a.size()) {
            d += a[i];
        }
        if (i < (int)b.size()) {
            d += b[i];
        }
        sum.push_back(d % 10);
        carry = d / 10;
    }
    return sum;
}

// |a| - |b| for |a| >= |b|; strips the leading zeros it creates.
vector<int> subtractMagnitude(const vector<int>& a, const vector<int>& b) {
    vector<int> diff;
    int borrow = 0;
    for (int i = 0; i < (int)a.size(); i++) {
        int d = a[i] - borrow;
        if (i < (int)b.size()) {
            d -= b[i];
        }
        if (d < 0) {
            d += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        diff.push_back(d);
    }
    while (diff.size() > 1 && diff.back() == 0) {
        diff.pop_back();
    }
    return diff;
}

// Same signs: add magnitudes; else larger minus smaller with the larger's sign.
// T: O(m + n), S: O(m + n).
vector<char> addSignedNumbers(const vector<char>& left, const vector<char>& right) {
    Number a = parse(left);
    Number b = parse(right);
    vector<int> digits;
    bool negative = false;

    if (a.negative == b.negative) {
        digits = addMagnitude(a.digits, b.digits);
        negative = a.negative;
    } else {
        int cmp = compareMagnitude(a.digits, b.digits);
        // equal magnitudes: plain 0, never "-0"
        if (cmp == 0) {
            return {'0'};
        }
        if (cmp > 0) {
            digits = subtractMagnitude(a.digits, b.digits);
            negative = a.negative;
        } else {
            digits = subtractMagnitude(b.digits, a.digits);
            negative = b.negative;
        }
    }

    vector<char> out;
    if (negative) {
        out.push_back('-');
    }
    for (int i = (int)digits.size() - 1; i >= 0; i--) {
        out.push_back('0' + digits[i]);
    }
    return out;
}

void test(const string& label, const string& left, const string& right,
          const string& want) {
    vector<char> a(left.begin(), left.end());
    vector<char> b(right.begin(), right.end());
    bool ok = addSignedNumbers(a, b) == vector<char>(want.begin(), want.end());
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("carry", "+95", "7", "102");
    test("negatives", "-8", "-5", "-13");
    test("flip-sign", "3", "-10", "-7");
    test("zero", "-007", "+7", "0");
    test("negative-zero", "-0", "0", "0");
}

// follow-up: subtraction a - b -> flip b.negative, reuse the same add path
// follow-up: multiply -> negative = a.negative != b.negative (zero stays '+');
//   prod[i + j] += a[i] * b[j] on LSB-first digits, carry pass, strip zeros, O(m * n)
// follow-up: invalid input ("", "-", "1a") -> validate first: optional sign, >= 1 digit,
//   all '0'..'9', else reject (parse() assumes valid input)
