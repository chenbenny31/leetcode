// LC 721 Accounts Merge (Medium). Pattern: union-find over shared keys.
// accounts[i] = [name, emails...]. Two accounts belong to the same person if they share
// an email. Merge them into [name, sorted distinct emails...], in any order.

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

// returns the root account of node, compressing the path
int findRoot(vector<int>& parent, int node) {
    if (parent[node] != node) { parent[node] = findRoot(parent, parent[node]); }
    return parent[node];
}

// Union-find over account indices, joined through shared emails.
// T: O(E log E) for sorting, E = total emails; S: O(E).
vector<vector<string>> accountsMerge(const vector<vector<string>>& accounts) {
    int n = accounts.size();
    // parent[i] = parent account of i in the union-find
    vector<int> parent(n);
    for (int i = 0; i < n; i++) { parent[i] = i; }
    // email -> first account that listed it
    unordered_map<string, int> owner;
    for (int i = 0; i < n; i++) {
        for (int j = 1; j < (int)accounts[i].size(); j++) {
            const string& email = accounts[i][j];
            if (!owner.count(email)) {
                owner[email] = i;
                continue;
            }
            int rootCur = findRoot(parent, i);
            int rootOwner = findRoot(parent, owner[email]);
            parent[rootCur] = rootOwner;
        }
    }
    // root account -> its distinct emails
    unordered_map<int, vector<string>> groups;
    for (const auto& [email, id] : owner) {
        groups[findRoot(parent, id)].push_back(email);
    }
    vector<vector<string>> result;
    for (auto& [root, emails] : groups) {
        sort(emails.begin(), emails.end());
        vector<string> merged = {accounts[root][0]};
        for (const string& email : emails) { merged.push_back(email); }
        result.push_back(merged);
    }
    return result;
}

// LC 1101: sort logs [time, a, b] by time and union a, b; each union that joins two
// groups drops the count, and the time it hits 1 is the answer, -1 if never.
// T: O(L log L + n), S: O(n); L = number of logs.
int earliestAllFriends(vector<vector<int>> logs, int n) {
    sort(logs.begin(), logs.end());
    // parent[i] = parent person of i in the union-find
    vector<int> parent(n);
    for (int i = 0; i < n; i++) { parent[i] = i; }
    int components = n;
    for (const auto& entry : logs) {
        int rootA = findRoot(parent, entry[1]);
        int rootB = findRoot(parent, entry[2]);
        // already friends through others
        if (rootA == rootB) { continue; }
        parent[rootA] = rootB;
        components--;
        if (components == 1) { return entry[0]; }
    }
    return -1;
}

// LC 547: union i and j whenever isConnected[i][j] == 1; provinces = cities that are
// still their own root. T: O(n^2 log n), S: O(n).
int findCircleNum(const vector<vector<int>>& isConnected) {
    int n = isConnected.size();
    // parent[i] = parent city of i in the union-find
    vector<int> parent(n);
    for (int i = 0; i < n; i++) { parent[i] = i; }
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (isConnected[i][j] == 0) { continue; }
            parent[findRoot(parent, i)] = findRoot(parent, j);
        }
    }
    int provinces = 0;
    for (int i = 0; i < n; i++) {
        if (parent[i] == i) { provinces++; }
    }
    return provinces;
}

void test(const string& label, const vector<vector<string>>& accounts,
          vector<vector<string>> want) {
    vector<vector<string>> got = accountsMerge(accounts);
    sort(got.begin(), got.end());
    sort(want.begin(), want.end());
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

void testFriends(const string& label, const vector<vector<int>>& logs, int n, int want) {
    cout << label << (earliestAllFriends(logs, n) == want ? " PASS\n" : " FAIL\n");
}

void testCircle(const string& label, const vector<vector<int>>& isConnected, int want) {
    cout << label << (findCircleNum(isConnected) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("chain", {{"Ann", "a@x", "b@x"}, {"Ann", "c@x"}, {"Ann", "b@x", "c@x"}},
         {{"Ann", "a@x", "b@x", "c@x"}});
    test("same-name-apart", {{"Bob", "b1@x"}, {"Bob", "b2@x"}},
         {{"Bob", "b1@x"}, {"Bob", "b2@x"}});
    test("dup-email", {{"Cy", "c@x", "c@x"}}, {{"Cy", "c@x"}});
    testFriends("friends-unsorted", {{5, 0, 1}, {2, 1, 2}}, 3, 5);
    testFriends("friends-redundant", {{1, 0, 1}, {2, 1, 0}, {3, 1, 2}}, 3, 3);
    testFriends("friends-never", {{1, 0, 1}}, 3, -1);
    testCircle("circle-chain", {{1, 1, 0}, {1, 1, 1}, {0, 1, 1}}, 1);
    testCircle("circle-ends-linked", {{1, 0, 1}, {0, 1, 0}, {1, 0, 1}}, 2);
    testCircle("circle-isolated", {{1, 0}, {0, 1}}, 2);
}

// follow-up: DFS instead of union-find -> graph[email] = emails in the same account (link
//   the first email to the rest), DFS each component, sort; O(E log E)
// follow-up: union by size -> with path compression ~O(alpha(n)) per op
