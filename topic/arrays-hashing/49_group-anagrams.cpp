// counting-key grouping, T: O(nk), S: O(nk), n = num of strs, k = max str len

#include <vector>
#include <string> // std::to_string
#include <unordered_map>
#include <utility> // std::move

class Solution {
private:
    static constexpr int R = 26;
    static constexpr char BASE = 'a';

    std::string countKey(const std::string& s) const {
        int freq[R] = {};
        for (int i = 0; i < static_cast<int>(s.size()); i++) {
            freq[s[i] - BASE]++;
        }

        std::string key; // no-reserve, typical keys are short for SSO
        for (int i = 0; i < R; i++) {
            if (freq[i] > 0) {
                key += static_cast<char>(BASE + i);
                key += std::to_string(freq[i]);
            }
        }
        return key;
    }

public:
    std::vector<std::vector<std::string>> groupAnagrams(std::vector<std::string>& strs) {
        std::unordered_map<std::string, std::vector<std::string>> groups;
        groups.reserve(strs.size());

        for (int i = 0; i < static_cast<int>(strs.size()); i++) {
            std::string key = countKey(strs[i]);
            groups[key].push_back(std::move(strs[i]));
        }

        std::vector<std::vector<std::string>> out;
        out.reserve(groups.size());
        for (auto& entry : groups) {
            out.push_back(std::move(entry.second));
        }
        return out;
    }
};
