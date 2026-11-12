// LC 721 Accounts Merge (Medium). Pattern: union-find over shared keys.
// accounts[i] = [name, emails...]. Two accounts belong to the same person if they share
// an email. Merge them into [name, sorted distinct emails...], in any order.
//
// Clarify:
// - same name but no shared email -> different people, kept apart
// - one email in two accounts -> always the same name (given), so the root's name works
// - an email listed twice in one account -> appears once in the output
// - every account has at least one email -> assume (a name-only account would vanish)
// - up to 1e3 accounts x 10 emails -> E <= 1e4 emails; sorting them dominates
// - output: accounts in any order, each one's emails sorted (plain string order)
// Idea: union-find over account indices. The first account that lists an email owns it;
//   a later account that lists it is united with the owner. Then every distinct email
//   goes to its owner's root, and each root's sorted emails form one merged account.
// Pitfalls:
// - union the roots (parent[rootCur] = rootOwner), not parent[i] = owner[email]: account
//   i may already hang under another root, and that link would be cut
// - collect emails from owner (one key per distinct email), not from the accounts, or a
//   repeated email shows up twice
// - sort the emails before putting the name in front (or sort from begin() + 1)
// - recursive findRoot is fine for 1e3 accounts; use the halving loop for ~1e5 nodes
// Tests to write: three accounts chained by shared emails; same name, nothing shared ->
//   two people; an email listed twice; two people interleaved in the input.

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

// adds cur and every email id reachable from it to component
void dfs(int cur, const vector<vector<int>>& graph, vector<bool>& seen,
         vector<int>& component) {
    seen[cur] = true;
    component.push_back(cur);
    for (int nxt : graph[cur]) {
        if (!seen[nxt]) { dfs(nxt, graph, seen, component); }
    }
}

// follow-up: DFS instead of union-find. Number the distinct emails, link each email of
// an account to that account's first email (a star, so O(E) edges, not all pairs), then
// one DFS per unseen email collects one person. T: O(E log E), S: O(E).
vector<vector<string>> accountsMergeDfs(const vector<vector<string>>& accounts) {
    // id[email] = index of email in emails
    unordered_map<string, int> id;
    // emails[i] = the email with id i; names[i] = name of the first account listing it
    vector<string> emails;
    vector<string> names;
    // graph[i] = ids of emails listed in one account together with email i
    vector<vector<int>> graph;
    for (const vector<string>& account : accounts) {
        for (int j = 1; j < (int)account.size(); j++) {
            const string& email = account[j];
            if (id.insert({email, (int)emails.size()}).second) {
                emails.push_back(email);
                names.push_back(account[0]);
                graph.push_back({});
            }
            if (j == 1) { continue; }
            int first = id[account[1]];
            int cur = id[email];
            graph[first].push_back(cur);
            graph[cur].push_back(first);
        }
    }
    vector<bool> seen(emails.size(), false);
    vector<vector<string>> result;
    for (int start = 0; start < (int)emails.size(); start++) {
        if (seen[start]) { continue; }
        // ids of one person's emails
        vector<int> component;
        dfs(start, graph, seen, component);
        vector<string> merged;
        for (int e : component) { merged.push_back(emails[e]); }
        sort(merged.begin(), merged.end());
        merged.insert(merged.begin(), names[start]);
        result.push_back(merged);
    }
    return result;
}

// LC 1101: sort logs [time, a, b] by time and union a, b; each union that joins two
// groups drops the count, and the time it hits 1 is the answer, -1 if never.
// Pitfall: the logs come unsorted; count only unions of two different roots.
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

