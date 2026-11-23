// 17 · Look-and-Say (Cisco OA, Medium) — LC 38, run-length scan
// ==================== (1) ====================
// next look-and-say term: "225" -> "2215"

// LC 38: n-th term of the sequence that starts at "1"
//
// clarify:
// input chars -> digits only
// a run of 10+ equal digits -> count written in full ("10" + digit)
// n is 1-based -> n == 1 returns "1"
// empty str -> empty str

#include <iostream>
#include <string>

using std::cout;
using std::string;
using std::to_string;

// next term
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

// ==================== (2) ====================
// n-th term, starting from "1"

// core idea: apply nextTerm n - 1 times
// T: O(sum of term lens), S: O(len of the last term) — a term is ~1.3x the prev one
string countAndSay(int n) {
    string term = "1";
    for (int i = 1; i < n; i++) { term = nextTerm(term); }
    return term;
}

// ==================== tests ====================

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
// 1. compress in place (LC 443) -> read / write pointers on one array
// 2. decode "2215" back -> pairs (count, digit); ambiguous once a count has 2+ digits
// 3. only the len of the n-th term -> still simulate; no closed form (grows ~1.3^n)
// 4. recursive -> dfs(v, parent) adds child time + 2 for children with apples below
