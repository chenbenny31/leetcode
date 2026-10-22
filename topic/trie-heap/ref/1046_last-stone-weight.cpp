// LC 1046 Last Stone Weight: repeatedly smash the two heaviest stones (x <= y): both go
// if equal, else y - x remains. Return the last stone's weight, or 0 if none is left.
#include <iostream>
#include <vector>
#include <string>
#include <queue>
using std::cout;
using std::string;
using std::vector;
using std::priority_queue;

// Max-heap gives the two heaviest each round. T: O(n log n), S: O(n).
int lastStoneWeight(vector<int>& stones) {
    priority_queue<int> heap(stones.begin(), stones.end());   // remaining stones
    while (heap.size() > 1) {
        int y = heap.top();
        heap.pop();
        int x = heap.top();
        heap.pop();
        if (y != x) { heap.push(y - x); }
    }
    return heap.empty() ? 0 : heap.top();
}

void test(const string& label, vector<int> stones, int want) {
    cout << label << (lastStoneWeight(stones) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("one-left", {5, 3, 1}, 1);
    test("all-cancel", {4, 4, 2, 2}, 0);
    test("single", {7}, 7);
    test("remainder", {9, 2, 1}, 6);
}
