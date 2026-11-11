// LC 767 Reorganize String (Medium). Pattern: max-heap greedy by remaining count.
// Rearrange s (lowercase) so no two adjacent characters are equal; return any such
// string, or "" if impossible.

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <utility>

using std::cout;
using std::string;
using std::vector;
using std::priority_queue;
using std::pair;

// Always place the most frequent remaining char that differs from the last one: pop the
// top two, write both, push back whatever is left. T: O(n log 26), S: O(26).
string reorganizeString(const string& s) {
    // count[letter] = copies of that letter in s
    vector<int> count(26, 0);
    for (char c : s) { count[c - 'a']++; }
    // (remaining count, letter), most remaining on top
    priority_queue<pair<int, char>> heap;
    for (int i = 0; i < 26; i++) {
        if (count[i] > 0) { heap.push({count[i], (char)('a' + i)}); }
    }

    string result;
    while (heap.size() >= 2) {
        auto [countA, letterA] = heap.top();
        heap.pop();
        auto [countB, letterB] = heap.top();
        heap.pop();
        // last round's letterB never outranks last round's letterA once both lose one,
        // so this round's letterA differs from the last char written
        result += letterA;
        result += letterB;
        if (countA > 1) { heap.push({countA - 1, letterA}); }
        if (countB > 1) { heap.push({countB - 1, letterB}); }
    }
    if (!heap.empty()) {
        // one letter left: only one copy may remain, or it would sit next to itself
        if (heap.top().first > 1) { return ""; }
        result += heap.top().second;
    }
    return result;
}

// valid = same letters as s and no equal neighbors
bool valid(const string& s, const string& got) {
    if (got.size() != s.size()) { return false; }
    vector<int> count(26, 0);
    for (char c : s) { count[c - 'a']++; }
    for (char c : got) { count[c - 'a']--; }
    for (int diff : count) {
        if (diff != 0) { return false; }
    }
    for (int i = 1; i < (int)got.size(); i++) {
        if (got[i] == got[i - 1]) { return false; }
    }
    return true;
}

void test(const string& label, const string& s, bool possible) {
    string got = reorganizeString(s);
    bool ok = possible ? valid(s, got) : got.empty();
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("two-letters", "aab", true);
    test("impossible", "aaab", false);
    test("many-letters", "aaabbc", true);
    test("single", "z", true);
}

// follow-up: why pop two at a time -> the second pick guarantees the next char differs
//   from the one just written without remembering it
// follow-up: quick feasibility -> possible iff maxCount <= (n + 1) / 2
// follow-up: same letters at least k apart (LC 358) -> heap + a cooldown queue of k
