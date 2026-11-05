// ByteDance / LC 394: decode k[encoded] = encoded repeated k times; brackets may nest.
// Input is valid, digits appear only as repeat counts.
#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// Two stacks saved at each '['. T: O(output length), S: O(output length).
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
    test("flat", "3[a]2[bc]", "aaabcbc");
    test("nested", "3[a2[c]]", "accaccacc");
    test("trailing-text", "2[abc]3[cd]ef", "abcabccdcdcdef");
}

// follow-up: recursive version -> decode(s, i) reads until ']' or end; on digits read k,
//   skip '[', recurse, append result k times; index i shared by reference
// follow-up: k-th char without expanding (LC 880) -> compute decoded size; scan back:
//   k %= size, letter with k == 0 is the answer, digit d -> size /= d, letter -> size--
// follow-up: decoded length only -> same stacks of lengths: len = prev + k * len, O(n)
