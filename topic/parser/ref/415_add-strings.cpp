// LC 415 Add Strings: add two non-negative integers given as decimal strings and
// return the sum as a string, without converting them to integers.
#include <iostream>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::reverse;

// Grade-school addition from the back with a carry. T: O(m+n), S: O(max(m,n)).
string addStrings(string num1, string num2) {
    string sum;   // result digits, least significant first
    int i = num1.size() - 1, j = num2.size() - 1, carry = 0;
    // carry > 0 keeps going after both run out, e.g. 99 + 1
    while (i >= 0 || j >= 0 || carry > 0) {
        int d = carry;
        if (i >= 0) {
            d += num1[i] - '0';
            i--;
        }
        if (j >= 0) {
            d += num2[j] - '0';
            j--;
        }
        sum += (char)('0' + d % 10);
        carry = d / 10;
    }
    reverse(sum.begin(), sum.end());
    return sum;
}

void test(const string& label, const string& a, const string& b, const string& want) {
    cout << label << (addStrings(a, b) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("different-lengths", "456", "77", "533");
    test("final-carry", "99", "1", "100");
    test("zeros", "0", "0", "0");
}
