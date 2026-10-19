// score 0..100 for measuring a string matches a palindrome
// normalize, score = 100 * matched / pairs

#include <iostream>
#include <string>
#include <cmath>

using std::cout;
using std::string;
using std::fabs; 

// default to lower case
char lower(char c) {
    return c >= 'A' && c <= 'Z'
         ? c - 'A' + 'a'
         : c;
}

// two pointers over pairs, middle char of odd length doesn't pair
// T: O(n), S: O(1)
double palindromeScore(const string& s) {
    const int n = static_cast<int>(s.length());
    int pairs = n / 2;
    int match = 0;

    int l = 0;
    int r = n - 1;
    while (l < r) {
        if (lower(s[l]) == lower(s[r])) {
            match++;
        }
        l++;
        r--;
    }

    return pairs == 0 ? 100.0 : 100.0 * match / pairs;
}

void test(const string& label, const string& s, double want) {
    cout << label << (palindromeScore(s) == want ? " pass\n" : " fail\n");
}

int main() {
    test("half odd", "abcca", 50.0);
    test("upper palin odd", "Racecar", 100.0);
    test("zero even", "abcd", 0.0);
    test("empty", "", 100.0);
}

