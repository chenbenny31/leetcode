// Count Wrapped Lines, two parts (Google, interview report). Words are maximal runs of
// non-whitespace; any run of spaces / tabs / newlines is one separator (explicit newlines
// are not honored). Greedy wrap: a word joins the current line iff len + 1 + word <=
// width. A word longer than width starts a new line, takes ceil(len / width) lines alone,
// and the next word starts a new line. No words -> 0 lines.
// (1) Return the number of lines for text at maxWidth.
// (2) Two texts side by side as "left | right" in total width T (T >= 5, so both columns
//     get width >= 1). Return {min height, left width}, smallest left width on ties.

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::pair;
using std::max;

// word lengths of text, split with a plain character loop. T: O(len), S: O(words).
vector<int> wordLengths(const string& text) {
    vector<int> words;
    int len = 0;
    for (char c : text) {
        if (c == ' ' || c == '\n' || c == '\t') {
            if (len > 0) { words.push_back(len); }
            len = 0;
        } else {
            len++;
        }
    }
    if (len > 0) { words.push_back(len); }
    return words;
}

// greedy wrap over word lengths. T: O(words), S: O(1).
int linesFor(const vector<int>& words, int width) {
    int lines = 0;
    // characters used on the current line, 0 = no open line
    int used = 0;
    for (int len : words) {
        if (len > width) {
            lines += (len + width - 1) / width;
            used = 0;
        } else if (used > 0 && used + 1 + len <= width) {
            used += 1 + len;
        } else {
            lines++;
            used = len;
        }
    }
    return lines;
}

// (1) split, then greedy wrap. T: O(len), S: O(words).
int countLines(const string& text, int maxWidth) {
    return linesFor(wordLengths(text), maxWidth);
}

// (2) try every left width w; the right column gets T - 3 - w.
// T: O(T * (wordsLeft + wordsRight)), S: O(words).
pair<int, int> twoColumns(const string& left, const string& right, int total) {
    vector<int> leftWords = wordLengths(left);
    vector<int> rightWords = wordLengths(right);
    int bestHeight = -1;
    int bestWidth = -1;
    for (int width = 1; width <= total - 4; width++) {
        int leftLines = linesFor(leftWords, width);
        int height = max(leftLines, linesFor(rightWords, total - 3 - width));
        if (bestHeight == -1 || height < bestHeight) {
            bestHeight = height;
            bestWidth = width;
        }
    }
    return {bestHeight, bestWidth};
}

void testLines(const string& label, const string& text, int maxWidth, int want) {
    cout << label << (countLines(text, maxWidth) == want ? " PASS\n" : " FAIL\n");
}

void testColumns(const string& label, const string& left, const string& right, int total,
                 pair<int, int> want) {
    cout << label << (twoColumns(left, right, total) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    testLines("fit-exact", "the cat sat", 7, 2);
    testLines("spaces-newlines", "  a  b\nc ", 3, 2);
    testLines("long-word", "hi abcdefg ok", 3, 5);
    testLines("empty", "   ", 5, 0);

    testColumns("balanced", "aa bb", "cc dd", 13, {1, 5});
    testColumns("skewed", "a", "bb cc dd", 12, {1, 1});
    testColumns("two-rows", "aaaa bbbb", "cc", 8, {2, 4});
    testColumns("empty-right", "a b", "", 7, {1, 3});
}

// follow-up: (1) honor explicit newlines -> split text on '\n' first, sum the paragraphs'
//   counts (an empty paragraph still takes 1 line)
// follow-up: (2) faster -> linesFor is non-increasing in width, so left(w) falls while
//   right(T - 3 - w) rises: binary search the first w with left(w) <= right(T - 3 - w),
//   answer = better of w and w - 1 -> O(log T) wraps. Do NOT ternary search the max
//   itself: its flat steps break unimodal search.
// follow-up: return the rendered lines -> same greedy, append words to a string per line
