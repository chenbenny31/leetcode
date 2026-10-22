// LC 43 Multiply Strings: multiply two non-negative integers given as decimal strings
// and return the product as a string, without converting the whole numbers to ints.
#include <iostream>
#include <string>
#include <vector>
using std::cout;
using std::string;
using std::vector;

// Schoolbook multiplication: digit i of num1 times digit j of num2 lands in slot i+j+1
// with the carry going to slot i+j. T: O(m*n), S: O(m+n).
string multiply(string num1, string num2) {
    int m = num1.size();
    int n = num2.size();
    vector<int> digits(m + n, 0);   // digits[k] = decimal digit of the product at slot k
    for (int i = m - 1; i >= 0; i--) {
        for (int j = n - 1; j >= 0; j--) {
            int sum = (num1[i] - '0') * (num2[j] - '0') + digits[i + j + 1];
            digits[i + j + 1] = sum % 10;
            digits[i + j] += sum / 10;
        }
    }
    string res;
    for (int d : digits) {
        if (res.empty() && d == 0) { continue; }
        res += char('0' + d);
    }
    return res.empty() ? "0" : res;
}

void test(const string& label, const string& a, const string& b, const string& want) {
    cout << label << (multiply(a, b) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("small", "12", "13", "156");
    test("carry-chain", "99", "99", "9801");
    test("zero", "0", "999", "0");
    test("big", "123456789", "987654321", "121932631112635269");
}
