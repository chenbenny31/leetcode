// LC 394 Decode String: decode k[encoded] into encoded repeated k times; brackets can
// nest. The input is always valid, digits only appear as repeat counts.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;
using std::pair;

// Explicit stack of outer contexts; on ']' repeat the inner text and append it to the
// outer prefix. T: O(output length), S: O(output length).
string decodeString(const string& s) {
    vector<pair<string, int>> stk;   // (text before the '[', repeat count of that '[')
    string cur;                      // text decoded so far at the current depth
    int k = 0;                       // repeat count being read
    for (char c : s) {
        if (c >= '0' && c <= '9') {
            k = k * 10 + (c - '0');
        } else if (c == '[') {
            stk.push_back({cur, k});
            cur = "";
            k = 0;
        } else if (c == ']') {
            auto [prev, times] = stk.back();
            stk.pop_back();
            for (int i = 0; i < times; i++) { prev += cur; }
            cur = prev;
        } else {
            cur += c;
        }
    }
    return cur;
}

void test(const string& label, const string& s, const string& want) {
    if (decodeString(s) == want) {
        cout << label << " PASS\n";
    } else {
        cout << label << " FAIL\n";
    }
}

int main() {
    test("flat", "2[ab]c", "ababc");
    test("nested", "2[a3[b]]", "abbbabbb");
    test("two-digit", "10[x]", "xxxxxxxxxx");
    test("no-brackets", "abc", "abc");
}
