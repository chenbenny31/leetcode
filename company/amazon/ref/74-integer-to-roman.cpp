// LC 12: convert 1 <= num <= 3999 to a Roman numeral.

#include <iostream>
#include <vector>
#include <string>
#include <utility>

using std::cout;
using std::string;
using std::vector;
using std::pair;

// Greedy over the 13 symbols including the subtractive pairs (CM, CD, XC, XL, IX, IV):
// always take the largest that fits. T: O(1), S: O(1).
string intToRoman(int num) {
    static const vector<pair<int, string>> symbols = {
        {1000, "M"}, {900, "CM"}, {500, "D"}, {400, "CD"}, {100, "C"},
        {90, "XC"},  {50, "L"},   {40, "XL"}, {10, "X"},   {9, "IX"},
        {5, "V"},    {4, "IV"},   {1, "I"}};
    string result;
    for (const auto& [value, sym] : symbols) {
        while (num >= value) {
            result += sym;
            num -= value;
        }
    }
    return result;
}

void test(const string& label, int num, const string& want) {
    cout << label << (intToRoman(num) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("one", 1, "I");
    test("subtract", 14, "XIV");
    test("forty", 40, "XL");
    test("mixed", 444, "CDXLIV");
    test("max", 3999, "MMMCMXCIX");
}
