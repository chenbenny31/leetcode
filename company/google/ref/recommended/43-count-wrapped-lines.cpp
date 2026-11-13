// Count Wrapped Lines, two parts (Google, interview report)
// greedy wrap: word joins the line iff used + 1 + len <= width; words split on whitespace
// word longer than width -> new line, takes ceil(len / width) lines alone; no words -> 0
// (1) number of lines for text at maxWidth
// (2) two texts as "left | right" in total width T; {min height, smallest left width}
//
// clarify:
// separators -> any run of spaces / tabs / newlines = one
// maxWidth >= 1, T >= 5 -> both columns get width >= 1
// word longer than width -> split across lines (vs error / hyphenation)
// text len 1e5, T 1e3 -> O(T * words) ok; binary search version below

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

// word lens of text, plain char loop; O(len)
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

// greedy wrap over word lens; O(words)
int linesFor(const vector<int>& words, int width) {
    int lines = 0;
    // chars used on current line, 0 = no open line
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

// (1) count lines
// core idea: collapse whitespace into word lens, then greedy wrap
// T: O(len), S: O(words)
int countLines(const string& text, int maxWidth) {
    return linesFor(wordLengths(text), maxWidth);
}

// (2) two columns, try every width
// core idea: " | " takes 3 columns; try left width w in [1, T - 4], right gets T - 3 - w
// T: O(T * words), S: O(words)
pair<int, int> twoColumns(const string& left, const string& right, int total) {
    vector<int> leftWords = wordLengths(left);
    vector<int> rightWords = wordLengths(right);
    int bestHeight = -1;
    int bestWidth = -1;
    for (int width = 1; width <= total - 4; width++) {
        int leftLines = linesFor(leftWords, width);
        int height = max(leftLines, linesFor(rightWords, total - 3 - width));
        // strict <, so the smallest width wins a tie
        if (bestHeight == -1 || height < bestHeight) {
            bestHeight = height;
            bestWidth = width;
        }
    }
    return {bestHeight, bestWidth};
}

// (1) explicit newlines honored
// core idea: each '\n' ends a paragraph; wrap each alone and add the cnts
// blank paragraph still takes 1 line; trailing '\n' adds none
// T: O(len), S: O(len)
int countLinesParagraphs(const string& text, int maxWidth) {
    int lines = 0;
    // chars of current paragraph, w/o its '\n'
    string para;
    for (char c : text) {
        if (c != '\n') {
            para += c;
            continue;
        }
        lines += max(1, countLines(para, maxWidth));
        para.clear();
    }
    // last paragraph has no '\n' after it
    if (!para.empty()) { lines += max(1, countLines(para, maxWidth)); }
    return lines;
}

// (2) two columns, binary search
// core idea: left(w) never rises, right(T - 3 - w) never falls -> binary search the cross
// best height = right(cross) or left(cross - 1); then first w w/ left(w) <= best
// T: O(log T * words), S: O(words)
pair<int, int> twoColumnsFast(const string& left, const string& right, int total) {
    vector<int> leftWords = wordLengths(left);
    vector<int> rightWords = wordLengths(right);
    int maxWidth = total - 4;
    // first w in [1, maxWidth] w/ left(w) <= right(w); maxWidth + 1 if none
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
    // cross == maxWidth + 1: no crossing, right width there would be 0
    if (cross <= maxWidth) { bestHeight = linesFor(rightWords, total - 3 - cross); }
    // cross == 1: no width before it
    if (cross > 1) { bestHeight = min(bestHeight, linesFor(leftWords, cross - 1)); }
    // first w in [1, maxWidth] w/ left(w) <= bestHeight; an optimal w always qualifies
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
    testColumns("two-rows", "aaaa bbbb", "cc", 8, {2, 4});
    testColumns("empty-right", "a b", "", 7, {1, 3});
    testColumns("tie-range", "aaa bbb", "c", 10, {2, 3});
}

// follow-up:
// (2) ternary search on max(left, right) -> no: flat steps break unimodal search
// (2) only "better of cross and cross - 1" -> height right, can miss smallest tied width
// return rendered lines -> split into word strings, same greedy; cut long words by width
// streaming text -> keep only (lines, used); carry a partial word len across chunks
