// LC 904 Fruit Into Baskets: fruits[i] is the fruit type of tree i. Pick a contiguous
// run of trees with at most 2 distinct types (two baskets). Return the max run length.
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::max;

// At-most-2-distinct sliding window; shrink from the left while 3 types are inside.
// T: O(n), S: O(1).
int totalFruit(const vector<int>& fruits) {
    int n = fruits.size();
    unordered_map<int, int> cnt;   // fruit type -> count inside the window
    int best = 0;
    int left = 0;
    for (int right = 0; right < n; right++) {
        cnt[fruits[right]]++;
        while (cnt.size() > 2) {
            cnt[fruits[left]]--;
            // a type with count 0 must leave the map, or size() overcounts
            if (cnt[fruits[left]] == 0) { cnt.erase(fruits[left]); }
            left++;
        }
        best = max(best, right - left + 1);
    }
    return best;
}

void test(const string& label, const vector<int>& fruits, int want) {
    if (totalFruit(fruits) == want) {
        cout << label << " PASS\n";
    } else {
        cout << label << " FAIL\n";
    }
}

int main() {
    test("two-types", {1, 1, 2, 2}, 4);
    test("third-type-restarts", {1, 2, 2, 3, 3, 3}, 5);
    test("single", {7}, 1);
    test("empty", {}, 0);
}
