// array-backed trie + pre-alloc poll, T: O(m) all ops, m: word length, S: O(26mn)

#include <cstring> // std::memset
#include <string>

class Trie {
private:
    static constexpr int N = 100001;
    static constexpr char BASE = 'a';

    struct TrieNode {
        int children[26];
        bool is_end;
    };

    TrieNode pool[N];
    int cnt = 0;

    int new_node() {
        pool[cnt].is_end = false;
        std::memset(pool[cnt].children, -1, sizeof(pool[cnt].children));
        cnt++;
        return cnt - 1;
    }

public:
    Trie() { cnt = 0; new_node(); } // node 0 = root

    void insert(const std::string& word) {
        int cur = 0;
        for (char c : word) {
            int idx = c - BASE;
            if (pool[cur].children[idx] == -1) { pool[cur].children[idx] = new_node(); }
            cur = pool[cur].children[idx];
        }
        pool[cur].is_end = true;
    }

    bool search(const std::string& word) {
        int cur = 0;
        for (char c : word) {
            int idx = c - BASE;
            if (pool[cur].children[idx] == -1) { return false; }
            cur = pool[cur].children[idx];
        }
        return pool[cur].is_end;
    }

    bool startsWith(const std::string& prefix) {
        int cur = 0;
        for (char c : prefix) {
            int idx = c - BASE;
            if (pool[cur].children[idx] == -1) { return false; }
            cur = pool[cur].children[idx];
        }
        return true;
    }
};

// cache pool of nodes: flat contiguous array
// Node size 26 * 4 bytes(int children) + 1 byte (bool) + 3 padding = 108 bytes fits two cache line, vs 26 * 8(Node*) + 1 + 7 padding = 216 bytes, four cache lines
// pool size 100001 covers up worst case, zero heap activity
// pool vs pointer trie: pool wastes 26 ints per node, apply for fixed alphabet
// thread-safety: cnt++ needs to be atomic, use std::atomic<int> cnt with fetch_add(1)
