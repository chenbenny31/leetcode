// 02 · Lowest Proportion Strings (Moveworks HackerRank screen, Medium) — counting
// proportion(s) = (total count of all chars tied for the max freq in s) / len(s)
// (1) keep every string w/ the min proportion; from each, drop every char that occurs in
//     any other input string; concatenate the rest in input order
// (2) same, but among the kept strings c, score(c) = min sim(c, c') over the other kept
//     c'; keep only those w/ the min score (one kept string -> keep it) before dropping
// (3) implement sim: Jaccard similarity that counts repeated chars
//
// clarify:
// chars -> lowercase letters
// ties -> every string tied at the min proportion (or min score) is kept
// "other string" -> any other idx in the original list, kept or not
// (3) Jaccard w/ repeats -> sum of min counts / sum of max counts (multiset Jaccard);
//     two empty strings -> 0 (assumption)
// compare proportions -> exact w/ cross multiplication, no floats

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>

using std::cout;
using std::string;
using std::vector;
using std::max;
using std::min;
using std::fabs;

// (3) core idea: per letter, shared = min of the two counts, total = max of them
// T: O(len a + len b), S: O(1)
double sim(const string& a, const string& b) {
    vector<int> cntA(26, 0);
    vector<int> cntB(26, 0);
    for (char c : a) { cntA[c - 'a']++; }
    for (char c : b) { cntB[c - 'a']++; }
    int shared = 0;
    int total = 0;
    for (int i = 0; i < 26; i++) {
        shared += min(cntA[i], cntB[i]);
        total += max(cntA[i], cntB[i]);
    }
    if (total == 0) { return 0.0; }
    return (double)shared / total;
}

// numerator of proportion(s): max freq * num of chars that reach it
// T: O(len), S: O(1)
int tiedMaxTotal(const string& s) {
    vector<int> cnt(26, 0);
    for (char c : s) { cnt[c - 'a']++; }
    int top = 0;
    for (int f : cnt) { top = max(top, f); }
    int total = 0;
    for (int f : cnt) {
        if (f == top) { total += f; }
    }
    return total;
}

// idxs of the strings w/ the min proportion
// T: O(total len), S: O(n)
vector<int> minProportion(const vector<string>& words) {
    int n = words.size();
    vector<int> keep;
    // best proportion so far = bestNum / bestDen
    long long bestNum = 1;
    long long bestDen = 0;
    for (int i = 0; i < n; i++) {
        long long num = tiedMaxTotal(words[i]);
        long long den = words[i].size();
        // num / den vs bestNum / bestDen, w/o division
        long long lhs = num * bestDen;
        long long rhs = bestNum * den;
        if (keep.empty() || lhs < rhs) {
            keep = {i};
            bestNum = num;
            bestDen = den;
        } else if (lhs == rhs) {
            keep.push_back(i);
        }
    }
    return keep;
}

// kept strings w/o the chars that occur in any other input string, concatenated
// T: O(total len), S: O(1) besides the output
string dropShared(const vector<string>& words, const vector<int>& keep) {
    // inWords[c] = num of input strings that contain c
    vector<int> inWords(26, 0);
    for (const string& w : words) {
        vector<bool> seen(26, false);
        for (char c : w) { seen[c - 'a'] = true; }
        for (int c = 0; c < 26; c++) {
            if (seen[c]) { inWords[c]++; }
        }
    }
    string out;
    for (int i : keep) {
        for (char c : words[i]) {
            // words[i] is one of them, so 1 means no other string has c
            if (inWords[c - 'a'] == 1) { out += c; }
        }
    }
    return out;
}

// (1) core idea: exact min proportion -> tied strings -> drop shared chars
// T: O(total len), S: O(n)
string lowestProportion(const vector<string>& words) {
    return dropShared(words, minProportion(words));
}

// (2) core idea: after (1)'s filter, a pairwise pass for each candidate's min sim
// T: O(total len + k^2 * sim cost), S: O(k) — k = num of candidates
string lowestProportionBySim(const vector<string>& words) {
    vector<int> cand = minProportion(words);
    int k = cand.size();
    if (k == 1) { return dropShared(words, cand); }
    vector<double> score(k);
    for (int a = 0; a < k; a++) {
        score[a] = 2.0;
        for (int b = 0; b < k; b++) {
            if (a == b) { continue; }
            double s = sim(words[cand[a]], words[cand[b]]);
            if (s < score[a]) { score[a] = s; }
        }
    }
    double lowest = score[0];
    for (double s : score) {
        if (s < lowest) { lowest = s; }
    }
    vector<int> keep;
    for (int a = 0; a < k; a++) {
        // exact == is safe: both sides are the same values sim returned
        if (score[a] == lowest) { keep.push_back(cand[a]); }
    }
    return dropShared(words, keep);
}

void test(const string& label, const string& got, const string& want) {
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("p1-one-min", lowestProportion({"aabbc", "aaaa", "xyzxyz"}), "bbc");
    test("p1-all-tied", lowestProportion({"abc", "def", "ghi"}), "abcdefghi");
    test("p2-one-candidate", lowestProportionBySim({"abca", "bcde", "cdef"}), "aa");
    test("p2-min-score", lowestProportionBySim({"abx", "by", "aby"}), "x");
    test("p3-repeats", fabs(sim("aab", "ab") - 2.0 / 3) < 1e-9 ? "y" : "n", "y");
    test("p3-disjoint", sim("ab", "cd") == 0.0 ? "y" : "n", "y");
    return 0;
}

// follow-up:
// why not double for the proportion -> 1/3 vs 2/6 can differ in the last bit; cross
//   multiply (num * den fits long long: len <= 1e4)
// sim is expensive -> k^2 calls; cache sim(a, b) = sim(b, a) to halve them
// uppercase / any ASCII -> arrays of 128 instead of 26
// Jaccard on sets, repeats ignored -> shared += (a && b), total += (a || b) per letter
// Jaccard on words instead of chars -> split on spaces w/ a char loop, count map of words
