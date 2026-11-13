// 05 · Accounts Merge (Union-Find) — LC 547, 721, 1101, 1319
// Mother problem: select/08-graphs/07-accounts-merge.cpp
// Google reports: provinces, earliest all friends, people with secret (36), point
// clusters ×2, dynamic friendship ×2, org hierarchy, min cable moves
//
// LC 721: accounts[i] = [name, emails...]; accounts sharing an email are one person.
// Return [name, sorted distinct emails...] per person, in any order.
// Clarify
// - same name, no shared email → different people; a shared email → same name
// - an email listed twice → once; every account has at least one email
// Core idea
// - Union-find over account indices; the first account listing an email owns it, a later
//   one is united with the owner, root to root (parent[i] = owner cuts i's old link).
// - Collect each distinct email (keys of owner) under its root, sort, name in front.
// - T O(E log E) for sorting, S O(E); E = total emails.
// Tests: chain via shared emails → one; same name apart → two; dup email; interleaved.
//
// Follow-ups
// - DFS instead → star edges from each account's first email, one DFS per person
// - provinces (LC 547) → union i, j when isConnected[i][j]; count cities that are roots
// - earliest all friends (LC 1101) → sort logs by time, union; answer when components = 1
// - point clusters within d → union every close pair; squared distances in long long
// - min cable moves (LC 1319) → -1 if fewer than n - 1 cables, else components - 1
// - union by size → hang the smaller root under the larger one, add the sizes
// - dynamic friendship → adds only: online union-find; removals known up front: replay
//   in reverse so each removal becomes a union
//
//////////////////////////////////////////////////////////////////////////////////////////

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
