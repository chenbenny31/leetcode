// 24 · Decode String — LC 394, 339
// Mother: select/05-stack/05-decode-string.cpp
// (1) LC 394: decode k[encoded] = encoded repeated k times; brackets may nest
// (2) LC 339 from a raw string like "[[2],-3]": sum of each int times its depth
// reported: LC 394 x4, {k} decompression, nested list weight sum x3, 1p3a decode x3
// reported: (2) as "[3, 8, [2, 14], [2, [91]]]" after the DFS and BFS versions
//
// clarify:
// input valid and balanced? -> yes
// multi-digit counts -> yes, build num digit by digit
// plain text outside brackets -> kept as is
// (2) spaces, negative nums, empty lists -> all handled

#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::string;
using std::vector;

// (1) decode k[encoded] (LC 394)
// core idea: 2 stacks; at '[' push (count, text so far) and start fresh
// at ']' pop, append inner text count times to the saved text
// T: O(output len), S: O(output len)
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

// (2) nested list weight sum from a raw string (LC 339)
// core idea: '[' / ']' track depth (outermost = 1); '-' and digits build the num
// ',' or ']' flushes the num at the current depth
// T: O(n), S: O(1)
int depthSum(const string& s) {
    int depth = 0;
    int total = 0;
    int num = 0;
    int sign = 1;
    // a num was read since the last flush ("[]" and "]," flush nothing)
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
            // flush before ']' lowers depth: the num belongs to this list
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

// follow-up:
// recursive version -> decode(s, i) w/ i by ref; on k[ recurse, append k times
// k-th char w/o expanding (LC 880) -> compute decoded size, scan back w/ k %= size
// (2) on the NestedInteger API -> DFS(list, depth): int adds val * depth, list recurses
// (2) BFS version -> queue of lists; whole level at once, depth++ per level
// (2) no outer brackets, "3, [2]" -> depth starts at 1; flush once more after the loop
// (2) inverse weights (LC 364) -> max depth first, weight = maxDepth - depth + 1
// "(ab){3}" format -> same 2 stacks; on ')' read the {k} that follows, repeat
