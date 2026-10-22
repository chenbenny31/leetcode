// LC 17 Letter Combinations of a Phone Number: digits 2-9 map to phone-key letters
// (2 = abc ... 9 = wxyz). Return every letter combination, or [] for empty digits.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// letters[d] = the letters on phone key d
const vector<string> letters = {"", "", "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv",
                                "wxyz"};

void dfs(const string& digits, int i, string& cur, vector<string>& out) {
    int n = digits.size();
    if (i == n) {
        out.push_back(cur);
        return;
    }
    for (char c : letters[digits[i] - '0']) {
        cur.push_back(c);
        dfs(digits, i + 1, cur, out);
        cur.pop_back();
    }
}

// Backtracking: one level per digit, try each of its letters. cur is the partial word.
// T: O(4^n * n), S: O(n) beyond the output.
vector<string> letterCombinations(string digits) {
    vector<string> out;
    if (digits.empty()) { return out; }
    string cur;
    dfs(digits, 0, cur, out);
    return out;
}

void test(const string& label, const string& digits, vector<string> want) {
    cout << label << (letterCombinations(digits) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("two-digits", "23", {"ad", "ae", "af", "bd", "be", "bf", "cd", "ce", "cf"});
    test("four-letter-key", "7", {"p", "q", "r", "s"});
    test("empty", "", {});
}
