// Nth License Plate (Google, interview report)
// plates have 5 chars; order: 00000 .. 99999, 0000A .. 9999A, 0000B .. 9999Z,
// 000AA .. 999AA, 000AB .. 999ZZ, 00AAA .. , ZZZZZ; return plate number n (0-based)
// reported: NG onsite x2 ("license plate generation"); format taken from older reports
//
// clarify:
// exact order -> letters grow from the right, one more letter per block
// which part moves fastest -> digits, then the last letter
// n 0-based or 1-based -> 0-based
// n past the last plate -> ""

#include <iostream>
#include <string>

using std::cout;
using std::string;

// core idea: block k = plates w/ k letters, size 10^(5 - k) * 26^k; skip whole blocks
// inside a block: digits = idx % 10^(5 - k), letters = idx / 10^(5 - k) in base 26
// T: O(len^2), S: O(len)
string nthPlate(long long n) {
    const int LEN = 5;
    for (int letters = 0; letters <= LEN; letters++) {
        int digits = LEN - letters;
        // num of digit strings / letter strings in this block
        long long digitCnt = 1;
        for (int i = 0; i < digits; i++) { digitCnt *= 10; }
        long long letterCnt = 1;
        for (int i = 0; i < letters; i++) { letterCnt *= 26; }
        long long blockSize = digitCnt * letterCnt;
        if (n >= blockSize) {
            n -= blockSize;
            continue;
        }
        long long digitPart = n % digitCnt;
        long long letterPart = n / digitCnt;
        string plate(LEN, '0');
        for (int pos = digits - 1; pos >= 0; pos--) {
            plate[pos] = '0' + digitPart % 10;
            digitPart /= 10;
        }
        for (int pos = LEN - 1; pos >= digits; pos--) {
            plate[pos] = 'A' + letterPart % 26;
            letterPart /= 26;
        }
        return plate;
    }
    return "";
}

void test(const string& label, long long n, const string& want) {
    cout << label << (nthPlate(n) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("first", 0, "00000");
    test("last-all-digits", 99999, "99999");
    test("first-letter", 100000, "0000A");
    test("digits-move-first", 100001, "0001A");
    test("next-letter", 110000, "0000B");
    test("two-letters", 360000, "000AA");
    test("last-letter-moves", 361000, "000AB");
    test("last", 19244735, "ZZZZZ");
    test("past-the-end", 19244736, "");
}

// follow-up:
// plate -> its number -> add sizes of earlier blocks + letters value * 10^digits + digits
// next plate after a given one -> plate -> number, + 1, back; or carry in place
// print the first n plates -> loop over blocks; odometer on digits, then letters
// length L instead of 5 -> same code; total = sum of 10^(L - k) * 26^k, watch overflow
// skip letters I / O / Q -> base 23 w/ a lookup string instead of 'A' + x
