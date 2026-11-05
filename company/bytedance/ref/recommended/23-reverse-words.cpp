// ByteDance (hack2hire) / LC 151: reverse the order of words in s (words = runs of
// non-space chars); output single-spaced with no leading/trailing spaces.
// Follow-ups: keep the original spacing (only move words); in place with O(1) space.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::reverse;

// Char-loop split into words, then join them in reverse. T: O(n), S: O(n).
string reverseWords(const string& s) {
    // words of s in original order
    vector<string> words;
    // word being read
    string word;
    // trailing sentinel space flushes the last word
    for (char c : s + ' ') {
        if (c != ' ') {
            word += c;
        } else if (!word.empty()) {
            words.push_back(word);
            word.clear();
        }
    }
    string result;
    for (int i = (int)words.size() - 1; i >= 0; i--) {
        if (!result.empty()) { result += ' '; }
        result += words[i];
    }
    return result;
}

// follow-up 1: collect words and the gaps around them, then refill the gap template
// with the words reversed. T: O(n), S: O(n).
string reverseWordsKeepSpacing(const string& s) {
    // words of s in original order
    vector<string> words;
    // gaps[i] = spaces before words[i]; gaps.back() = trailing spaces
    vector<string> gaps{""};
    int n = s.size();
    for (int i = 0; i < n; i++) {
        if (s[i] == ' ') {
            gaps.back() += ' ';
            continue;
        }
        // first char of a word opens the word and the gap after it
        if (i == 0 || s[i - 1] == ' ') {
            words.push_back("");
            gaps.push_back("");
        }
        words.back() += s[i];
    }
    int w = words.size();
    string result = gaps[0];
    for (int i = 0; i < w; i++) { result += words[w - 1 - i] + gaps[i + 1]; }
    return result;
}

// follow-up 2, O(1) extra space: three steps on the string itself.
//   1. remove extra spaces   "  hello   world " -> "hello world"
//   2. reverse everything    "hello world"      -> "dlrow olleh"
//   3. reverse each word     "dlrow olleh"      -> "world hello"
// Step 1: one space between words, none at the ends. s[0..w) = kept chars; w <= i, so
// writes never overtake reads. T: O(n).
void removeExtraSpaces(string& s) {
    int n = s.size();
    int w = 0;
    for (int i = 0; i < n; i++) {
        if (s[i] == ' ') { continue; }
        // a new word after an earlier one gets one separating space
        if (w > 0) {
            s[w] = ' ';
            w++;
        }
        while (i < n && s[i] != ' ') {
            s[w] = s[i];
            w++;
            i++;
        }
    }
    s.resize(w);
}

// Step 3: reverse every word; a word ends at a space or at the end of s. T: O(n).
void reverseEachWord(string& s) {
    int n = s.size();
    int start = 0;
    for (int i = 0; i <= n; i++) {
        if (i == n || s[i] == ' ') {
            reverse(s.begin() + start, s.begin() + i);
            start = i + 1;
        }
    }
}

// Same signature as LC 151. T: O(n), S: O(1) extra.
string reverseWordsInPlace(string s) {
    removeExtraSpaces(s);
    reverse(s.begin(), s.end());
    reverseEachWord(s);
    return s;
}

void test(const string& label, const string& s, const string& want) {
    bool ok = reverseWords(s) == want && reverseWordsInPlace(s) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void testKeep(const string& label, const string& s, const string& want) {
    cout << label << (reverseWordsKeepSpacing(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("simple", "the sky is blue", "blue is sky the");
    test("trim-and-gaps", "  a good   example ", "example good a");
    testKeep("keep-spacing", " hello  big world ", " world  big hello ");
}

// follow-up: keep original spacing -> see reverseWordsKeepSpacing() above; gap template
// follow-up: in place, O(1) space -> see reverseWordsInPlace() above; remove extra
//   spaces, reverse all, reverse each word
// follow-up: reverse chars of each word, keep word order (LC 557) -> reverse each
//   [start, end) in place, O(n)
