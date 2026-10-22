// prefix-sum + binary-search, T: O(log n) pick, S: O(n)

#include <vector>
#include <cstdlib> // std::rand, std::srand
#include <ctime> // std::time

class Solution {
private:
    std::vector<int> prefix; // prefix[i] = sum(w[0..i]);

public:
    Solution(std::vector<int>& w) : prefix(w.size()) {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        prefix.resize(w.size());
        prefix[0] = w[0];
        for (int i = 1; i < w.size(); i++) {
            prefix[i] = prefix[i - 1] + w[i];
        }
    }

    int pickIndex() {
        int target = 1 + std::rand() % prefix.back(); // uniform in [1, total]

        int lo = 0, hi = prefix.size() - 1;
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            if (prefix[mid] < target) {
                lo = mid + 1;
            } else {
                hi = mid;
            }
        }
        return lo;
    }
};
