// LC 763 Partition Labels (Medium). Pattern: last occurrence + greedy cut.
// Split lowercase s into as many parts as possible so each letter appears in at most
// one part; return the part sizes in order.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;

// A part can close only once every letter in it has had its last occurrence.
// T: O(n), S: O(26).
vector<int> partitionLabels(const string& s) {
    int n = s.size();
    // last[c] = last index of letter c in s
    vector<int> last(26, 0);
    for (int i = 0; i < n; i++) { last[s[i] - 'a'] = i; }
    vector<int> sizes;
    // first index of the current part
    int start = 0;
    // farthest last occurrence among letters in the current part
    int end = 0;
    for (int i = 0; i < n; i++) {
        end = max(end, last[s[i] - 'a']);
        if (i == end) {
            sizes.push_back(end - start + 1);
            start = i + 1;
        }
    }
    return sizes;
}

void test(const string& label, const string& s, const vector<int>& want) {
    cout << label << (partitionLabels(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("two-parts", "ababcc", {4, 2});
    test("all-distinct", "abcd", {1, 1, 1, 1});
    test("one-part", "abca", {4});
    test("empty", "", {});
}

// follow-up: return the parts themselves -> push s.substr(start, end - start + 1)
// follow-up: same idea as merging [first, last] intervals of each letter (LC 56)
