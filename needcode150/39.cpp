// backtracking, T: O(n^(target/min)), S: O(target/min) recursion path

#include <vector>
#include <algorithm> // std::sort

class Solution {
public:
    std::vector<std::vector<int>> combinationSum(std::vector<int>& candidates, int target) {
        std::sort(candidates.begin(), candidates.end());
        std::vector<std::vector<int>> out;
        std::vector<int> curr;
        backtrack(candidates, target, 0, curr, out);
        return out;
    }

private:
    void backtrack(std::vector<int>& candidates, int remain, int start,
                   std::vector<int>& curr, std::vector<std::vector<int>>& out) {
        if (remain == 0) {
            out.push_back(curr);
            return;
        }

        for (int i = start; i < static_cast<int>(candidates.size()); i++) {
            if (candidates[i] > remain) { break; } // pruning, sorted
            curr.push_back(candidates[i]);
            backtrack(candidates, remain - candidates[i], i, curr, out);
            curr.pop_back();
        }
    }
};

// i not i + 1 in recursive call: same candidate re-usable
// sort + pruning: candidates[i] > remain prunes further branches
// remain as parameter not global: decremented value passed down
