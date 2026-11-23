// 01 · Text Justification — LC 68 (Hard)
// pack words greedily into lines of exactly maxWidth chars, spaces between words
// leftover spaces spread over the gaps, left gaps get the extra ones
// last line and one-word lines are left-justified (pad the right end)
//
// clarify:
// word longer than maxWidth -> never
// empty word list -> not given (LC: at least one word)
// uneven spaces -> gaps on the left get one more
// last line -> single spaces, pad the right end
// line with one word -> left-justified (no gap to stretch)

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// core idea: collect words until the next one won't fit, then justify those words;
// the last line is left-justified
// T: O(total chars), S: O(maxWidth) besides the output
vector<string> fullJustify(const vector<string>& words, int maxWidth) {
    vector<string> result;
    // words of the line being filled
    vector<string> curWords;
    // letters only; curWords.size() = min spaces needed once the next word joins
    int curLen = 0;

    for (const string& word : words) {
        if (curLen + (int)word.size() + (int)curWords.size() > maxWidth) {
            int totalSpaces = maxWidth - curLen;
            if (curWords.size() == 1) {
                // one-word line: left-justified
                result.push_back(curWords[0] + string(totalSpaces, ' '));
            } else {
                int gaps = curWords.size() - 1;
                int sepSpaces = totalSpaces / gaps;
                int extSpaces = totalSpaces % gaps;
                string line;
                for (int i = 0; i < (int)curWords.size(); i++) {
                    line += curWords[i];
                    if (i < gaps) {
                        line += string(sepSpaces, ' ');
                        // extra spaces go to the left gaps
                        if (i < extSpaces) { line += ' '; }
                    }
                }
                result.push_back(line);
            }
            curWords.clear();
            curLen = 0;
        }
        curWords.push_back(word);
        curLen += word.size();
    }

    // last line: left-justified
    string lastLine = curWords[0];
    for (int i = 1; i < (int)curWords.size(); i++) { lastLine += " " + curWords[i]; }
    lastLine += string(maxWidth - (int)lastLine.size(), ' ');
    result.push_back(lastLine);
    return result;
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

// follow-up:
// 1. right-justify -> same packing; all spaces go before the first word
// 2. center -> same packing; (maxWidth - line len) / 2 spaces left, the rest right
// 3. word longer than maxWidth -> cut it into maxWidth-sized pieces before packing
// 4. words arrive as a stream -> keep curWords; emit a line when the next word won't fit
// 5. rearrange spaces in one line (LC 1592) -> the same total / gaps + extra split
