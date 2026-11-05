// ByteDance intern OA (1p3a 1189035, Q3; LC 2243): s is a digit string, k >= 2. While
// s is longer than k, split it into groups of k digits (the last may be shorter) and
// replace each group by the decimal string of its digit sum. Return the final s.

#include <iostream>
#include <string>

using std::cout;
using std::string;
using std::to_string;

// Simulate round by round. T: O(n) per round, O(n log n) overall, S: O(n).
string digitSum(string s, int k) {
    while ((int)s.size() > k) {
        // s after this round
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
}

// follow-up: k = 1 -> each digit maps to itself, so the loop never ends; guard it
//   if (k < 2) { return s; }
// follow-up: in place, O(1) extra -> write each group's sum back at index w <= start;
//   digits(sum) <= group length, so w never passes the read index; s.resize(w)
// follow-up: return every round -> push s into vector<string> before each round
