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

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::sort;

// root account of node, w/ path compression
int findRoot(vector<int>& parent, int node) {
    if (parent[node] != node) { parent[node] = findRoot(parent, parent[node]); }
    return parent[node];
}

// core idea: union-find over account idx; first account listing an email owns it
// later account w/ that email -> union w/ owner, root to root; then group emails by root
// T: O(E log E), S: O(E) — E = total emails; log from sorting
vector<vector<string>> accountsMerge(const vector<vector<string>>& accounts) {
    int n = accounts.size();
    // parent[i] = union-find parent of account i
    vector<int> parent(n);
    for (int i = 0; i < n; i++) { parent[i] = i; }
    // owner[email] = first account that listed it
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
    // groups[root account] = its distinct emails
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

// follow-up:
// DFS instead -> star edges from each account's first email, one DFS per person
// provinces (LC 547) -> union i, j when isConnected[i][j]; cnt cities that are roots
// earliest all friends (LC 1101) -> sort logs by time, union; answer at one component
// point clusters within d -> union every close pair; squared dist in long long
// min cable moves (LC 1319) -> -1 if < n - 1 cables, else components - 1
// union by size -> hang smaller root under larger, add sizes
// dynamic friendship w/ removals known up front -> replay in reverse, removal = union
// org hierarchy (manager / peer) -> union-find of peer classes, 1 manager ptr per class
