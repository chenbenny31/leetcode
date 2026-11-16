// LC 43 Multiply Strings (Medium) — big-number arithmetic on digit strings
// multiply two non-negative ints given as decimal strings; return product as a string
// no converting the inputs to a built-in integer
//
// clarify:
// non-negative, digits only, no leading zeros (except "0") -> no sign handling
// len up to 200 -> no 64-bit shortcut
// zero operand -> return "0", not "000"
// negative inputs? -> follow-up

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// core idea: schoolbook: a[i] * b[j] lands at pos i + j + 1 of m + n digits
// add all products first, then one carry pass from the right, strip leading zeros
// T: O(m * n), S: O(m + n)
string multiply(const string& a, const string& b) {
    int m = a.size();
    int n = b.size();
    // digits[pos] = sum of products for that pos, most significant first
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
    // keep the last digit so a zero product is "0"
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

// follow-up:
// add strings (LC 415) / add binary (LC 67) -> two ptrs from the right w/ carry, reverse
// subtract strings (Google) -> swap so larger is on top; borrow from right, strip zeros
// plus one (LC 66) -> from right: 9 -> 0 and carry on; other digit++ and stop
// add two nums as linked lists (LC 2) -> same carry loop w/ dummy head
// LC 445 (forward order) -> reverse first or use two stacks
// negative inputs -> strip signs, multiply magnitudes, add '-' if exactly one negative
// factorial of large n -> digits in a vector (least significant first), multiply by 2..n