// follow-up (Google "point clusters" ×2; statement assumed): points[i] = [x, y]; two
// points at Euclidean distance <= d are in one cluster, and so are chains of them.
// Count the clusters: union every close pair, each union of two roots drops the count.
// Pitfall: compare squared distances in long long: no sqrt, and dx * dx overflows int
// (|x|, |y| <= 1e9 keeps dx * dx + dy * dy <= 8e18, inside long long).
// T: O(p^2 log p), S: O(p); p = number of points (a grid of d-sized buckets avoids p^2).
int countClusters(const vector<vector<int>>& points, int d) {
    int p = points.size();
    // parent[i] = parent point of i in the union-find
    vector<int> parent(p);
    for (int i = 0; i < p; i++) { parent[i] = i; }
    long long limit = (long long)d * d;
    int clusters = p;
    for (int i = 0; i < p; i++) {
        for (int j = i + 1; j < p; j++) {
            long long dx = (long long)points[i][0] - points[j][0];
            long long dy = (long long)points[i][1] - points[j][1];
            if (dx * dx + dy * dy > limit) { continue; }
            int rootI = findRoot(parent, i);
            int rootJ = findRoot(parent, j);
            if (rootI == rootJ) { continue; }
            parent[rootI] = rootJ;
            clusters--;
        }
    }
    return clusters;
}

void test(const string& label, const vector<vector<string>>& accounts,
          vector<vector<string>> want) {
    vector<vector<string>> got = accountsMerge(accounts);
    vector<vector<string>> gotDfs = accountsMergeDfs(accounts);
    sort(got.begin(), got.end());
    sort(gotDfs.begin(), gotDfs.end());
    sort(want.begin(), want.end());
    cout << label << (got == want && gotDfs == want ? " PASS\n" : " FAIL\n");
}

void testFriends(const string& label, const vector<vector<int>>& logs, int n, int want) {
    cout << label << (earliestAllFriends(logs, n) == want ? " PASS\n" : " FAIL\n");
}

void testCircle(const string& label, const vector<vector<int>>& isConnected, int want) {
    cout << label << (findCircleNum(isConnected) == want ? " PASS\n" : " FAIL\n");
}

void testClusters(const string& label, const vector<vector<int>>& points, int d,
                  int want) {
    cout << label << (countClusters(points, d) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("chain", {{"Ann", "a@x", "b@x"}, {"Ann", "c@x"}, {"Ann", "b@x", "c@x"}},
         {{"Ann", "a@x", "b@x", "c@x"}});
    test("same-name-apart", {{"Bob", "b1@x"}, {"Bob", "b2@x"}},
         {{"Bob", "b1@x"}, {"Bob", "b2@x"}});
    test("dup-email", {{"Cy", "c@x", "c@x"}}, {{"Cy", "c@x"}});
    test("two-people", {{"Ann", "a@x"}, {"Bob", "b@x"}, {"Ann", "c@x", "a@x"}},
         {{"Ann", "a@x", "c@x"}, {"Bob", "b@x"}});
    testFriends("friends-unsorted", {{5, 0, 1}, {2, 1, 2}}, 3, 5);
    testFriends("friends-redundant", {{1, 0, 1}, {2, 1, 0}, {3, 1, 2}}, 3, 3);
    testFriends("friends-never", {{1, 0, 1}}, 3, -1);
    testCircle("circle-chain", {{1, 1, 0}, {1, 1, 1}, {0, 1, 1}}, 1);
    testCircle("circle-ends-linked", {{1, 0, 1}, {0, 1, 0}, {1, 0, 1}}, 2);
    testCircle("circle-isolated", {{1, 0}, {0, 1}}, 2);
    testClusters("clusters-chain", {{0, 0}, {2, 0}, {4, 0}, {9, 9}}, 2, 2);
    testClusters("clusters-exact-d", {{0, 0}, {3, 4}}, 5, 1);
    testClusters("clusters-just-over", {{0, 0}, {3, 4}}, 4, 2);
    testClusters("clusters-square-overflow", {{0, 0}, {65536, 0}}, 1, 2);
}

// follow-up: DFS instead of union-find -> accountsMergeDfs above
// follow-up: union by size -> with path compression ~O(alpha(n)) per op:
//   vector<int> size(n, 1);
//   if (size[rootCur] < size[rootOwner]) { swap(rootCur, rootOwner); }
//   parent[rootOwner] = rootCur;
//   size[rootCur] += size[rootOwner];
// follow-up: point clusters (Google ×2) -> countClusters above
// follow-up: dynamic friendship (Google ×2) -> adds only: union-find answers online;
//   removals only, known up front: replay in reverse so each removal becomes a union;
//   both online: rollback union-find with offline divide and conquer (mention only)
