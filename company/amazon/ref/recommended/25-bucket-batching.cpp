// Partition K documents (by length) into exactly G batches. A batch costs
// (number of documents) * (longest document in it). Return the min total cost.

#include <iostream>
#include <vector>
#include <string>
#include <climits>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::sort;
using std::min;

// After sorting, some optimal answer uses contiguous slices, and a slice's max is its
// last element. dp[g][i] = min cost of the first i documents in g batches:
// dp[g][i] = min over j of dp[g-1][j] + (i - j) * len[i-1]. T: O(G * K^2), S: O(G * K).
long long minPadding(vector<int> lengths, int G) {
    sort(lengths.begin(), lengths.end());
    int K = lengths.size();
    const long long INF = LLONG_MAX / 4;
    vector<vector<long long>> dp(G + 1, vector<long long>(K + 1, INF));
    dp[0][0] = 0;
    for (int g = 1; g <= G; g++) {
        for (int i = g; i <= K; i++) {
            for (int j = g - 1; j < i; j++) {
                if (dp[g - 1][j] == INF) { continue; }
                long long batch = (long long)(i - j) * lengths[i - 1];
                dp[g][i] = min(dp[g][i], dp[g - 1][j] + batch);
            }
        }
    }
    return dp[G][K];
}

void test(const string& label, const vector<int>& lengths, int G, long long want) {
    cout << label << (minPadding(lengths, G) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("two-groups", {10, 1, 9, 2}, 2, 24);
    test("one-batch", {1, 2, 9, 10}, 1, 40);
    test("each-alone", {1, 2, 9, 10}, 4, 22);
    test("single", {5}, 1, 5);
}
