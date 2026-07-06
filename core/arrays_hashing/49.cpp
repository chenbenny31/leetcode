// sort-key grouping, T: O(nklogk), S: O(nk)

// counting-key grouping, T: O(nk), S: O(nk)

#include <vector>
#include <string>
#include <unordered_map>

class Solution {
private:
    static constexpr int R = 26;
    static constexpr char BASE = 'a';

    std::string count_key(const std::string& s) const {
        int freq[R] = {};
        for (int i = 0; i < static_cast<int>(s.size()); i++) {
            freq[s[i] - BASE]++;
        }

        std::string key;
        key.reserve(R * 2);
        for (int i = 0; i < R; i++) {
            key += static_cast<char>(BASE + i);
            key += std::to_string(freq[i]);
        }
        return key;
    }

public:
    std::vector<std::vector<std::string>> groupAnagrams(std::vector<std::string>& strs) {
        std::unordered_map<std::string, std::vector<std::string>> groups;
        groups.reserve(strs.size());

        for (int i = 0; i < static_cast<int>(strs.size()); i++) {
            std::string key = count_key(strs[i]);
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

// sorted-key and counting-key grouping both pay one heap alloc per str for the key <- std::array<uint8_t, 26> + custom std::hash in C++17
// cache behavior: freq[26] is 104 bytes on stack, L1-resident
// copy avoidance: std::move(strs[i]) and std::move(entry.second)
