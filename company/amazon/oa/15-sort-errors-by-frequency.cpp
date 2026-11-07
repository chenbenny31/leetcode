// Sort error codes so that less frequent codes come first (all copies together); equal
// frequencies go by smaller code first. (LC 1636 breaks ties the other way.)

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::sort;

// count, then sort by (frequency asc, value asc). T: O(n log n), S: O(n).
vector<int> sortErrors(vector<int> errors) {
    // freq[x] = occurrences of code x
    unordered_map<int, int> freq;
    for (int x : errors) { freq[x]++; }
    sort(errors.begin(), errors.end(), [&](int a, int b) {
        if (freq[a] != freq[b]) { return freq[a] < freq[b]; }
        return a < b;
    });
    return errors;
}

void test(const string& label, const vector<int>& errors, const vector<int>& want) {
    cout << label << (sortErrors(errors) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("basic", {5, 2, 5, 8, 5, 2}, {8, 2, 2, 5, 5, 5});
    test("tie-value", {9, 4, 9, 4}, {4, 4, 9, 9});
    test("same", {1, 1}, {1, 1});
}
