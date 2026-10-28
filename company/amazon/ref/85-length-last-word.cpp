// LC 58: s has words separated by spaces (maybe with extra spaces). Return the length of
// the last word.

#include <iostream>
#include <string>

using std::cout;
using std::string;

// skip trailing spaces from the end, then count letters. T: O(n), S: O(1).
int lengthOfLastWord(const string& s) {
    int i = (int)s.size() - 1;
    while (i >= 0 && s[i] == ' ') { i--; }
    int len = 0;
    while (i >= 0 && s[i] != ' ') {
        len++;
        i--;
    }
    return len;
}

void test(const string& label, const string& s, int want) {
    cout << label << (lengthOfLastWord(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("plain", "ab cde", 3);
    test("trailing", "ab c  ", 1);
    test("one-word", "word", 4);
}
