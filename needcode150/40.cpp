// backtracking with duplicate skip, T: O(2^n), S: O(n) recursion path

#include <vector>
#include <algorithm> // std::sort

class Solution {
public:
    std::vector<std::vector<int>> combinationSum2(std::vector<int>& candidate, int target) {
        std::sort(candidate.begin(), candidate.end());
        std::vector<std::vector<int>> out;
        std::vector<int> curr;
        backtrack(candidate, target, 0, curr, out);
        return out;
    }

private:
    void backtrack(std::vector<int>& candidates, int remain, int start,
                   std::vector<int>& curr, std::vector<std::vector<int>>& out) {
        if (remain == 0) { out.push_back(curr); return; }

        for (int i = start; i < static_cast<int>(candidates.size()); i++) {
            if (candidates[i] > remain) { break; }
            if (i > start && candidates[i] == candidates[i - 1]) { continue; }
            curr.push_back(candidates[i]);
            backtrack(candidates, remain - candidates[i], i + 1, curr, out);
            curr.pop_back();
        }
    }
};

// i + 1 not i: each candidates used at most once
// i > start && candidates[i] == candidates[i-1]: skip duplicate at the same recursion level, avoid identical combinations
