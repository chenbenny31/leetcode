// LC 547 Number of Provinces: isConnected[i][j] == 1 means cities i and j are directly
// connected (symmetric). A province is a group of directly or indirectly connected
// cities. Return the number of provinces.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

void dfs(const vector<vector<int>>& isConnected, vector<bool>& seen, int cur) {
    seen[cur] = true;
    int n = isConnected.size();
    for (int nxt = 0; nxt < n; nxt++) {
        if (isConnected[cur][nxt] == 1 && !seen[nxt]) { dfs(isConnected, seen, nxt); }
    }
}

// Each unvisited city starts a new province; DFS marks its whole group.
// T: O(V^2) for the matrix, S: O(V).
int findCircleNum(const vector<vector<int>>& isConnected) {
    int n = isConnected.size();
    vector<bool> seen(n, false);   // seen[c] = c already counted in a province
    int provinces = 0;
    for (int c = 0; c < n; c++) {
        if (seen[c]) { continue; }
        provinces++;
        dfs(isConnected, seen, c);
    }
    return provinces;
}

void test(const string& label, const vector<vector<int>>& isConnected, int want) {
    cout << label << (findCircleNum(isConnected) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("isolated", {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}}, 3);
    test("indirect", {{1, 1, 0}, {1, 1, 1}, {0, 1, 1}}, 1);
    test("pair-and-loner", {{1, 1, 0}, {1, 1, 0}, {0, 0, 1}}, 2);
}
