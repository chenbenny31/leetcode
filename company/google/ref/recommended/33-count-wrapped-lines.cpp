// Count Wrapped Lines, two parts (Google, interview report). Words are maximal runs of
// non-whitespace; any run of spaces / tabs / newlines is one separator (explicit newlines
// are not honored). Greedy wrap: a word joins the current line iff len + 1 + word <=
// width. A word longer than width starts a new line, takes ceil(len / width) lines alone,
// and the next word starts a new line. No words -> 0 lines.
// (1) Return the number of lines for text at maxWidth.
// (2) Two texts side by side as "left | right" in total width T (T >= 5, so both columns
//     get width >= 1). Return {min height, left width}, smallest left width on ties.
//
// Clarify (1):
// - separators -> space, tab, newline; a run of them is one separator (stated); '\r'?
//   -> assume none
// - text length up to ~1e5 -> one O(len) pass; keep word lengths, not word strings
// - maxWidth >= 1? -> assume so (width 0 divides by zero in the ceil)
// - a word longer than the width -> ceil(len / width) lines alone (stated); confirm vs
//   an error or hyphenation
// - no words (empty or only whitespace) -> 0 lines
// Clarify (2):
// - " | " takes 3 columns -> left width w, right width T - 3 - w, so w in [1, T - 4]
// - an empty column has 0 lines -> the height is the other column's
// - ties -> smallest left width (stated); T up to ~1e3 -> trying every w is fine
// Idea: (1) collapse whitespace into word lengths, then greedy: a word joins the open
//   line iff used + 1 + len <= width, else it opens a new line. (2) a column never needs
//   more lines when it is wider, so left(w) falls and right(T - 3 - w) rises with w: try
//   every w, or binary search where they cross (twoColumnsFast).
// Pitfalls (1):
// - the first word of a line costs len, not 1 + len: test used > 0 before joining
//   (without it the first word never counts a line)
// - long word: (len + width - 1) / width lines, then used = 0 so the next word does not
//   join the long word's last piece
// - push the last word after the loop (text need not end in whitespace)
// Pitfalls (2):
// - the right width is T - 3 - w; w = T - 3 gives width 0 (division by zero)
// - update only on height < best (strict), so the smallest w wins a tie
// - fast: hi starts at maxWidth + 1 (= no crossing); check cross <= maxWidth before
//   right(cross) and cross > 1 before left(cross - 1)
// Tests to write: (1) exact fit; runs of spaces / newlines; long word between short ones;
//   only spaces -> 0. (2) balanced; skewed; two rows; empty right column; one height
//   over a range of widths (smallest wins).

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <climits>

using std::cout;
using std::string;
using std::vector;
using std::pair;
using std::max;
using std::min;

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

// follow-up (1): honor explicit newlines. Each '\n' ends a paragraph; wrap each one
// alone and add the counts. A blank paragraph still takes 1 line; a trailing '\n' adds
// none. T: O(len), S: O(len).
int countLinesParagraphs(const string& text, int maxWidth) {
    int lines = 0;
    // characters of the current paragraph, without its '\n'
    string para;
    for (char c : text) {
        if (c != '\n') {
            para += c;
            continue;
        }
        lines += max(1, countLines(para, maxWidth));
        para.clear();
    }
    // the last paragraph has no '\n' after it
    if (!para.empty()) { lines += max(1, countLines(para, maxWidth)); }
    return lines;
}

// follow-up (2), faster: left(w) never rises and right(T - 3 - w) never falls as w
// grows, so "left <= right" is false...false true...true. Binary search the first w
// where it holds (cross); the best height is right(cross) or left(cross - 1). The
// smallest width with that height is the first w with left(w) <= best (right(w) <= best
// too: w is at most any optimal width). T: O(log T * words), S: O(words).
pair<int, int> twoColumnsFast(const string& left, const string& right, int total) {
    vector<int> leftWords = wordLengths(left);
    vector<int> rightWords = wordLengths(right);
    int maxWidth = total - 4;
    // first w in [1, maxWidth] with left(w) <= right(w); maxWidth + 1 if there is none
    int lo = 1;
    int hi = maxWidth + 1;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (linesFor(leftWords, mid) <= linesFor(rightWords, total - 3 - mid)) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    int cross = lo;
    int bestHeight = INT_MAX;
    // cross == maxWidth + 1: no crossing, and the right width there would be 0
    if (cross <= maxWidth) { bestHeight = linesFor(rightWords, total - 3 - cross); }
    // cross == 1: no width before it
    if (cross > 1) { bestHeight = min(bestHeight, linesFor(leftWords, cross - 1)); }
    // first w in [1, maxWidth] with left(w) <= bestHeight; an optimal w always qualifies
    lo = 1;
    hi = maxWidth;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (linesFor(leftWords, mid) <= bestHeight) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    return {bestHeight, lo};
}

void testLines(const string& label, const string& text, int maxWidth, int want) {
    cout << label << (countLines(text, maxWidth) == want ? " PASS\n" : " FAIL\n");
}

void testParagraphs(const string& label, const string& text, int maxWidth, int want) {
    bool ok = countLinesParagraphs(text, maxWidth) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void testColumns(const string& label, const string& left, const string& right, int total,
                 pair<int, int> want) {
    bool ok = twoColumns(left, right, total) == want &&
              twoColumnsFast(left, right, total) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    testLines("fit-exact", "the cat sat", 7, 2);
    testLines("spaces-newlines", "  a  b\nc ", 3, 2);
    testLines("long-word", "hi abcdefg ok", 3, 5);
    testLines("empty", "   ", 5, 0);

    testParagraphs("para-blank-line", "ab\n\ncd", 5, 3);
    testParagraphs("para-wrap", "aa bb cc\nd", 5, 3);
    testParagraphs("para-trailing-newline", "a b\n", 5, 1);
    testParagraphs("para-empty", "", 5, 0);

    testColumns("balanced", "aa bb", "cc dd", 13, {1, 5});
    testColumns("skewed", "a", "bb cc dd", 12, {1, 1});
    testColumns("two-rows", "aaaa bbbb", "cc", 8, {2, 4});
    testColumns("empty-right", "a b", "", 7, {1, 3});
    testColumns("tie-range", "aaa bbb", "c", 10, {2, 3});
}

// follow-up: (1) honor explicit newlines -> countLinesParagraphs above
// follow-up: (2) faster -> twoColumnsFast above. Do NOT ternary search the height
//   max(left, right) itself: its flat steps break unimodal search. Taking "the better of
//   cross and cross - 1" gets the height right but can miss the smallest width on a tie.
// follow-up: return the rendered lines -> split into word strings, same greedy; `open` =
//   the last line can still take words:
//   if (len > width) {
//       for (int i = 0; i < len; i += width) { lines.push_back(word.substr(i, width)); }
//       open = false;
//   } else if (open && (int)lines.back().size() + 1 + len <= width) {
//       lines.back() += " " + word;
//   } else { lines.push_back(word); open = true; }
// follow-up: streaming text -> linesFor keeps only (lines, used): feed each word as it
//   ends; a word may span two chunks, so carry its partial length across chunks
