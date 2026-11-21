// 07 · Parse Citations, Then Merge (Moveworks onsite 2025, Medium) — char-loop parse
// (1) text like "see [3], also [4-10]" -> the valid citation ranges in order:
//     [3] -> {3, 3}, [4-10] -> {4, 10}; no regex
// (2) parse two texts and merge all their ranges into sorted, non-overlapping ranges
//
// clarify:
// valid inside the brackets -> "n" or "a-b", digits only, a <= b; anything else skipped
// spaces inside ("[ 3]") -> invalid (assumption; ask)
// unclosed '[' -> skipped
// merge touching ranges ([1-3] and [4]) -> yes, citation numbers are ints (assumption)
// number size -> at most 9 digits so it fits int

#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::pair;
using std::sort;
using std::max;

// "123" -> 123, -1 if empty, not all digits, or too long. T: O(len), S: O(1)
int parseNumber(const string& s) {
    if (s.empty() || s.size() > 9) { return -1; }
    int value = 0;
    for (char c : s) {
        if (c < '0' || c > '9') { return -1; }
        value = value * 10 + (c - '0');
    }
    return value;
}

// (1) core idea: find each '[', read up to the next ']', split the inside at '-'
// T: O(len), S: O(len)
vector<pair<int, int>> parseCitations(const string& text) {
    int n = text.size();
    vector<pair<int, int>> out;
    int i = 0;
    while (i < n) {
        if (text[i] != '[') {
            i++;
            continue;
        }
        int close = i + 1;
        while (close < n && text[close] != ']' && text[close] != '[') { close++; }
        // no ']' before the end or before the next '[' -> not a citation
        if (close == n || text[close] == '[') {
            i = close;
            continue;
        }
        string inside = text.substr(i + 1, close - i - 1);
        int dash = -1;
        for (int k = 0; k < (int)inside.size(); k++) {
            if (inside[k] == '-') {
                dash = k;
                break;
            }
        }
        int lo = -1;
        int hi = -1;
        if (dash == -1) {
            lo = parseNumber(inside);
            hi = lo;
        } else {
            lo = parseNumber(inside.substr(0, dash));
            hi = parseNumber(inside.substr(dash + 1));
        }
        if (lo != -1 && hi != -1 && lo <= hi) { out.push_back({lo, hi}); }
        i = close + 1;
    }
    return out;
}

// (2) core idea: parse both, sort by start, extend the last range while they touch
// T: O(len + r log r), S: O(r) — r = num of ranges
vector<pair<int, int>> mergeCitations(const string& a, const string& b) {
    vector<pair<int, int>> all = parseCitations(a);
    vector<pair<int, int>> more = parseCitations(b);
    all.insert(all.end(), more.begin(), more.end());
    sort(all.begin(), all.end());
    vector<pair<int, int>> merged;
    for (const auto& range : all) {
        // + 1: [1-3] and [4] touch, so they merge
        if (!merged.empty() && range.first <= merged.back().second + 1) {
            merged.back().second = max(merged.back().second, range.second);
        } else {
            merged.push_back(range);
        }
    }
    return merged;
}

void test(const string& label, const vector<pair<int, int>>& got,
          const vector<pair<int, int>>& want) {
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("p1-single-and-range", parseCitations("see [3], also [4-10]"),
         {{3, 3}, {4, 10}});
    test("p1-invalid-skipped", parseCitations("[a] [5-2] [ 7] [-3] [3"), {});
    test("p1-nested-open", parseCitations("[[8]"), {{8, 8}});
    test("p2-merge", mergeCitations("[1-3] [8]", "[2-5] [9-12]"), {{1, 5}, {8, 12}});
    test("p2-touching", mergeCitations("[1-3]", "[4]"), {{1, 4}});
    return 0;
}

// follow-up:
// report why a citation is invalid -> return (range, error) pairs instead of skipping
// list form "[3, 5, 7-9]" -> split the inside on ',' first, then the same per piece
// only overlapping (not touching) ranges merge -> use range.first <= back.second
// many texts, ranges arrive over time -> ordered map start -> end, merge w/ neighbors
// merge intervals mother problem -> select/11-intervals-greedy/01-merge-intervals
