// ByteDance (hack2hire): repeatedly remove every contiguous triple "xXx" (a lowercase
// letter, any uppercase letter, the same lowercase letter, e.g. "aAa", "vQv") until
// none is left; return the final string.

#include <iostream>
#include <string>
#include <cctype>

using std::cout;
using std::string;
using std::islower;
using std::isupper;

// String as a stack; a removal can only complete at the top. T: O(n), S: O(n).
string filterCamel(const string& s) {
    // chars kept so far, no removable triple inside
    string stk;
    for (char c : s) {
        stk.push_back(c);
        int m = stk.size();
        if (m >= 3 && islower(stk[m - 1]) && isupper(stk[m - 2])
            && stk[m - 3] == stk[m - 1]) {
            // popping may expose a new triple ending at the next pushed char
            stk.resize(m - 3);
        }
    }
    return stk;
}

void test(const string& label, const string& s, const string& want) {
    cout << label << (filterCamel(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("XxX-kept", "AaADbDEeEbcvQv", "AaADbDEeEbc");
    test("one-triple", "abBbc", "ac");
    test("cascade", "abBbAa", "");
}

// follow-up: O(1) extra -> use s itself as the stack: write index w, s[w] = c, w++,
//   check s[w-3..w-1], w -= 3; s.resize(w)
// follow-up: remove every occurrence of part (LC 1910) -> push, compare tail to part,
//   O(n * L); store KMP state per stack slot for O(n + L)
// follow-up: k adjacent equal chars (LC 1209) -> stack of (char, count), pop at count k
