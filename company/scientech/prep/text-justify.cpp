#include <iostream>
#include <vector>
#include <string>

using std::vector;
using std::string;
using std::cout;

vector<string> fullJustify(const vector<string>& words, int maxWidth) {
    vector<string> out;
    vector<string> curWords;
    int curLen = 0;

    for (const auto& word : words) {
        if (curLen + (int)curWords.size() + (int)word.size() > maxWidth) {
            int total = maxWidth - curLen;
            if (curWords.size() == 1) { // one word line is left-justify
                out.push_back(curWords[0] + string(total, ' '));
            } else { // dist even, extra to left
                int gaps = curWords.size() - 1;
                int each = total / gaps;
                int extra = total % gaps;
                string line;
                for (int i = 0; i < (int)curWords.size(); i++) {
                    line += curWords[i];
                    if (i < gaps) {
                        line += string(each, ' ');
                        if (i < extra) { line += " "; }
                    }
                }
                out.push_back(line);
            }
            curWords.clear();
            curLen = 0;
        }

        curWords.push_back(word);
        curLen += word.size();
    }

    // lastline is left-justify
    string lastline = curWords[0];
    for (int i = 1; i < (int)curWords.size(); i++) {
        lastline += " " + curWords[i];
    }
    lastline += string(maxWidth - (int)lastline.size(), ' ');
    out.push_back(lastline);
    return out;
}

// center -> (maxWidth - line-len) / 2, space left then rest right

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

