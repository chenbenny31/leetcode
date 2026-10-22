// LC 151 Reverse Words in a String: reverse the order of the words in s. Words are
// separated by one or more spaces; drop leading, trailing and repeated spaces.
// Follow-up: if the string is mutable, solve it in O(1) extra space.
#include <iostream>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::reverse;

// Scan from the back and append each word found. T: O(n), S: O(n).
string reverseWords(string s) {
    string res;
    int i = s.size() - 1;
    while (i >= 0) {
        if (s[i] == ' ') {
            i--;
            continue;
        }
        int end = i;   // last char of the word
        while (i >= 0 && s[i] != ' ') {
            i--;
        }
        if (!res.empty()) { res += ' '; }
        res += s.substr(i + 1, end - i);
    }
    return res;
}

// Follow-up: reverse everything, then reverse each word while compacting spaces
// leftwards. T: O(n), S: O(1).
void reverseWordsInPlace(string& s) {
    reverse(s.begin(), s.end());
    int n = s.size(), w = 0;   // w = write position
    for (int i = 0; i < n; i++) {
        if (s[i] == ' ') { continue; }
        if (w > 0) {
            s[w] = ' ';
            w++;
        }
        int start = w;
        while (i < n && s[i] != ' ') {
            s[w] = s[i];
            w++;
            i++;
        }
        reverse(s.begin() + start, s.begin() + w);
    }
    s.resize(w);
}

void test(const string& label, const string& s, const string& want) {
    cout << label << (reverseWords(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("three-words", "red green blue", "blue green red");
    test("extra-spaces", "  a   bc  ", "bc a");
    test("one-word", "hello", "hello");
}
