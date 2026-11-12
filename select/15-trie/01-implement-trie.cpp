// LC 208 Implement Trie (Prefix Tree) (Medium). Pattern: trie with 26 child pointers.
// insert(word); search(word) = word was inserted; startsWith(prefix) = some inserted
// word starts with prefix. Lowercase letters only.

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::lower_bound;
using std::binary_search;

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

// One node per letter along the path; insert creates missing nodes.
// T: O(L) per op, S: O(26 * total letters inserted).
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

// J4 without a trie: sorted order compares letter by letter, so every word starting with
// prefix lies in [prefix, prefix + '{') ('{' is the char after 'z') and no other word
// falls between them: the matches are one contiguous range. T: O(L log n), S: O(1).
int countWithPrefix(const vector<string>& sortedWords, const string& prefix) {
    auto first = lower_bound(sortedWords.begin(), sortedWords.end(), prefix);
    auto last = lower_bound(sortedWords.begin(), sortedWords.end(), prefix + '{');
    return (int)(last - first);
}

// Exact lookup by binary search on the sorted list. T: O(L log n), S: O(1).
bool containsWord(const vector<string>& sortedWords, const string& word) {
    return binary_search(sortedWords.begin(), sortedWords.end(), word);
}

void test(const string& label, bool got, bool want) {
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

void testCount(const string& label, const vector<string>& sortedWords,
               const string& prefix, int want) {
    bool ok = countWithPrefix(sortedWords, prefix) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    Trie trie;
    trie.insert("car");
    trie.insert("cart");
    test("word", trie.search("car"), true);
    test("prefix-not-word", trie.search("ca"), false);
    test("prefix", trie.startsWith("ca"), true);
    test("longer-than-any", trie.startsWith("carts"), false);

    vector<string> sortedWords = {"bat", "car", "card", "care", "cat", "do"};
    testCount("count-word-and-longer", sortedWords, "car", 3);
    testCount("count-short-prefix", sortedWords, "ca", 4);
    testCount("count-none", sortedWords, "x", 0);
    testCount("count-empty-prefix", sortedWords, "", 6);
    test("sorted-word", containsWord(sortedWords, "card"), true);
    test("sorted-prefix-not-word", containsWord(sortedWords, "ca"), false);
}

// follow-up: delete(word) -> per-node pass count, decrement along the path and free a
//   child whose count hits 0
// follow-up: autocomplete top-k -> DFS from the prefix node, or cache top-k per node
