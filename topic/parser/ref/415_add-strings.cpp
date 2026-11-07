// LC 415 Add Strings: add two decimal strings, without converting them to integers.
#include <iostream>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::reverse;

// Grade-school addition from the back with a carry. T: O(m+n), S: O(max(m,n)).
string addStrings(string num1, string num2) {
    int m = num1.size(), n = num2.size();
    string sum;   // result digits, least significant first
    int i = m - 1, j = n - 1, carry = 0;
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
