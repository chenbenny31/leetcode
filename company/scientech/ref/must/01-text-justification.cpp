// 01 · Text Justification — LC 68 (Hard)
// pack words greedily into lines of exactly maxWidth chars, spaces between words
// leftover spaces spread over the gaps, left gaps get the extra ones
// last line and one-word lines are left-justified (pad the right end)
//
// clarify:
// word longer than maxWidth -> never
// empty word list -> return {}
// uneven spaces -> gaps on the left get one more
// last line -> single spaces, pad the right end
// line w/ one word -> same as the last line (no gap to stretch)

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// core idea: take as many words as fit w/ one space each, then stretch the gaps
// each gap gets spaces / gaps; the first spaces % gaps gaps get one more
// T: O(total chars), S: O(maxWidth) besides the output
vector<string> fullJustify(const vector<string>& words, int maxWidth) {
    int n = words.size();
    vector<string> lines;
    int first = 0;
    while (first < n) {
        // [first, last) = words on this line; lettersLen = their letters only
        int last = first;
        int lettersLen = 0;
        while (last < n) {
            int wordLen = words[last].size();
            // (last - first) = single spaces needed if this word joins
            if (lettersLen + wordLen + (last - first) > maxWidth) { break; }
            lettersLen += wordLen;
            last++;
        }

        int gaps = last - first - 1;
        string line;
        if (last == n || gaps == 0) {
            // left-justified: single spaces, then pad the right end
            for (int w = first; w < last; w++) {
                if (w > first) { line += ' '; }
                line += words[w];
            }
            line += string(maxWidth - (int)line.size(), ' ');
        } else {
            int spaces = maxWidth - lettersLen;
            int each = spaces / gaps;
            int extra = spaces % gaps;
            for (int w = first; w < last; w++) {
                line += words[w];
                // no gap after the last word of the line
                if (w == last - 1) { break; }
                int pad = each;
                if (w - first < extra) { pad++; }
                line += string(pad, ' ');
            }
        }
        lines.push_back(line);
        first = last;
    }
    return lines;
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
    test("empty", {}, 5, {});
}

// follow-up:
// right-justify -> same packing; all spaces go before the first word
// center -> same packing; (maxWidth - line len) / 2 spaces on the left, rest on the right
// word longer than maxWidth -> split it into maxWidth-sized pieces before packing
// words arrive as a stream -> keep the current line's words; emit when the next won't fit
// rearrange spaces in one line (LC 1592) -> the same each / extra split
