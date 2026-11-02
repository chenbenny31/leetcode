// LC 768 Max Chunks To Make Sorted II: split arr into the most chunks so that sorting
// each chunk and joining them gives the sorted array (values may repeat).

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;

// Stack of chunk maxima (nondecreasing). A smaller value must join every chunk whose
// max is larger: pop them, merge into one chunk keeping the largest max.
// T: O(n), S: O(n).
int maxChunksToSorted(const vector<int>& arr) {
    // chunkMax for each current chunk, left to right
    vector<int> stk;
    for (int x : arr) {
        int merged = x;
        while (!stk.empty() && stk.back() > x) {
            merged = max(merged, stk.back());
            stk.pop_back();
        }
        stk.push_back(merged);
    }
    return stk.size();
}

void test(const string& label, const vector<int>& arr, int want) {
    cout << label << (maxChunksToSorted(arr) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("sorted", {1, 2, 3}, 3);
    test("reversed", {3, 2, 1}, 1);
    test("merge-two", {2, 1, 3, 4, 4}, 4);
    test("dup-pulls-back", {1, 1, 0}, 1);
}
