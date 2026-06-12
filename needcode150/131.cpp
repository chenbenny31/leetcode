// backtracking + dp precomputation, T: O(n2^n), S: O(n2)

#include <vector>
#include <string>
#include <cstring> // std::memest
#include <cstdint> // uint8_t

class Solution {
public:
    std::vector<std::vector<std::string>> partition(std::string s) {
        int n = static_cast<int>(s.size());
        std::vector<std::vector<uint8_t>> isPalin(n, std::vector<uint8_t>(n, 0)); // isPalin[i][j] = s[i..j] includsive is palindrome

        for (int i = n - 1; i >= 0; i--) {
            for (int j = i; j < n; j++) {
                if (s[i] == s[j] && (j - i <= 2 || isPalin[i + 1][j - 1])) { isPalin[i][j] = 1; }
            }
        }

        std::vector<std::vector<std::string>> out;
        std::vector<std::string> curr;
        backtrack(s, isPalin, 0, curr, out);
        return out;
    }

private:
    void backtrack(const std::string& s, std::vector<std::vector<uint8_t>>& isPalin,
                   int start, std::vector<std::string>& curr, std::vector<std::vector<std::string>>& out) {
        if (start == static_cast<int>(s.size())) { out.push_back(curr); return; }

        for (int end = start; end < static_cast<int>(s.size()); end++) {
            if (!isPalin[start][end]) { continue; }
            curr.push_back(s.substr(start, end - start + 1));
            backtrack(s, isPalin, end + 1, curr, out);
            curr.pop_back();
        }
    }
};

// dp precomputation: isPalin[i][j] = s[i]==s[j] && (j-i <= 2 || isPalin[i+1][j-1]), single/two-char always true, isPalin[i+1][j-1] compute before isPalin[i][j]
// right-to-left outer loop: isPalin[i][j] depends on isPalin[i+1][j-1], inner substrings computed first
// why dp precomputation: dp reduce palindrome check to O(1)
// store indices rather than substrings: curr stores {start, end} pairs of O(1) push/pop
