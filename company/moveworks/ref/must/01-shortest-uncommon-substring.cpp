// 01 · Shortest Uncommon Substring (Moveworks phone screen, Medium) — LC 3076
// (1) LC 3076: for each string, return its shortest substring that occurs in no other
//     string; "" if it has none
// (2) 2026 phone screen: wrap that substring in <u></u> inside the original string;
//     compare case-insensitively; a tie goes to the earliest start
//
// clarify:
// several shortest ones -> (1) lexicographically smallest, (2) earliest start
// (2) no unique substring -> return the string unchanged (assumption)
// two equal strings -> each substring occurs in the other, so both get ""
// sizes -> n <= 100 strings, len <= 20, so all O(L^2) substrings fit
// "occurs" -> as a contiguous substring

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <cctype>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::unordered_set;
using std::tolower;

// substring -> num of strings that contain it
// T: O(n * L^3), S: O(n * L^3)
unordered_map<string, int> countOwners(const vector<string>& arr) {
    unordered_map<string, int> owners;
    for (const string& s : arr) {
        int len = s.size();
        // distinct substrings of s, so s counts once per substring
        unordered_set<string> seen;
        for (int i = 0; i < len; i++) {
            for (int j = i + 1; j <= len; j++) { seen.insert(s.substr(i, j - i)); }
        }
        for (const string& sub : seen) { owners[sub]++; }
    }
    return owners;
}

// (1) core idea: count how many strings contain each substring; the answer for s is the
// shortest substring of s w/ count 1 (s itself is that one string)
// T: O(n * L^3) — n * L^2 substrings, each up to L chars to build and hash
// S: O(n * L^3)
vector<string> shortestSubstrings(const vector<string>& arr) {
    unordered_map<string, int> owners = countOwners(arr);
    vector<string> out;
    for (const string& s : arr) {
        int len = s.size();
        string best;
        // shortest first: stop at the first size that has an answer
        for (int size = 1; size <= len && best.empty(); size++) {
            for (int i = 0; i + size <= len; i++) {
                string sub = s.substr(i, size);
                if (owners[sub] == 1 && (best.empty() || sub < best)) { best = sub; }
            }
        }
        out.push_back(best);
    }
    return out;
}

// (2) core idea: same counts on lowercase copies; scan sizes up, starts left to right,
// the first hit is the shortest w/ the earliest start
// T: O(n * L^3), S: O(n * L^3)
vector<string> underlineUnique(const vector<string>& arr) {
    vector<string> lower;
    for (const string& s : arr) {
        string low = s;
        for (char& c : low) { c = tolower((unsigned char)c); }
        lower.push_back(low);
    }
    unordered_map<string, int> owners = countOwners(lower);

    vector<string> out;
    int n = arr.size();
    for (int k = 0; k < n; k++) {
        int len = lower[k].size();
        // start and size of the answer, -1 = none yet
        int start = -1;
        int size = 0;
        for (int sz = 1; sz <= len && start == -1; sz++) {
            for (int i = 0; i + sz <= len; i++) {
                if (owners[lower[k].substr(i, sz)] == 1) {
                    start = i;
                    size = sz;
                    break;
                }
            }
        }
        if (start == -1) {
            out.push_back(arr[k]);
            continue;
        }
        // cut the original string, so its case is kept
        const string& s = arr[k];
        out.push_back(s.substr(0, start) + "<u>" + s.substr(start, size) + "</u>"
                      + s.substr(start + size));
    }
    return out;
}

void test(const string& label, const vector<string>& got, const vector<string>& want) {
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("p1-mixed", shortestSubstrings({"cab", "ad", "bad", "c"}), {"ab", "", "ba", ""});
    test("p1-contained", shortestSubstrings({"abc", "bcd", "abcd"}), {"", "", "abcd"});
    test("p1-duplicates", shortestSubstrings({"ab", "ab"}), {"", ""});
    test("p2-earliest-and-case", underlineUnique({"Cab", "ad", "bad", "c"}),
         {"<u>Ca</u>b", "ad", "<u>ba</u>d", "c"});
    test("p2-case-insensitive", underlineUnique({"AB", "ab", "abc"}),
         {"AB", "ab", "ab<u>c</u>"});
    return 0;
}

// follow-up:
// longer strings -> store substrings as (string id, start, len) + rolling hash, or a
//   generalized suffix automaton; hashing L^2 strings of len L is the bottleneck
// only answer for one query string -> same count map, built once, reused per query
// shortest substring of s that is NOT a substring of t (two strings) -> same idea, n = 2
