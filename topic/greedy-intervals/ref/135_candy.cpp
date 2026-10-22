// LC 135 Candy: children in a row each get >= 1 candy; a child with a higher rating
// than a neighbor gets more than that neighbor. Return the minimum candies.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::max;

// Two passes: left-to-right enforces the left neighbor rule, right-to-left the right
// one, combined with max. T: O(n), S: O(n).
int candy(const vector<int>& ratings) {
    int n = ratings.size();
    vector<int> candies(n, 1);   // candies[i] = candies given to child i
    for (int i = 1; i < n; i++) {
        if (ratings[i] > ratings[i - 1]) { candies[i] = candies[i - 1] + 1; }
    }
    for (int i = n - 2; i >= 0; i--) {
        if (ratings[i] > ratings[i + 1]) {
            candies[i] = max(candies[i], candies[i + 1] + 1);
        }
    }
    int total = 0;
    for (int c : candies) { total += c; }
    return total;
}

void test(const string& label, const vector<int>& ratings, int want) {
    cout << label << (candy(ratings) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("valley", {1, 0, 2}, 5);
    test("peak", {1, 2, 3, 1}, 7);
    test("equal", {2, 2}, 2);
    test("empty", {}, 0);
}
