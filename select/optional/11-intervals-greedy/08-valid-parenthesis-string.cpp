// LC 678 Valid Parenthesis String (Medium). Pattern: greedy range of open counts.
// s has '(', ')' and '*'; each '*' may be '(', ')' or empty. Can s be a valid
// parenthesis string?

#include <iostream>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::max;

// Track the range [minOpen, maxOpen] of possible open counts; valid iff it holds 0 at
// the end.
// T: O(n), S: O(1).
bool checkValidString(const string& s) {
    // fewest opens possible (each '*' as ')' while that keeps opens >= 0)
    int minOpen = 0;
    // most opens possible (each '*' as '(')
    int maxOpen = 0;
    for (char c : s) {
        if (c == '(') {
            minOpen++;
            maxOpen++;
        } else if (c == ')') {
            minOpen--;
            maxOpen--;
        } else {
            minOpen--;
            maxOpen++;
        }
        // too many ')' even with every '*' as '('
        if (maxOpen < 0) { return false; }
        // a negative open count is never a real choice; clamp
        minOpen = max(minOpen, 0);
    }
    return minOpen == 0;
}

void test(const string& label, const string& s, bool want) {
    cout << label << (checkValidString(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("star-close", "((*)", true);
    test("star-empty", "(*)", true);
    test("close-first", "*)(", false);
    test("unclosed", "((*", false);
}

// follow-up: why clamp minOpen at 0 -> minOpen < 0 means a '*' was used as ')' with
//   nothing open; treat that '*' as empty instead
// follow-up: index stacks -> stacks of '(' and '*' indices; ')' pops '(' first, else
//   '*'; at the end each leftover '(' needs a '*' at a later index. O(n) space
