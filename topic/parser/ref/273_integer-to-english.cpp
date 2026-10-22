// LC 273 Integer to English Words: convert a non-negative int to its English words,
// e.g. 1000010 -> "One Million Ten". 0 -> "Zero".
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

const vector<string> ones = {"", "One", "Two", "Three", "Four", "Five", "Six", "Seven",
                             "Eight", "Nine", "Ten", "Eleven", "Twelve", "Thirteen",
                             "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen",
                             "Nineteen"};
const vector<string> tens = {"", "", "Twenty", "Thirty", "Forty", "Fifty", "Sixty",
                             "Seventy", "Eighty", "Ninety"};
const vector<string> scales = {"", "Thousand", "Million", "Billion"};

// Words for 1..999, each word followed by a space. T: O(1), S: O(1).
string below1000(int n) {
    string res;
    if (n >= 100) {
        res += ones[n / 100] + " Hundred ";
        n %= 100;
    }
    if (n >= 20) {
        res += tens[n / 10] + " ";
        n %= 10;
    }
    if (n > 0) {
        res += ones[n] + " ";
    }
    return res;
}

// Peel off groups of three digits from the right and attach each scale word.
// T: O(log10 num), S: O(1) beyond the output.
string numberToWords(int num) {
    if (num == 0) { return "Zero"; }
    string res;
    for (int u = 0; num > 0; u++) {
        int group = num % 1000;
        num /= 1000;
        // an all-zero group (e.g. the middle of 1,000,010) contributes nothing
        if (group == 0) { continue; }
        string part = below1000(group);
        if (u > 0) { part += scales[u] + " "; }
        res = part + res;
    }
    res.pop_back();
    return res;
}

void test(const string& label, int num, const string& want) {
    cout << label << (numberToWords(num) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("zero", 0, "Zero");
    test("teen", 115, "One Hundred Fifteen");
    test("skip-zero-group", 1000010, "One Million Ten");
    test("billion", 2000000000, "Two Billion");
}
