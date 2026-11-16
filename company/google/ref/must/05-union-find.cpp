// 05 · Accounts Merge (Union-Find) — LC 547, 721, 1101, 1319
// Mother: select/08-graphs/07-accounts-merge.cpp
// LC 721: accounts[i] = [name, emails...]; accounts sharing an email = one person
// return [name, sorted distinct emails...] per person, any order
//
// clarify:
// same name w/o shared email -> different people
// email listed twice -> dedup
// account w/o email -> assume >= 1 email each
// output order -> any

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

// core idea: union-find over account idx; first account listing an email owns it
// later account w/ that email -> union w/ owner, root to root; then group emails by root
// T: O(E log E), S: O(E) — E = total emails; log from sorting
vector<vector<string>> accountsMerge(const vector<vector<string>>& accounts) {
    int n = accounts.size();
    // parent[i] = union-find parent of account i
    vector<int> parent(n);
    for (int i = 0; i < n; i++) { parent[i] = i; }
    // sz[root] = num of accounts in that root's group
    vector<int> sz(n, 1);
    // owner[email] = first account that listed it
    unordered_map<string, int> owner;
    for (int i = 0; i < n; i++) {
        for (int j = 1; j < (int)accounts[i].size(); j++) {
            const string& email = accounts[i][j];
            if (!owner.count(email)) {
                owner[email] = i;
                continue;
            }
            unite(parent, sz, i, owner[email]);
        }
    }
    // groups[root account] = its distinct emails
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
}

// follow-up:
// DFS instead -> star edges from each account's first email, one DFS per person
// provinces (LC 547) -> union i, j when isConnected[i][j]; cnt cities that are roots
// earliest all friends (LC 1101), friend graph w/ timeline -> 06
// point clusters within d -> union every close pair; squared dist in long long
// min cable moves (LC 1319) -> -1 if < n - 1 cables, else components - 1
// why sz -> halving alone: O(log n) amortized, one find can walk a chain; w/ sz ~O(1)
// dynamic friendship w/ removals known up front -> replay in reverse, removal = union
// redundant connection (LC 684) -> union edges in order; first edge inside one root
// org hierarchy (manager / peer) -> union-find of peer classes, 1 manager ptr per class
