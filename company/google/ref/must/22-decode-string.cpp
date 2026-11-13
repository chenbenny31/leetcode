// 22 · Decode String — LC 394, 339
// Mother problem: select/05-stack/05-decode-string.cpp
//
// Google reports: LC 394 ×4, `{k}` decompression, nested list weight sum ×2
// (`depthSum`), 1p3a decode ×3
//
// LC 394: decode k[encoded] = encoded repeated k times; brackets may nest.
//
// Clarify
// - input valid, brackets balanced; digits only as repeat counts, may be multi-digit
// - plain text before / after brackets → kept as is
//
// Core idea
// - Two stacks: at '[' push (count, text so far) and start fresh; at ']' pop and append
//   the inner text count times to the saved text.
// - T O(output length), S O(output length).
//
// Follow-ups
// - nested list weight sum (LC 339) from a raw string → `depthSum` below
// - recursive version → decode(s, i) with i by reference; on k[ recurse, append k times
// - k-th char without expanding (LC 880) → compute the decoded size, scan back with
//   k %= size
// - "(ab){3}" format → same two stacks; on ')' read the {k} that follows and repeat

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

// LC 339 from a raw string like "[[2],-3]": sum of each integer times its depth
// (outermost list = 1). Assumes valid input with no spaces. Char loop: '[' / ']' track
// the depth, '-' and digits build the number, ',' or ']' flushes it. T: O(n), S: O(1).
int depthSum(const string& s) {
    int depth = 0;
    int total = 0;
    int num = 0;
    int sign = 1;
    // a number has been read since the last flush ("[]" and "]," flush nothing)
    bool inNumber = false;
    for (char c : s) {
        if (c == '[') {
            depth++;
        } else if (c == '-') {
            sign = -1;
        } else if (c >= '0' && c <= '9') {
            num = num * 10 + (c - '0');
            inNumber = true;
        } else if (c == ',' || c == ']') {
            // flush before ']' lowers the depth: the number belongs to this list
            if (inNumber) {
                total += sign * num * depth;
            }
            num = 0;
            sign = 1;
            inNumber = false;
            if (c == ']') {
                depth--;
            }
        }
    }
    return total;
}

void test(const string& label, const string& s, const string& want) {
    cout << label << (decodeString(s) == want ? " PASS\n" : " FAIL\n");
}

void testDepth(const string& label, const string& s, int want) {
    cout << label << (depthSum(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("flat", "2[ab]c", "ababc");
    test("nested", "2[a2[b]]", "abbabb");
    test("multi-digit", "10[a]", "aaaaaaaaaa");
    test("text-around", "x2[y]z", "xyyz");
    testDepth("depth-three-levels", "[1,[1,[1]]]", 6);
    testDepth("depth-negative", "[[-3],1]", -5);
    testDepth("depth-empty-lists", "[[],[[]],5]", 5);
}
