// ByteDance intern OA (1p3a 1189035, Q3; same as LC 2243): s is a digit string. While
// its length is greater than k, split it into consecutive groups of k digits (the last
// group may be shorter), replace each group by the decimal string of its digit sum,
// and join the results. Return the final string.
// Edge case: with k = 1 a string longer than 1 never shrinks (each digit maps to
// itself), so the loop would not end; LC 2243 has k >= 2, and this version returns s
// unchanged for k < 2.

#include <iostream>
#include <string>

using std::cout;
using std::string;
using std::to_string;

// Simulate the rounds; each round shrinks the string by about a factor of k / 2.
// T: O(n) per round, O(n log n) overall, S: O(n).
string digitSum(string s, int k) {
    if (k < 2) {
        return s;
    }
    while ((int)s.size() > k) {
        string next;
        for (int start = 0; start < (int)s.size(); start += k) {
            int sum = 0;
            for (int i = start; i < start + k && i < (int)s.size(); i++) {
                sum += s[i] - '0';
            }
            next += to_string(sum);
        }
        s = next;
    }
    return s;
}

void test(const string& label, const string& s, int k, const string& want) {
    cout << label << (digitSum(s, k) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("one-round", "1234", 2, "37");
    test("two-rounds", "999999", 3, "117");
    test("already-short", "5", 4, "5");
    test("zeros", "0000", 2, "00");
    test("k-one", "123", 1, "123");
}
