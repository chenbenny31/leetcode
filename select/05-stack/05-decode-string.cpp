// LC 394 Decode String (Medium) — two stacks saved at each '['
// decode k[encoded] = encoded repeated k times; brackets may nest
//
// clarify:
// input valid? -> yes
// digits -> only as repeat cnts, never in text
// multi-digit k -> yes, build num digit by digit
// output len -> can be huge; assume it fits in memory

#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::string;
using std::vector;

// core idea: at '[' save (cnt, text so far), start a fresh text
// at ']' repeat current text cnt times, append to the saved text
// T: O(output len), S: O(output len)
string decodeString(const string& s) {
    // counts = repeat cnt of each open bracket
    vector<int> counts;
    // prevs = text built before each open bracket
    vector<string> prevs;
    // cur = text inside the innermost open bracket so far
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

// follow-up:
// recursive version -> decode from shared idx until ']' or end; on k[ recurse, repeat k
// k-th char w/o expanding (LC 880) -> compute decoded size, scan back w/ k %= size
// (ab){3} format -> same two stacks; '(' pushes, on ')' read the {k} that follows
// nested list weight sum "[[2],-3]" (LC 339) -> char loop: track depth, add num * depth
// LC 339 on the NestedInteger API -> DFS(list, depth), or BFS w/ depth++ per level
// encode / decode list of strings (LC 271) -> "len#text": read len up to '#', take len
