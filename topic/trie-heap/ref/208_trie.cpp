// LC 208 Implement Trie (Prefix Tree): insert(word), search(word) for an exact word,
// startsWith(prefix) for any inserted word with that prefix. Lowercase letters only.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

struct TrieNode {
    int child[26] = {};   // child[c] = pool index of the next node, 0 = none
    bool end = false;     // some inserted word ends here
};

// Prefix tree stored in a pool; node 0 is the root, so index 0 can mean "no child".
// T: O(L) per call, S: O(total letters inserted * 26).
class Trie {
    vector<TrieNode> pool;   // every node, indexed by int instead of pointers

    // node reached by following s from the root, or -1 if the path breaks
    int walk(const string& s) {
        int cur = 0;
        for (char c : s) {
            int nxt = pool[cur].child[c - 'a'];
            if (nxt == 0) { return -1; }
            cur = nxt;
        }
        return cur;
    }

public:
    Trie() : pool(1) {}

    void insert(string word) {
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
        int node = walk(word);
        return node != -1 && pool[node].end;
    }

    bool startsWith(string prefix) {
        return walk(prefix) != -1;
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    Trie trie;
    trie.insert("app");
    trie.insert("apple");
    check(trie.search("app") && trie.search("apple"), "search-hit");
    check(!trie.search("ap") && !trie.search("apples"), "search-prefix-only-miss");
    check(trie.startsWith("ap") && !trie.startsWith("b"), "starts-with");
    check(trie.startsWith(""), "empty-prefix");
}
