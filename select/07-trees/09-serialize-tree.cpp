// LC 297 Serialize and Deserialize Binary Tree (Hard). Pattern: preorder + null markers.
// Design a Codec that turns a binary tree into a string and rebuilds the same tree from
// that string. Values may be negative.

#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::stoi;
using std::string;
using std::to_string;
using std::vector;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x, TreeNode* l = nullptr, TreeNode* r = nullptr)
        : val(x), left(l), right(r) {}
};

// Preorder, comma-separated, "#" for null: the null markers fix the shape, so one
// traversal is enough, e.g. "1,#,2,#,#". T: O(n), S: O(n) for both directions.
class Codec {
public:
    string serialize(TreeNode* root) {
        string out;
        write(root, out);
        return out;
    }

    TreeNode* deserialize(const string& data) {
        // preorder tokens, "#" for null
        vector<string> tokens;
        string tok;
        for (char c : data) {
            if (c == ',') {
                tokens.push_back(tok);
                tok = "";
            } else {
                tok += c;
            }
        }
        tokens.push_back(tok);
        int pos = 0;
        return dfs(tokens, pos);
    }

private:
    void write(TreeNode* node, string& out) {
        if (!out.empty()) { out += ','; }
        if (!node) {
            out += '#';
            return;
        }
        out += to_string(node->val);
        write(node->left, out);
        write(node->right, out);
    }

    // pos is shared: each call consumes exactly its own subtree's tokens
    TreeNode* dfs(const vector<string>& tokens, int& pos) {
        string tok = tokens[pos];
        pos++;
        if (tok == "#") { return nullptr; }
        TreeNode* node = new TreeNode(stoi(tok));
        node->left = dfs(tokens, pos);
        node->right = dfs(tokens, pos);
        return node;
    }
};

void freeTree(TreeNode* node) {
    if (!node) { return; }
    freeTree(node->left);
    freeTree(node->right);
    delete node;
}

void test(const string& label, TreeNode* root, const string& want) {
    Codec codec;
    TreeNode* copy = codec.deserialize(want);
    bool ok = codec.serialize(root) == want && codec.serialize(copy) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
    freeTree(copy);
}

int main() {
    TreeNode a2(2);
    TreeNode a3(3);
    TreeNode a1(1, &a2, &a3);
    test("full", &a1, "1,2,#,#,3,#,#");
    TreeNode b2(2);
    TreeNode b1(1, nullptr, &b2);
    test("right-chain", &b1, "1,#,2,#,#");
    TreeNode c4(-4);
    TreeNode c120(120);
    TreeNode c0(0, &c4, &c120);
    test("negative-multi-digit", &c0, "0,-4,#,#,120,#,#");
    test("empty", nullptr, "#");
}

// follow-up: BST only (LC 449) -> preorder without "#", rebuild with (lo, hi) bounds
// follow-up: LeetCode's level-order format -> BFS writing "#" for null children, read
//   back with a queue, attaching two tokens per popped node
