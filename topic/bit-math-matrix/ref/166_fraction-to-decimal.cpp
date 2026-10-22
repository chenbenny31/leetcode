// LC 166 Fraction to Recurring Decimal: return numerator / denominator as a string,
// wrapping the repeating part of the fraction in parentheses.
#include <iostream>
#include <string>
#include <unordered_map>
#include <cstdlib>
using std::cout;
using std::string;
using std::unordered_map;
using std::to_string;
using std::llabs;

// Long division: the digit stream repeats exactly when a remainder shows up again.
// T: O(len of the output, at most the denominator), S: O(same).
string fractionToDecimal(int numerator, int denominator) {
    if (numerator == 0) { return "0"; }
    string res = (numerator < 0) != (denominator < 0) ? "-" : "";
    long long num = llabs(numerator);   // 64 bits so that INT_MIN is safe
    long long den = llabs(denominator);
    res += to_string(num / den);
    long long rem = num % den;
    if (rem == 0) { return res; }
    res += '.';
    unordered_map<long long, int> seen;   // remainder -> index in res of its digit
    while (rem != 0) {
        auto it = seen.find(rem);
        if (it != seen.end()) {
            res.insert(it->second, "(");
            res += ')';
            return res;
        }
        seen[rem] = res.size();
        rem *= 10;
        res += char('0' + rem / den);
        rem %= den;
    }
    return res;
}

void test(const string& label, int numerator, int denominator, const string& want) {
    cout << label << (fractionToDecimal(numerator, denominator) == want ? " PASS\n"
                                                                        : " FAIL\n");
}

int main() {
    test("terminating-negative", -50, 8, "-6.25");
    test("repeat-after-prefix", 1, 6, "0.1(6)");
    test("repeat-from-start", 4, 333, "0.(012)");
    test("int-min", -2147483647 - 1, -1, "2147483648");
}
