// LC 1345 Jump Game IV: from index i you can jump to i + 1, i - 1, or any j != i with
// arr[j] == arr[i]. Return the minimum jumps from index 0 to the last index.
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::unordered_map;

// BFS where an edge is a jump. Each value's group of indices is expanded once and then
// cleared, otherwise long runs of equal values make it quadratic. T: O(n), S: O(n).
int minJumps(vector<int>& arr) {
    int n = arr.size();
    unordered_map<int, vector<int>> same;   // value -> indices holding it
    for (int i = 0; i < n; i++) { same[arr[i]].push_back(i); }
    vector<int> dist(n, -1);                // dist[i] = fewest jumps to reach i
    queue<int> q;
    dist[0] = 0;
    q.push(0);
    while (!q.empty()) {
        int cur = q.front();
        q.pop();
        if (cur == n - 1) { return dist[cur]; }
        vector<int> jumps = same[arr[cur]];
        same[arr[cur]].clear();
        jumps.push_back(cur - 1);
        jumps.push_back(cur + 1);
        for (int nxt : jumps) {
            if (nxt < 0 || nxt >= n || dist[nxt] != -1) { continue; }
            dist[nxt] = dist[cur] + 1;
            q.push(nxt);
        }
    }
    return -1;
}

void test(const string& label, vector<int> arr, int want) {
    cout << label << (minJumps(arr) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("single", {7}, 0);
    test("value-shortcut", {5, 1, 2, 3, 5}, 1);
    test("walk", {1, 2, 3, 4}, 3);
    test("equal-run", {7, 7, 7, 7, 7, 1, 7}, 1);
}
