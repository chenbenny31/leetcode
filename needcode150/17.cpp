// backtracking, T: O(n4^n), S: O(n) recursion path

#include <vector>
#include <string>
#include <string_view>

class Solution {
public:
    std::vector<std::string> letterCombinations(std::string digits) {
        if (digits.empty()) { return {}; }
        std::vector<std::string> out;
        int total = 1;
        for (char d : digits) { total *= (d == '7' || d == '9') ? 4 : 3; }
        out.reserve(total);
        std::string curr;
        curr.reserve(digits.size());
        backtrack(digits, 0, curr, out);
        return out;
    }

private:
    static constexpr string_view MAP[] = {
        "", "", "abc", "def",
        "ghi", "jkl", "mno", "pqrs",
        "tuv", "wxyz"
    };

    void backtrack(const std::string& digits,
                   int idx, std::string& curr,
                   std::vector<std::string>& out) {
        if (idx == static_cast<int>(digits.size())) { out.push_back(curr); return; }

        for (char c : MAP[digits[idx] - '0']) {
            curr.push_back(c);
            backtrack(digits, idx + 1, curr, out);
            curr.pop_back();
        }
    }
};

// static constexpr const char* MAP[]: compile-time string table, no heap allocation
// out.reserve for worst case
// vs iterative bfs: bfs need building combinations by level, introducing O(n4^n), while backtracking uses O(n) stack
// string_view over char*[]: compile-time no heap allocation but .size() avaiable, no null termination scan (C++17)
