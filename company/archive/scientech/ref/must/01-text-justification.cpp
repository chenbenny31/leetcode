// 01 · Text Justification — LC 68 (Hard), Quant Dev interview 2023
// pack words greedily into lines of exactly maxWidth chars, spaces between words
// leftover spaces spread over the gaps, left gaps get the extra ones
// last line and one-word lines are left-justified (pad the right end)
//
// clarify:
// word longer than maxWidth -> never
// empty word list -> return no lines
// uneven spaces -> gaps on the left get one more
// last line -> single spaces, pad the right end
// line with one word -> left-justified (no gap to stretch)

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// core idea: collect words until the next one would overflow, then emit the full line;
// the last line is built after the loop
// T: O(input chars + output chars), S: O(maxWidth) besides the output
vector<string> fullJustify(const vector<string>& words, int maxWidth) {
    vector<string> out;
    if (words.empty()) { return out; }

    vector<string> curWords; // words of the line being filled
    int curLen = 0;          // letters in curWords, spaces not counted
    for (const string& word : words) {
        // curWords.size() = spaces needed if word joins: one before each later word
        if (curLen + (int)curWords.size() + (int)word.size() > maxWidth) {
            int total = maxWidth - curLen; // spaces to place on this line
            if (curWords.size() == 1) {
                out.push_back(curWords[0] + string(total, ' '));
            } else {
                int gaps = curWords.size() - 1;
                int each = total / gaps;
                int extra = total % gaps;
                string line;
                for (int i = 0; i < (int)curWords.size(); i++) {
                    line += curWords[i];
                    if (i < gaps) {
                        line += string(each, ' ');
                        // the first `extra` gaps get one more
                        if (i < extra) { line += ' '; }
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

    // last line: single spaces, pad the right end
    string last = curWords[0];
    for (int i = 1; i < (int)curWords.size(); i++) { last += " " + curWords[i]; }
    last += string(maxWidth - (int)last.size(), ' ');
    out.push_back(last);
    return out;
}

void test(const string& label, const vector<string>& words, int maxWidth,
          const vector<string>& want) {
    cout << label << (fullJustify(words, maxWidth) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("empty", {}, 6, {});
    test("extra-goes-left", {"a", "b", "c", "d"}, 6, {"a  b c", "d     "});
    test("one-word-line", {"data", "structures", "ok"}, 10,
         {"data      ", "structures", "ok        "});
}

// follow-up:
// 1. right-justify -> same packing; all spaces go before the first word
// 2. center -> same packing; (maxWidth - line len) / 2 spaces left, the rest right
// 3. word longer than maxWidth -> cut it into maxWidth-sized pieces before packing
// 4. words arrive as a stream -> buffer one line, emit it when the next word won't fit
// 5. rearrange spaces in one line (LC 1592) -> the same total / gaps + extra split
