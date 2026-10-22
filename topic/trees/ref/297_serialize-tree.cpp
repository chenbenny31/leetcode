// LC 297 Serialize and Deserialize Binary Tree: design a Codec that turns a binary tree
// into a string and rebuilds the same tree from that string.
#include <iostream>
#include <string>
using std::cout;
using std::string;
using std::to_string;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int v = 0, TreeNode* l = nullptr, TreeNode* r = nullptr)
        : val(v), left(l), right(r) {}
};

// Preorder with "#" for null: the null markers fix the shape, so one traversal is enough.
// Format: every token is followed by a comma, e.g. "1,2,#,#,3,#,#,".
class Codec {
    void write(TreeNode* root, string& out) {
        if (root == nullptr) {
            out += "#,";
            return;
        }
        out += to_string(root->val) + ",";
        write(root->left, out);
        write(root->right, out);
    }

    // pos is shared across calls: each call consumes exactly its own subtree's tokens
    TreeNode* read(const string& data, int& pos) {
        if (data[pos] == '#') {
            pos += 2;
            return nullptr;
        }
        int sign = 1;
        if (data[pos] == '-') {
            sign = -1;
            pos++;
        }
        int val = 0;
        while (data[pos] >= '0' && data[pos] <= '9') {
            val = val * 10 + (data[pos] - '0');
            pos++;
        }
        pos++;
        TreeNode* node = new TreeNode(sign * val);
        node->left = read(data, pos);
        node->right = read(data, pos);
        return node;
    }

public:
    // T: O(n), S: O(n)
    string serialize(TreeNode* root) {
        string out;
        write(root, out);
        return out;
    }

    // T: O(n), S: O(n)
    TreeNode* deserialize(string data) {
        int pos = 0;   // start of the next unread token
        return read(data, pos);
    }
};

bool sameTree(TreeNode* a, TreeNode* b) {
    if (a == nullptr || b == nullptr) { return a == b; }
    return a->val == b->val && sameTree(a->left, b->left) && sameTree(a->right, b->right);
}

void freeTree(TreeNode* root) {
    if (root == nullptr) { return; }
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

bool roundTrip(TreeNode* root) {
    Codec codec;
    TreeNode* copy = codec.deserialize(codec.serialize(root));
    bool ok = sameTree(root, copy);
    freeTree(copy);
    return ok;
}

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    TreeNode a3(3), a2(2, nullptr, &a3), a1(1, &a2);
    check(roundTrip(&a1), "right-chain");
    TreeNode b4(-4), b12(120), b0(0, &b4, &b12);
    check(roundTrip(&b0), "negative-multi-digit");
    TreeNode c2(2), c3(3, &c2), c1(1, &c3);
    check(roundTrip(&c1), "left-chain");
    check(roundTrip(nullptr), "empty");
}
