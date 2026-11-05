// ByteDance (hack2hire) / LC 662: max width of a binary tree = max over levels of
// (rightmost pos - leftmost pos + 1), counting the null slots between the two ends.

#include <iostream>
#include <string>
#include <queue>
#include <algorithm>

using std::cout;
using std::string;
using std::queue;
using std::pair;
using std::max;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
};

// BFS level by level with heap positions (children 2p, 2p+1). T: O(n), S: O(n).
int widthOfBinaryTree(TreeNode* root) {
    if (root == nullptr) {
        return 0;
    }
    // (node, position within its level)
    queue<pair<TreeNode*, long long>> q;
    q.push({root, 0});
    int best = 0;
    while (!q.empty()) {
        int sz = q.size();
        long long first = q.front().second;
        long long last = 0;
        for (int i = 0; i < sz; i++) {
            auto [cur, pos] = q.front();
            q.pop();
            // rebase so the level's leftmost is 0: positions never double past the width
            pos -= first;
            last = pos;
            if (cur->left != nullptr) {
                q.push({cur->left, 2 * pos});
            }
            if (cur->right != nullptr) {
                q.push({cur->right, 2 * pos + 1});
            }
        }
        best = max(best, (int)last + 1);
    }
    return best;
}

void test(const string& label, TreeNode* root, int want) {
    cout << label << (widthOfBinaryTree(root) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("empty", nullptr, 0);

    //    1
    //  2   3
    // 4 . . 5
    TreeNode a4{4, nullptr, nullptr};
    TreeNode a5{5, nullptr, nullptr};
    TreeNode a2{2, &a4, nullptr};
    TreeNode a3{3, nullptr, &a5};
    TreeNode a1{1, &a2, &a3};
    test("gap", &a1, 4);

    //        1
    //    3       2
    //  5   .   .   9
    // 6 . . . . . 7 .
    TreeNode b6{6, nullptr, nullptr};
    TreeNode b7{7, nullptr, nullptr};
    TreeNode b5{5, &b6, nullptr};
    TreeNode b9{9, &b7, nullptr};
    TreeNode b3{3, &b5, nullptr};
    TreeNode b2{2, nullptr, &b9};
    TreeNode b1{1, &b3, &b2};
    test("deep-gap", &b1, 7);
}

// follow-up: why rebase pos -> 2p + 1 doubles per level, overflows on deep skewed
//   trees; pos - first keeps it below the level width
// follow-up: DFS instead -> leftmost[depth] = first pos seen (left-first), best =
//   max(pos - leftmost[depth] + 1), rebase children the same way, O(n), O(h) stack
// follow-up: right side view (LC 199) -> same BFS, record last node of each level
