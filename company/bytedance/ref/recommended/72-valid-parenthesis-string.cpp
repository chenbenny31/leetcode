// ByteDance (hack2hire) / LC 678: s has '(', ')' and '*'; '*' may be '(', ')' or empty.
// Is s valid?

#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::set;
using std::max;

// Greedy: track the range [lo, hi] of possible open counts. T: O(n), S: O(1).
bool checkValidString(const string& s) {
    // fewest opens possible (every '*' used as ')' when allowed)
    int lo = 0;
    // most opens possible (every '*' used as '(')
    int hi = 0;
    for (char c : s) {
        if (c == '(') {
            lo++;
            hi++;
        } else if (c == ')') {
            lo--;
            hi--;
        } else {
            lo--;
            hi++;
        }
        // too many ')' even with every '*' as '('
        if (hi < 0) {
            return false;
        }
        // open count can never go negative
        lo = max(lo, 0);
    }
    return lo == 0;
}

// cur = s[0..i) after replacements, open = unmatched '(' in cur
void dfs(const string& s, int i, int open, string& cur, set<string>& found) {
    // a ')' with nothing to match: no fix later
    if (open < 0) {
        return;
    }
    if (i == (int)s.size()) {
        if (open == 0) {
            found.insert(cur);
        }
        return;
    }
    if (s[i] != '*') {
        cur.push_back(s[i]);
        dfs(s, i + 1, open + (s[i] == '(' ? 1 : -1), cur, found);
        cur.pop_back();
        return;
    }
    cur.push_back('(');
    dfs(s, i + 1, open + 1, cur, found);
    cur.back() = ')';
    dfs(s, i + 1, open - 1, cur, found);
    cur.pop_back();
    // '*' as empty
    dfs(s, i + 1, open, cur, found);
}

// follow-up: DFS over each '*' choice. T: O(3^k * n), k = number of '*'. S: O(3^k * n).
vector<string> allValidStrings(const string& s) {
    // distinct valid strings, kept sorted
    set<string> found;
    string cur;
    dfs(s, 0, 0, cur, found);
    return vector<string>(found.begin(), found.end());
}

void test(const string& label, const string& s, bool want) {
    cout << label << (checkValidString(s) == want ? " PASS\n" : " FAIL\n");
}

void testAll(const string& label, const string& s, const vector<string>& want) {
    cout << label << (allValidStrings(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("star-as-empty", "(*)", true);
    test("star-as-open", "(*))", true);
    test("unclosed", "((*", false);
    test("close-first", ")(", false);
    testAll("list-all", "(**)", {"(())", "()", "()()"});
}

// follow-up: list every distinct valid string, sorted -> see allValidStrings() above;
//   3 choices per '*', prune open < 0, set<string> dedups and sorts
// follow-up: min adds to fix a plain "()" string (LC 921) -> open counter; ')' with
//   open == 0 -> need++; answer open + need
// follow-up: remove invalid parentheses (LC 301) -> BFS one removal per level, stop at
//   the first level with a valid string; set dedups
