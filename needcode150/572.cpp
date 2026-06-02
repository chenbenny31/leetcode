// dfs, T: O(mn), S: O(h) stack frames

#include <cstddef> // nullptr

class Solution {
public:
    bool isSubtree(TreeNode* root, TreeNode* subRoot) {
        if (!root) { return false; }
        if (is_same(root, subRoot)) { return true; }
        return isSubtree(root->left, subRoot)
            || isSubtree(root->right, subRoot);
    }

private:
    bool is_same(TreeNode* p, TreeNode* q) {
        if (!p && !q) { return true; }
        if (!p || !q) { return false; }
        if (p->val != q->val) { return false; }
        return is_same(p->left, q->left)
            && is_same(p->right, q->right);
    }
};

// serialization + KMP, T: O(m + n), S: O(m + n)

#include <string>
#include <vector>
#include <cstddef> // nullptr

class Solution {
public:
    bool isSubtree(TreeNode* root, TreeNode* subRoot) {
        std::string s1, s2;
        serialize(root, s1);
        serialize(subRoot, s2);
        return kmp_search(s1, s2);
    }

private:
    void serialize(TreeNode* node, std::string& out) {
        if (!node) { out += "#null#"; return; }
        out += "#" + std::to_string(node->val) + "#";
        serialize(node->left, out);
        serialize(node->right, out);
    }

    std::vector<int> build_lps(const std::string& pat) {
        int n = static_cast<int>(pat.size());
        std::vector<int> lps(n ,0); // lps[i] = len of longest str both prefix and suffic of pat[0..i]
        int len = 0, i = 1;
        while (i < n) {
            if (pat[i] == pat[len]) {
                len++;
                lps[i] = len;
                i++;
            } else {
                if (len != 0) { len = lps[len - 1]; }
                else { lps[i] = 0; i++; }
            }
        }
        return lps;
    }

    bool kmp_search(const std::string& text, const std::string& pat) {
        std::vector<int> lps = build_lps(pat);
        int i = 0, j = 0;
        int n = static_cast<int>(text.size());
        int m = static_cast<int>(pat.size());
        while (i < n) {
            if (text[i] == pat[j]) { i++; j++; }
            if (j == m) {
                return true;
            } else if (i < n && text[i] != pat[j]) {
                if (j != 0) { j = lps[j - 1]; }
                else { i++; }
            }
        }
        return false;
    }
};

// #val# not val: intro delim for serial id, false pos, `#12#` vs `#1##2#`
