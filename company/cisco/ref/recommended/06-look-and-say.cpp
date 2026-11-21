// 06 · Look-and-Say (Cisco OA, Medium) — LC 38, run-length scan
// (1) next term: say each run of equal digits as count + digit ("225" -> "2215")
// (2) LC 38: n-th term of the sequence that starts at "1"
//
// clarify:
// input chars -> digits only
// a run of 10+ equal digits -> count written in full ("10" + digit)
// n is 1-based -> n == 1 returns "1"
// empty string -> empty string

#include <iostream>
#include <string>

using std::cout;
using std::string;
using std::to_string;

// (1) next term
// core idea: two idx; j runs to the end of the run that starts at i, emit (j - i) + digit
// T: O(len), S: O(len) — output
string nextTerm(const string& s) {
    int n = s.size();
    string out;
    int i = 0;
    while (i < n) {
        // j = first idx after the run starting at i
        int j = i;
        while (j < n && s[j] == s[i]) { j++; }
        out += to_string(j - i);
        out += s[i];
        i = j;
    }
    return out;
}

// (2) n-th term
// core idea: apply nextTerm n - 1 times
// T: O(sum of term lens), S: O(len of the last term) — a term is ~1.3x the previous one
string countAndSay(int n) {
    string term = "1";
    for (int i = 1; i < n; i++) { term = nextTerm(term); }
    return term;
}

void test(const string& label, const string& got, const string& want) {
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("next", nextTerm("225"), "2215");
    test("next-empty", nextTerm(""), "");
    test("first", countAndSay(1), "1");
    test("fourth", countAndSay(4), "1211");
    return 0;
}

// follow-up:
// compress in place, count only when > 1 (LC 443) -> read / write pointers on one array
// decode "2215" back -> pairs (count, digit); ambiguous once a count has 2+ digits
// only the len of the n-th term -> still simulate; no closed form (grows ~1.3^n)
// recursive version -> countAndSay(n) = nextTerm(countAndSay(n - 1)), depth n
