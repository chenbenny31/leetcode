// ByteDance (hack2hire) / LC 211: WordDictionary with addWord(word) and search(word),
// where '.' in a search word matches any one letter.

#include <iostream>
#include <string>
#include <map>

using std::cout;
using std::string;
using std::map;

// Trie; search is a DFS where '.' tries every child.
// T: add O(L), search O(L) without dots, O(trie nodes) worst case with dots.
class WordDictionary {
    struct Node {
        // a word ends at this node
        bool isWord = false;
        // next letter -> child node
        map<char, Node> children;
    };
    // empty prefix
    Node root;

    // match word[i..] from node
    bool dfs(const Node& node, const string& word, int i) {
        if (i == (int)word.size()) {
            return node.isWord;
        }
        if (word[i] == '.') {
            for (const auto& [c, child] : node.children) {
                if (dfs(child, word, i + 1)) {
                    return true;
                }
            }
            return false;
        }
        map<char, Node>::const_iterator it = node.children.find(word[i]);
        if (it == node.children.end()) {
            return false;
        }
        return dfs(it->second, word, i + 1);
    }

public:
    // Walk the trie, creating missing nodes.
    void addWord(const string& word) {
        Node* cur = &root;
        for (char c : word) {
            cur = &cur->children[c];
        }
        cur->isWord = true;
    }

    bool search(const string& word) {
        return dfs(root, word, 0);
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    WordDictionary d;
    d.addWord("bad");
    d.addWord("dad");
    d.addWord("mad");
    check(!d.search("pad"), "missing");
    check(d.search("bad"), "exact");
    check(d.search(".ad"), "leading-dot");
    check(d.search("b.."), "trailing-dots");
    check(!d.search("ba"), "prefix-only");
}

// follow-up: speed / memory -> Node* children[26] instead of map, O(1) child lookup;
//   free nodes in destructor
// follow-up: '*' = any sequence -> dfs(node, i): skip '*' (i + 1) or let it eat one
//   letter (each child, i); memo on (node, i)
// follow-up: delete word -> pass count per node, decrement along the path, prune at 0
