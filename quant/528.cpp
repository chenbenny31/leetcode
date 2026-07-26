// prefix + binary-search

#include <vector>
#include <random> // std::mt19937, std::random_device, std::uniform_int_distribution
#include <algorithm> // std::lower_bound

class Solution {
private:
    std::vector<int> prefix; // prefix[i] = sum(0..i]
    std::mt19937 rng;

public:
    Solution(vector<int>& w) : rng(std::random_device{}()) {
        prefix.reserve(w.size() + 1); 
        prefix.push_back(w[0]);
        for (int i = 1; i < static_cast<int>(w.size()); i++) {
            prefix.push_back(prefix[i - 1] + w[i]);
        }
    }

    int pickIndex() {
        std::uniform_int_distribution<int> dist(1, prefix.back()); // [1, prefix.back()]
        int target = dist(rng);
        return static_cast<int>(std::lower_bound(prefix.begin(), prefix.end(), target) - prefix.begin());
    }
};
