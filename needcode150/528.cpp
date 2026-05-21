// prefix-sum + binary-search, T: O(logn) pick, S: O(n)

#include <vector>
#include <cstdlib> // std::rand
#include <numeric> // std::partial_sum
#include <algorithm> // std::lower_bound

class Solution {
    std::vector<int> prefix;
    int total;

public:
    Solution(std::vector<int>& w) {
        prefix.resize(w.size());
        std::partial_sum(w.begin(), w.end(), prefix.begin());
        total = prefix.back();
    }

    int pickIndex() {
        int r = std::rand() % total + 1;
        return static_cast<int>(
            std::lower_bound(prefix.begin(), prefix.end(), r) - prefix.begin()
        );
    }
};

// prefix sum encodes weight: prefix[i] = sum(w[0..i]), rand int of [1, total] falls in (prefix[i-1], prefix[i]) with prob w[i] / total
// r = rand() % total + 1: range [1, total], lower_bound finds first prefix >= r
// std::parital_sum: vectorizable O(1) cache-friendly
// lower_bound correctness: prefix strictly incre, lower_bound returns first elem >= r, never touch end() as r <= total = prefix.back()
// O(1) pick: walker's alias, pre-compute prob and alias table, O(1) pick with two rand num
// floating-point wegiht: (double)rand() / RAND_MAX * total
