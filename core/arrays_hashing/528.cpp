// prefix-sum + linear-scan, T: O(n) pickIndex, S: O(n)

#include <vector>
#include <random> // std::mt19937, std::random_device, std::uniform_int_distribution

class Solution {
private:
    std::vector<int> prefix;
    int total;
    std::mt19937 rng;

public:
    Solution(std::vector<int>& w) : rng(std::random_device{}()) {
        prefix.reserve(w.size());
        total = 0;
        for (int i = 0; i < static_cast<int>(w.size()); i++) {
            total += w[i];
            prefix.push_back(total);
        }
    }

    int pickIndex() {
        std::uniform_int_distribution<int> dist(1, total);
        int target = dist(rng);

        for (int i = 0; i < static_cast<int>(prefix.size()); i++) {
            if (prefix[i] >= target) {
                return i;
            }
        }
        return -1;
    }
};

// prefix-sum + binary-search, T: O(logn) pickIndex, S: O(n)

#include <vector>
#include <random> // std::mt19937, std::random_device, std::uniform_int_distribution
#include <algorithm> // std::lower_bound

class Solution {
private:
    std::vector<int> prefix;
    int total;
    std::mt19937 rng;

public:
    Solution(std::vector<int>& w) : rng(std::random_device{}()) {
        prefix.reserve(w.size());
        total = 0;
        for (int i = 0; i < static_cast<int>(w.size()); i++) {
            total += w[i];
            prefix.push_back(total);
        }
    }

    int pickIndex() {
        std::uniform_int_distribution<int> dist(1, total);
        int target = dist(rng);

        int lo = 0;
        int hi = static_cast<int>(prefix.size()) - 1;
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            if (prefix[mid] < target) {
                lo = mid + 1;
            } else {
                hi = mid;
            }
        }
        // auto it = std::lower_bound(prefix.begin(), prefix.end(), target);
        // return static_cast<int>(it - prefix.begin());
        return lo;
    }
};

// binary-search over linear-scan: determined by the size of data range
// why not reservoir-sampling: full data is known in adv, pre-compute beats reservoir-sampling (re-pay O(n) on every call)
