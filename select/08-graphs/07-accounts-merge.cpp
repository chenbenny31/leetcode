// LC 721 Accounts Merge (Medium) — union-find over accounts sharing an email
// accounts[i] = [name, emails...]; two accounts sharing an email = one person
// return each person as [name, sorted distinct emails...], any order
//
// clarify:
// same name, no shared email -> stay apart
// email twice in one account -> dedup via owner map
// every account has an email -> assume yes
// output order -> people any order, emails sorted
// sizes -> ~1e3 accounts x 10 emails

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <utility>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::sort;
using std::swap;

// root of x's group, w/ path halving
int find(vector<int>& parent, int x) {
    while (parent[x] != x) {
        parent[x] = parent[parent[x]];
        x = parent[x];
    }
    return x;
}

// joins the groups of a and b, smaller under larger; false if already one group
bool unite(vector<int>& parent, vector<int>& sz, int a, int b) {
    int rootA = find(parent, a);
    int rootB = find(parent, b);
    if (rootA == rootB) { return false; }
    if (sz[rootA] < sz[rootB]) { swap(rootA, rootB); }
    parent[rootB] = rootA;
    sz[rootA] += sz[rootB];
    return true;
}

// core idea: first account listing an email owns it; later account -> union w/ owner
// then each distinct email goes to its owner's root
// T: O(E log E), S: O(E) — E = total emails; sort dominates
vector<vector<string>> accountsMerge(const vector<vector<string>>& accounts) {
    int n = accounts.size();
    // parent[i] = parent account of i
    vector<int> parent(n);
    for (int i = 0; i < n; i++) { parent[i] = i; }
    // sz[root] = num of accounts in that root's group
    vector<int> sz(n, 1);
    // email -> first account that listed it
    unordered_map<string, int> owner;
    for (int i = 0; i < n; i++) {
        for (int j = 1; j < (int)accounts[i].size(); j++) {
            const string& email = accounts[i][j];
            if (!owner.count(email)) {
                owner[email] = i;
                continue;
            }
            // link the roots: account i may already hang under another root
            unite(parent, sz, i, owner[email]);
        }
    }
    // root account -> its distinct emails
    unordered_map<int, vector<string>> groups;
    for (const auto& [email, id] : owner) {
        groups[find(parent, id)].push_back(email);
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

void test(const string& label, const vector<vector<string>>& accounts,
          vector<vector<string>> want) {
    vector<vector<string>> got = accountsMerge(accounts);
    sort(got.begin(), got.end());
    sort(want.begin(), want.end());
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("chain", {{"Ann", "a@x", "b@x"}, {"Ann", "c@x"}, {"Ann", "b@x", "c@x"}},
         {{"Ann", "a@x", "b@x", "c@x"}});
    test("same-name-apart", {{"Bob", "b1@x"}, {"Bob", "b2@x"}},
         {{"Bob", "b1@x"}, {"Bob", "b2@x"}});
    test("dup-email", {{"Cy", "c@x", "c@x"}}, {{"Cy", "c@x"}});
    test("two-people", {{"Ann", "a@x"}, {"Bob", "b@x"}, {"Ann", "c@x", "a@x"}},
         {{"Ann", "a@x", "c@x"}, {"Bob", "b@x"}});
    return 0;
}

// follow-up:
// DFS instead of union-find -> link each email to account's first email, DFS per email
// why sz -> halving alone: O(log n) amortized, one find can walk a chain; w/ sz ~O(1)
// earliest moment all friends (LC 1101) -> sort logs by time, union; answer when cnt = 1
// + every pair within k hops (Google) -> binary search on log prefix; BFS from each node
// point clusters within dist d (Google) -> union pairs w/ dist^2 <= d * d; grid buckets
// make network connected (LC 1319) -> < n - 1 cables = -1, else components - 1
// dynamic friendship (Google) -> adds: online union-find; removals: replay in reverse
// redundant connection (LC 684) -> union edges in order; first edge inside one root
// graph valid tree (LC 261) -> exactly n - 1 edges and no edge inside one component
