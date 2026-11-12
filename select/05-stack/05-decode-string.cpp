// LC 394 Decode String (Medium). Pattern: two stacks saved at each '['.
// Decode k[encoded] = encoded repeated k times; brackets may nest. Input is valid and
// digits appear only as repeat counts.

#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::string;
using std::vector;

// At '[' save (count, text so far) and start fresh; at ']' repeat and append to the saved
// text. T: O(output length), S: O(output length).
string decodeString(const string& s) {
    // repeat count of each open bracket
    vector<int> counts;
    // text built before each open bracket
    vector<string> prevs;
    // text inside the innermost open bracket so far
    string cur;
    int num = 0;
    for (char c : s) {
        if (c >= '0' && c <= '9') {
            num = num * 10 + (c - '0');
        } else if (c == '[') {
            counts.push_back(num);
            prevs.push_back(cur);
            num = 0;
            cur = "";
        } else if (c == ']') {
            string outer = prevs.back();
            prevs.pop_back();
            for (int i = 0; i < counts.back(); i++) { outer += cur; }
            counts.pop_back();
            cur = outer;
        } else {
            cur += c;
        }
    }
    return cur;
}

void test(const string& label, const string& s, const string& want) {
    cout << label << (decodeString(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("flat", "2[ab]c", "ababc");
    test("nested", "2[a2[b]]", "abbabb");
    test("multi-digit", "10[a]", "aaaaaaaaaa");
    test("text-around", "x2[y]z", "xyyz");
}

// follow-up: recursive version -> decode(s, i) reads until ']' or the end; on k[ recurse
//   and append the result k times; index i shared by reference
// follow-up: k-th char without expanding (LC 880) -> decoded size, scan back k %= size
// follow-up: (ab){3} format (J14: parentheses + repeat count in curly braces after ')')
//   -> same two stacks; '(' pushes, on ')' read the {k} that follows and repeat
