// pack words inti lines of exact maxWidth chars, space between words
// left gap get extra spaces, last line and one-word lines are left-adjust

// word never surpass maxWidth
// empty word list -> return {}
// uneven space -> extra assign to left gap
// last line -> single spaces, pad to right end
// line with one word -> same as last line

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// take as many words try to fit with one space, then stretch gaps with spaces/gaps
// the space % gaps get one more, T: O(total-chars), S: O(maxWidth)
vector<string> fullJustify(const vector<string>& words, int maxWidth) {
    vector<string> out;
    vector<string> curWords; // num of words for cur line
    int curLen = 0;

    for (const string& w : words) {
        // exceed maxWidth, assemble line and add to out
        if (curLen + (int)w.size() + (int)curWords.size() > maxWidth) {
            int total = maxWidth - curLen; // total spaces

            if (curWords.size() == 1) { // left-adjust
                out.push_back(curWords[0] + string(total, ' '));
            } else {
                int gaps = curWords.size() - 1;
                int sep = total / gaps;
                int ext = total % gaps;
                string line;
                for (int i = 0; i < (int)curWords.size(); i++) {
                    line += curWords[i];
                    if (i < gaps) {
                        line += string(sep, ' ');
                        if (i < ext) { line += ' '; }
                    } 
                }
                out.push_back(line);
            }

            curWords.clear();
            curLen = 0;
        }

        // not exceed maxWidth, load w
        curWords.push_back(w);
        curLen += w.size();
    }

    // last-line: left-adjust
    string lastline = curWords[0];
    for (int i = 1; i < (int)curWords.size(); i++) {
        lastline += " " + curWords[i];
    }
    lastline += string(maxWidth - (int)lastline.size(), ' ');
    out.push_back(lastline);
    return out;
}

void test(const string& label, const vector<string>& words, int maxWidth,
          const vector<string>& want) {
    cout << label << (fullJustify(words, maxWidth) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("extra-goes-left", {"a", "b", "c", "d"}, 6, {"a  b c", "d     "});
    test("even-gaps", {"to", "be", "or", "not", "to"}, 10, {"to  be  or", "not to    "});
    test("one-word-line", {"data", "structures", "ok"}, 10,
         {"data      ", "structures", "ok        "});
}
