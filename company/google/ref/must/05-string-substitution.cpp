// Replace every %KEY% in text with dict[KEY] (keys are always wrapped in %...%), e.g.
// {"A": "abc", "B": "def"} and "%A%_%B%" -> "abc_def". Unknown keys stay as written.
// Follow-up: values may themselves contain %KEY% references; expand them recursively and
// report a cycle (A -> "%B%", B -> "%A%").
//
// Clarify:
// - keys are always wrapped in %...% -> stated; a lone or unclosed '%' -> assume none
//   (follow-up below)
// - unknown key -> keep "%KEY%" as written (stated); confirm vs empty string / error
// - keys case-sensitive; empty key "%%"? -> assume none (follow-up: escape)
// - follow-up: on a cycle, what to return? -> here a flag; the text is then ignored
// Idea: one left-to-right scan into a new string: copy plain chars; at '%' find the
//   closing '%', look the key up, append its value (or "%KEY%"). Follow-up: the same
//   scan, but a value is expanded before it is appended (DFS over keys). done[key]
//   memoizes a fully expanded value; onPath holds the keys on the current recursion
//   stack, and meeting one of them again is a cycle (3 states, as in course schedule).
// Pitfalls:
// - build a new result; replacing inside text in place shifts positions and rescans
//   inserted values
// - substr(pos + 1, close - pos - 1): the second argument is a length, not an end index
// - find returns size_t npos; stored in an int it becomes -1 and pos = close + 1 = 0
//   loops forever on an unclosed '%'
// - follow-up: erase the key from onPath after its recursion, or a key used twice
//   (A -> "%B%%B%") looks like a cycle
// - follow-up: memoize in done, or chains like A -> "%B%%B%", B -> "%C%%C%" expand
//   exponentially
// Tests to write: two keys; adjacent "%A%%B%"; no keys; unknown key kept; nested value;
//   one key used twice (not a cycle); 2-cycle.

#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>

using std::cout;
using std::string;
using std::unordered_map;
using std::unordered_set;

// copy characters; at '%' read up to the closing '%' and append the value.
// T: O(text + output), S: O(output).
string substitute(const string& text, const unordered_map<string, string>& dict) {
    string result;
    int n = text.size();
    int pos = 0;
    while (pos < n) {
        if (text[pos] != '%') {
            result += text[pos];
            pos++;
            continue;
        }
        int close = text.find('%', pos + 1);
        string key = text.substr(pos + 1, close - pos - 1);
        auto it = dict.find(key);
        if (it == dict.end()) {
            result += "%" + key + "%";
        } else {
            result += it->second;
        }
        pos = close + 1;
    }
    return result;
}

// follow-up: same scan, but a key's value is expanded first (DFS). done[key] = fully
// expanded value; onPath = keys being expanded right now, meeting one again is a cycle.
// T: O(dict size + output), S: O(dict size + output).
string expand(const string& text, const unordered_map<string, string>& dict,
              unordered_map<string, string>& done, unordered_set<string>& onPath,
              bool& hasCycle) {
    string result;
    int n = text.size();
    int pos = 0;
    while (pos < n) {
        if (text[pos] != '%') {
            result += text[pos];
            pos++;
            continue;
        }
        int close = text.find('%', pos + 1);
        string key = text.substr(pos + 1, close - pos - 1);
        pos = close + 1;

        auto it = dict.find(key);
        if (it == dict.end()) {
            result += "%" + key + "%";
        } else if (done.count(key)) {
            result += done[key];
        } else if (onPath.count(key)) {
            hasCycle = true;
        } else {
            onPath.insert(key);
            string value = expand(it->second, dict, done, onPath, hasCycle);
            onPath.erase(key);
            done[key] = value;
            result += value;
        }
    }
    return result;
}

void test(const string& label, const string& text,
          const unordered_map<string, string>& dict, const string& want) {
    cout << label << (substitute(text, dict) == want ? " PASS\n" : " FAIL\n");
}

void testNested(const string& label, const string& text,
                const unordered_map<string, string>& dict, const string& want,
                bool wantCycle) {
    unordered_map<string, string> done;
    unordered_set<string> onPath;
    bool hasCycle = false;
    string got = expand(text, dict, done, onPath, hasCycle);
    bool ok = hasCycle == wantCycle && (wantCycle || got == want);
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    unordered_map<string, string> dict = {{"A", "x"}, {"B", "yz"}};
    test("two-keys", "%A%_%B%", dict, "x_yz");
    test("adjacent", "%A%%B%", dict, "xyz");
    test("no-keys", "plain", dict, "plain");
    test("unknown", "%C%!", dict, "%C%!");

    testNested("nested", "%A%", {{"A", "%B%-"}, {"B", "b"}}, "b-", false);
    testNested("shared", "%A%%B%", {{"A", "%B%"}, {"B", "b"}}, "bb", false);
    testNested("cycle", "%A%", {{"A", "%B%"}, {"B", "%A%"}}, "", true);
}

// follow-up: a literal '%' in text -> agree on an escape, e.g. "%%" means '%':
//   if (close == pos + 1) { result += '%'; pos += 2; continue; }
// follow-up: unclosed '%' -> keep find's result as size_t and copy the rest as-is:
//   size_t close = text.find('%', pos + 1);
//   if (close == string::npos) { result += text.substr(pos); break; }
// follow-up: very deep nesting -> the recursion depth is the longest reference chain;
//   iterative DFS with an explicit stack if keys can chain thousands deep
