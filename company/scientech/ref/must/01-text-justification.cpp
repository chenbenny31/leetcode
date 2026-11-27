// 01 · Text Justification — LC 68 (Hard)
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

// curWords holds one nonempty line, curLen counts its letters
string formatLine(const vector<string>& curWords, int curLen, int maxWidth, bool last) {
    int gaps = (int)curWords.size() - 1;
    int each = 1;
    int extra = 0;
    if (!last && gaps > 0) {
        int total = maxWidth - curLen;
        each = total / gaps;
        extra = total % gaps;
    }

    string line = curWords[0];
    for (int i = 0; i < gaps; i++) {
        line += string(each, ' ');
        if (i < extra) { line += ' '; }
        line += curWords[i + 1];
    }
    line.resize(maxWidth, ' ');
    return line;
}

// core idea: buffer words until the next one would overflow, then emit the full line
// T: O(input chars + output chars), S: O(maxWidth) besides the output
vector<string> fullJustify(const vector<string>& words, int maxWidth) {
    vector<string> out;
    vector<string> curWords;
    int curLen = 0;
    for (const string& word : words) {
        if (curLen + (int)curWords.size() + (int)word.size() > maxWidth) {
            out.push_back(formatLine(curWords, curLen, maxWidth, false));
            curWords.clear();
            curLen = 0;
        }
        curWords.push_back(word);
        curLen += word.size();
    }
    if (!curWords.empty()) {
        out.push_back(formatLine(curWords, curLen, maxWidth, true));
    }
    return out;
}

void test(const string& label, const vector<string>& words, int maxWidth,
          const vector<string>& want) {
    cout << label << (fullJustify(words, maxWidth) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("empty", {}, 6, {});
    test("extra-goes-left", {"a", "b", "c", "d"}, 6, {"a  b c", "d     "});
    test("even-gaps", {"to", "be", "or", "not", "to"}, 10, {"to  be  or", "not to    "});
    test("one-word-line", {"data", "structures", "ok"}, 10,
         {"data      ", "structures", "ok        "});
}

// follow-up:
// 1. right-justify -> same packing; all spaces go before the first word
// 2. center -> same packing; (maxWidth - line len) / 2 spaces left, the rest right
// 3. word longer than maxWidth -> cut it into maxWidth-sized pieces before packing
// 4. words arrive as a stream -> buffer one line, emit it when the next word won't fit
// 5. rearrange spaces in one line (LC 1592) -> the same total / gaps + extra split
