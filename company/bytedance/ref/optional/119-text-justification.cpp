// ByteDance / LC 68: pack words greedily into lines of exactly maxWidth chars; spread
// extra spaces as evenly as possible, left gaps get more. The last line and single-word
// lines are left-justified (single spaces) and padded with spaces on the right.
#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// Greedy line packing, then distribute the gaps. T: O(L), S: O(L); L = output length.
vector<string> fullJustify(const vector<string>& words, int maxWidth) {
    // finished lines, each exactly maxWidth chars
    vector<string> lines;
    int n = words.size();
    int i = 0;
    while (i < n) {
        // words[i..j) fit on this line; len = their letters, spaces not counted
        int j = i;
        int len = 0;
        while (j < n && len + (int)words[j].size() + (j - i) <= maxWidth) {
            len += words[j].size();
            j++;
        }
        int gaps = j - i - 1;
        string line = words[i];
        if (j == n || gaps == 0) {
            for (int k = i + 1; k < j; k++) { line += ' ' + words[k]; }
            line += string(maxWidth - (int)line.size(), ' ');
        } else {
            int spaces = maxWidth - len;
            for (int k = i + 1; k < j; k++) {
                // the first spaces % gaps gaps take one extra space
                int g = spaces / gaps + (k - i <= spaces % gaps ? 1 : 0);
                line += string(g, ' ') + words[k];
            }
        }
        lines.push_back(line);
        i = j;
    }
    return lines;
}

void test(const string& label, const vector<string>& words, int maxWidth,
          const vector<string>& want) {
    cout << label << (fullJustify(words, maxWidth) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc-ex1", {"This", "is", "an", "example", "of", "text", "justification."}, 16,
         {"This    is    an", "example  of text", "justification.  "});
    test("lc-ex2", {"What", "must", "be", "acknowledgment", "shall", "be"}, 16,
         {"What   must   be", "acknowledgment  ", "shall be        "});
    test("uneven-gaps", {"a", "b", "c", "dddd"}, 6, {"a  b c", "dddd  "});
}

// follow-up: minimize raggedness (word wrap) -> dp[i] = min over j of (slack of words
//   i..j)^2 + dp[j+1], last line free, O(n^2)
// follow-up: sentence screen fitting (LC 418) -> s = words joined + ' ', pos += cols per
//   row, back pos up to a word start; answer pos / s.size()
// follow-up: word longer than maxWidth -> LC guarantees not; else hard-split it into
//   maxWidth chunks
