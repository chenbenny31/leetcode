// 10 · Expand Nested Repetitions (Cisco OA, Medium) — stack saved at '('
// Mother: select/05-stack/05-decode-string.cpp (LC 394, where the count comes first)
// "(ab){3}(cd){2}" -> "abababcdcd"; groups nest: "(a(b){2}){2}" -> "abbabb"
//
// clarify:
// input well formed (balanced parens, digits inside {}) -> assume yes
// ')' w/o a following {k} -> the group appears once
// k == 0 -> the group disappears
// letters outside any group -> copied as they are

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// core idea: '(' saves the text so far and starts a new one; at ')' read the {k} that
// follows, repeat the finished group k times and append it to the saved text
// T: O(n + output len * nesting depth), S: O(output len)
string expand(const string& s) {
    int n = s.size();
    // text built before each still-open '('
    vector<string> stk;
    // text of the innermost open group (or the top level)
    string cur;
    int i = 0;
    while (i < n) {
        char c = s[i];
        if (c == '(') {
            stk.push_back(cur);
            cur.clear();
            i++;
        } else if (c == ')') {
            i++;
            int k = 1;
            if (i < n && s[i] == '{') {
                i++;
                k = 0;
                while (i < n && s[i] != '}') {
                    k = k * 10 + (s[i] - '0');
                    i++;
                }
                // skip '}'
                i++;
            }
            string group = cur;
            cur = stk.back();
            stk.pop_back();
            for (int r = 0; r < k; r++) { cur += group; }
        } else {
            cur += c;
            i++;
        }
    }
    return cur;
}

void test(const string& label, const string& s, const string& want) {
    cout << label << (expand(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("flat", "(ab){3}(cd){2}", "abababcdcd");
    test("nested", "(a(b){2}){2}", "abbabb");
    test("no-count", "x(ab)y", "xaby");
    test("zero", "(ab){0}c", "c");
    return 0;
}

// follow-up:
// count first, "3[ab]" (LC 394) -> push the count too: select/05-stack/05-decode-string
// recursive version -> expandFrom(s, i) returns at ')' ; caller reads {k} and repeats
// only the output len -> same stack w/ lens (long long) instead of strings
// k-th char w/o building (LC 880) -> compute lens, walk back w/ k %= group len
// malformed input -> error when ')' finds an empty stack or the end leaves one open
