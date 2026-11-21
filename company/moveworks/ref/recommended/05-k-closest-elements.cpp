// 05 · Find K Closest Elements (phone screen, Medium) — LC 658, binary search on start
// sorted arr; return the k values closest to x, sorted; a tie goes to the smaller value
//
// clarify:
// arr sorted ascending, may have duplicates -> yes
// 1 <= k <= n -> yes
// x may be outside the range of arr -> yes
// output order -> ascending

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// core idea: the answer is a window arr[lo..lo+k); binary search its start: if
// arr[mid + k] is strictly closer than arr[mid], the window must start after mid
// T: O(log(n - k) + k), S: O(1) besides the output
vector<int> findClosestElements(const vector<int>& arr, int k, int x) {
    int n = arr.size();
    int lo = 0;
    // last possible start
    int hi = n - k;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        // compare signed distances, no abs: a tie keeps the left (smaller) value
        if (x - arr[mid] > arr[mid + k] - x) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return vector<int>(arr.begin() + lo, arr.begin() + lo + k);
}

void test(const string& label, const vector<int>& arr, int k, int x,
          const vector<int>& want) {
    cout << label << (findClosestElements(arr, k, x) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("middle", {1, 2, 3, 4, 5}, 4, 3, {1, 2, 3, 4});
    test("left-of-all", {1, 2, 3, 4, 5}, 4, -1, {1, 2, 3, 4});
    test("right-of-all", {1, 2, 3, 4, 5}, 2, 9, {4, 5});
    test("duplicates", {1, 1, 2, 2, 2, 2, 2, 3, 3}, 3, 3, {2, 3, 3});
    return 0;
}

// follow-up:
// why not abs -> arr[mid] == arr[mid + k] (duplicates) is an abs tie and keeps the left
//   window even when x is right of both; the signed compare moves right correctly
// simpler O(n) answer -> two pointers from both ends, drop the farther end until k left
// unsorted input -> size-k max-heap on (distance, value), select/10-heap/05
// lower_bound version -> find insertion point, expand left / right k times
