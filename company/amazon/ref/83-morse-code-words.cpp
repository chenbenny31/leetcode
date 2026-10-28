// LC 804: write each word as the concatenation of its letters' Morse codes; return how
// many different codes the words produce.

#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>

using std::cout;
using std::string;
using std::vector;
using std::unordered_set;

// build each word's code and count distinct ones. T: O(total letters), S: O(total).
int uniqueMorse(const vector<string>& words) {
    static const vector<string> code = {
        ".-",   "-...", "-.-.", "-..",  ".",   "..-.", "--.",  "....", "..",
        ".---", "-.-",  ".-..", "--",   "-.",  "---",  ".--.", "--.-", ".-.",
        "...",  "-",    "..-",  "...-", ".--", "-..-", "-.--", "--.."};
    unordered_set<string> seen;
    for (const string& w : words) {
        string m;
        for (char c : w) { m += code[c - 'a']; }
        seen.insert(m);
    }
    return seen.size();
}

void test(const string& label, const vector<string>& words, int want) {
    cout << label << (uniqueMorse(words) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("collide", {"a", "et"}, 1);
    test("differ", {"a", "b"}, 2);
    test("dup-word", {"z", "z"}, 1);
}
