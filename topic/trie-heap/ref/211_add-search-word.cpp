// LC 211 Design Add and Search Words Data Structure: addWord(word); search(word) where
// '.' in word matches any one letter. Lowercase letters only.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

struct TrieNode {
    int child[26] = {};   // child[c] = pool index of the next node, 0 = none
    bool end = false;     // some added word ends here
};

// Trie plus DFS: a letter follows one child, '.' tries all 26.
// T: addWord O(L), search O(26^d * L) with d dots, S: O(total letters * 26).
class WordDictionary {
    vector<TrieNode> pool;   // node 0 is the root

    bool dfs(const string& word, int i, int node) {
        int n = word.size();
        if (i == n) { return pool[node].end; }
        if (word[i] != '.') {
            int nxt = pool[node].child[word[i] - 'a'];
            return nxt != 0 && dfs(word, i + 1, nxt);
        }
        for (int c = 0; c < 26; c++) {
            int nxt = pool[node].child[c];
            if (nxt != 0 && dfs(word, i + 1, nxt)) { return true; }
        }
        return false;
    }

public:
    WordDictionary() : pool(1) {}

    void addWord(string word) {
        int cur = 0;
        for (char c : word) {
            if (pool[cur].child[c - 'a'] == 0) {
                // push_back may reallocate, so take the index before using pool[cur]
                int fresh = pool.size();
                pool.push_back(TrieNode());
                pool[cur].child[c - 'a'] = fresh;
            }
            cur = pool[cur].child[c - 'a'];
        }
        pool[cur].end = true;
    }

    bool search(string word) {
        return dfs(word, 0, 0);
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    WordDictionary dict;
    dict.addWord("bad");
    dict.addWord("dad");
    check(dict.search("bad") && !dict.search("pad"), "exact");
    check(dict.search(".ad") && dict.search("b..") && dict.search("..."), "wildcard");
    check(!dict.search("b.") && !dict.search("...."), "length-mismatch");
    check(!dict.search("ba"), "prefix-is-not-word");
}
