// greedy one-pass, T: O(n), S: O(1)

#include <string>
#include <vector>
#include <algorithm> // std::max

class Solution {
public:
    std::vector<int> partitionLabels(std::string s) {
        constexpr int R = 26;
        constexpr char BASE = 'a';
        int n = static_cast<int>(s.size());

        int last[R];
        for (int i = 0; i < n; i++) {
            last[s[i] - BASE] = i;
        }

        std::vector<int> out;
        out.reserve(R);

        int start = 0, end = 0;
        for (int i = 0; i < n; i++) {
            end = std::max(end, last[s[i] - BASE]);
            if (i == end) {
                out.push_back(i - start + 1);
                start = i + 1;
            }
        }
        return out;
    }
};

// last[]: greedy need how far right a char's ending at for comp max sep
