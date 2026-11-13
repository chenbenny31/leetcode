// LC 208 Implement Trie (Prefix Tree) (Medium) — trie with 26 child pointers
// insert(word); search(word) = word was inserted; startsWith(prefix) = some inserted
// word starts with prefix.
// Clarify: alphabet (lowercase a-z only); empty string; the same word inserted twice;
//   delete needed.

#include <iostream>
#include <string>

using std::cout;
using std::string;

struct TrieNode {
    // children[c] = node for letter 'a' + c, nullptr = none
    TrieNode* children[26] = {};
    // some inserted word ends here
    bool isEnd = false;

    ~TrieNode() {
        for (TrieNode* child : children) {
            delete child;
        }
    }
};

// Core idea: one node per letter along the path; insert creates the missing nodes and
// marks the last one, lookups just walk. T: O(L) per op, S: O(26 * total letters).
class Trie {
    // node for the empty prefix
    TrieNode root;

    // node reached by spelling s from the root, nullptr if the path breaks
    TrieNode* walk(const string& s) {
        TrieNode* cur = &root;
        for (char ch : s) {
            cur = cur->children[ch - 'a'];
            if (cur == nullptr) { return nullptr; }
        }
        return cur;
    }

public:
    void insert(const string& word) {
        TrieNode* cur = &root;
        for (char ch : word) {
            int letter = ch - 'a';
            if (cur->children[letter] == nullptr) {
                cur->children[letter] = new TrieNode();
            }
            cur = cur->children[letter];
        }
        cur->isEnd = true;
    }

    bool search(const string& word) {
        TrieNode* node = walk(word);
        return node != nullptr && node->isEnd;
    }

    bool startsWith(const string& prefix) { return walk(prefix) != nullptr; }
};

void test(const string& label, bool got, bool want) {
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    Trie trie;
    trie.insert("car");
    trie.insert("cart");
    test("word", trie.search("car"), true);
    test("prefix-not-word", trie.search("ca"), false);
    test("prefix", trie.startsWith("ca"), true);
    test("longer-than-any", trie.startsWith("carts"), false);
    return 0;
}

// follow-up (ideas only):
// - count words with a prefix, no trie -> sort the words; the matches are the one
//   contiguous range [prefix, prefix + '{') found by two lower_bounds, O(L log n)
// - exact lookup without a trie -> binary search on the sorted list, O(L log n)
// - delete(word) -> per-node pass count, decrement along the path and free a child
//   whose count hits 0
// - autocomplete top-k -> DFS from the prefix node, or cache the top k per node
