// LC 394 Decode String (Medium) — two stacks saved at each '['
// Decode k[encoded] = encoded repeated k times; brackets may nest.
// Clarify: input always valid; digits appear only as repeat counts; multi-digit k;
//   output length limit.

#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::string;
using std::vector;

// Core idea: at '[' save (count, text so far) and start a fresh text; at ']' repeat the
// current text count times and append it to the saved text.
// T: O(output length), S: O(output length).
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
    return 0;
}

// follow-up (ideas only):
// - recursive version -> decode from a shared index until ']' or the end; on k[ recurse
//   and append the result k times
// - k-th char without expanding (LC 880) -> compute the decoded size, then scan back
//   with k %= size
// - (ab){3} format (count in curly braces after ')') -> same two stacks; '(' pushes, on
//   ')' read the {k} that follows and repeat
// - nested list weight sum from a raw string "[[2],-3]" (LC 339) -> char loop: '[' / ']'
//   track the depth, '-' and digits build the number, ',' or ']' adds number * depth
//   before the depth drops
// - encode / decode a list of strings (LC 271) -> length prefix "len#text": read the
//   length up to '#', then take exactly that many chars
