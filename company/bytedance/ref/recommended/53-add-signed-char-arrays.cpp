// ByteDance phone (FT): two signed decimal integers are stored as char arrays left and
// right (optional leading '+' or '-', then at least one digit, leading zeros allowed).
// Return their exact sum as a normalized char array:
// - no fixed-width integer conversion and no big-integer library;
// - no leading zeros in the result;
// - '-' only when the result is negative;
// - zero is {'0'}.
//   addSignedNumbers(left, right) -> vector<char>
// Constraints: length <= 2e5. (important/62 is the unsigned version.)

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::reverse;

struct Number {
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

// Grade-school arithmetic on digit arrays. Same signs: add magnitudes, keep the sign.
// Different signs: subtract the smaller magnitude from the larger one and take the
// larger one's sign; equal magnitudes give 0, which never gets a '-'.
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

vector<char> chars(const string& s) {
    return vector<char>(s.begin(), s.end());
}

void test(const string& label, const string& left, const string& right,
          const string& want) {
    bool ok = addSignedNumbers(chars(left), chars(right)) == chars(want);
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("carry", "+95", "7", "102");
    test("negatives", "-8", "-5", "-13");
    test("borrow", "1000", "-1", "999");
    test("flip-sign", "3", "-10", "-7");
    test("zero", "-007", "+7", "0");
    test("negative-zero", "-0", "0", "0");
}
