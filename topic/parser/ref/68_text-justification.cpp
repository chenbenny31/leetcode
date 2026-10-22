// LC 68 Text Justification: pack words greedily into lines of exactly maxWidth chars.
// Spread extra spaces evenly between words, extras to the left. The last line and
// single-word lines are left-justified with trailing spaces.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// Greedy line packing, then distribute spaces by gap. T: O(total chars), S: O(output).
vector<string> fullJustify(vector<string>& words, int maxWidth) {
    int n = words.size();
    vector<string> lines;
    int i = 0;
    while (i < n) {
        // words[i..j) fit on one line with one space between each; len = letters only
        int j = i, len = 0;
        while (j < n && len + (int)words[j].size() + (j - i) <= maxWidth) {
            len += words[j].size();
            j++;
        }
        int gaps = j - i - 1;
        string line = words[i];
        if (j == n || gaps == 0) {
            for (int k = i + 1; k < j; k++) {
                line += " " + words[k];
            }
            line += string(maxWidth - line.size(), ' ');
        } else {
            int spaces = maxWidth - len;
            for (int k = i + 1; k < j; k++) {
                // the first (spaces % gaps) gaps get one extra space
                int cnt = spaces / gaps + (k - i <= spaces % gaps ? 1 : 0);
                line += string(cnt, ' ') + words[k];
            }
        }
        lines.push_back(line);
        i = j;
    }
    return lines;
}

void test(const string& label, vector<string> words, int maxWidth,
          const vector<string>& want) {
    cout << label << (fullJustify(words, maxWidth) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("extra-left", {"go", "to", "the", "big", "end"}, 10,
         {"go  to the", "big end   "});
    test("single-word-line", {"ab", "cdefgh", "i"}, 6, {"ab    ", "cdefgh", "i     "});
    test("one-word", {"hi"}, 4, {"hi  "});
}
