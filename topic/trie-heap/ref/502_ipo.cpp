// LC 502 IPO: start with capital w and finish at most k projects. Project i needs
// capital[i] to start and adds profits[i] to your capital. Return the maximum capital.
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::priority_queue;
using std::pair;
using std::sort;

// Sort projects by required capital; each round move every newly affordable project into
// a max-heap of profits and take the best. T: O((n + k) log n), S: O(n).
int findMaximizedCapital(int k, int w, vector<int>& profits, vector<int>& capital) {
    int n = profits.size();
    vector<pair<int, int>> projects;   // (capital needed, profit), ascending by capital
    for (int i = 0; i < n; i++) {
        projects.push_back({capital[i], profits[i]});
    }
    sort(projects.begin(), projects.end());
    priority_queue<int> affordable;    // profits of projects we can afford now
    int next = 0;
    for (int round = 0; round < k; round++) {
        while (next < n && projects[next].first <= w) {
            affordable.push(projects[next].second);
            next++;
        }
        if (affordable.empty()) { break; }
        w += affordable.top();
        affordable.pop();
    }
    return w;
}

void test(const string& label, int k, int w, vector<int> profits, vector<int> capital,
          int want) {
    cout << label << (findMaximizedCapital(k, w, profits, capital) == want
                          ? " PASS\n" : " FAIL\n");
}

int main() {
    test("unlock-chain", 2, 0, {1, 5, 3}, {0, 1, 2}, 6);
    test("k-limits", 1, 0, {2, 9}, {0, 0}, 9);
    test("unaffordable", 3, 1, {4, 4}, {5, 6}, 1);
    test("zero-k", 0, 3, {4}, {0}, 3);
}
