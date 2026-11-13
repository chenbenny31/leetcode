// LC 43 Multiply Strings (Medium) — big-number arithmetic on digit strings
// Multiply two non-negative integers given as decimal strings and return the product as
// a string, without converting the inputs to a built-in integer.
// Clarify: non-negative, digits only, no leading zeros (except "0" itself); lengths (up
//   to 200, so no 64-bit shortcut); a zero operand returns "0", not "000".

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// Core idea: schoolbook multiplication: a[i] * b[j] lands at position i + j + 1 of an
// array of m + n digits; add all the products first, then one carry pass from the right
// and strip the leading zeros. T: O(m*n), S: O(m + n).
string multiply(const string& a, const string& b) {
    int m = a.size();
    int n = b.size();
    // digits[pos] = sum of the products for that position, most significant first
    vector<int> digits(m + n, 0);
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            digits[i + j + 1] += (a[i] - '0') * (b[j] - '0');
        }
    }
    for (int pos = m + n - 1; pos > 0; pos--) {
        digits[pos - 1] += digits[pos] / 10;
        digits[pos] %= 10;
    }
    // keep the last digit so that a zero product is "0"
    int start = 0;
    while (start < m + n - 1 && digits[start] == 0) { start++; }
    string product;
    for (int pos = start; pos < m + n; pos++) {
        product.push_back((char)('0' + digits[pos]));
    }
    return product;
}

void test(const string& label, const string& a, const string& b, const string& want) {
    cout << label << (multiply(a, b) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("carries", "99", "99", "9801");
    test("different-lengths", "12", "345", "4140");
    test("zero-operand", "0", "345", "0");
    test("beyond-64-bit", "10000000000", "10000000000", "100000000000000000000");
    return 0;
}

// follow-up (ideas only):
// - add strings (LC 415) / add binary (LC 67) -> two pointers from the right with a
//   carry, loop while either pointer or the carry is left, reverse at the end
// - subtract strings (Google) -> compare magnitudes first (length, then lexicographic)
//   and swap so the larger is on top; borrow from the right, strip zeros, add the sign
// - plus one (LC 66) -> from the right: a 9 becomes 0 and the carry goes on, any other
//   digit is incremented and ends it; all 9s need a new leading 1
// - add two numbers as linked lists (LC 2) -> same carry loop on reversed lists with a
//   dummy head; LC 445 (forward order) reverses first or uses two stacks
// - negative inputs -> strip the signs, multiply the magnitudes, put '-' back when
//   exactly one input was negative and the product is not zero
// - factorial of a large n -> keep the digits in a vector (least significant first) and
//   multiply it by each int 2..n with a carry
