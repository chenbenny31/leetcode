// preorder dfs with null markers, T: O(n), S: O(n)

#include <string> // std::to_string, std::getline, std::stoi
#include <sstream> // std::istringstream
#include <cstddef> // nullptr

class Codec {
public:
    std::string serialize(TreeNode* root) {
        std::string out;
        encode(root, out);
        return out;
    }

    TreeNode* deserialize(std::string data) {
        std::istringstream ss(data);
        return decode(ss);
    }

private:
    void encode(TreeNode* node, std::string& out) {
        if (!node) { out += "#$"; return; }
        out += "^" + std::to_string(node->val) + "$";
        encode(node->left, out);
        encode(node->right, out);
    }

    TreeNode* decode(std::istringstream& ss) {
        std::string token;
        std::getline(ss, token, '$');
        if (token == "#") { return nullptr; }
        TreeNode* node = new TreeNode(std::stoi(token.substr(1)));
        node->left = decode(ss);
        node->right = decode(ss);
        return node;
    }
};

// ^ prefix: disambiguates sign, chosen because it never appears in int representation
// $ delimiter: fixed terminator, getline() splits cleanly
// token.substr(1): strip ^ prefix before parsing, safe from ^val$
// std::istringstream: tokenization sequential consumption, shared ss by refernece
// reserve upfront out in production: out.reserve(n * 8), estimate ~8 chars per node
