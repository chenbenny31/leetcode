// sort, T: O(nlogn), S: O(1) auxiliary, O(logn) recursion

#include <vector>
#include <algorithm> // std::sort
#include <functional> // std::greater

class Solution {
public:
    int hIndex(std::vector<int>& citations) {
        std::sort(citations.begin(), citations.end(), std::greater<int>()); // decre-order

        const int n = static_cast<int>(citations.size());
        int h = 0;
        while (h < n && citations[h] > h) { h++; }
        return h;
    }
};

// couting-sort, T: O(n), S: O(n)

#include <algorithm> // std::min

class Solution {
public:
    int hIndex(std::vector<int>& citations) {
        const int n = static_cast<int>(citations.size());

        std::vector<int> buckets(n + 1, 0);
        for (int i = 0; i < n; i++) {
            int idx = std::min(citations[i], n);
            buckets[idx]++; // clamp: >n is indistinguishable from ==n
        }

        int papers = 0;
        for (int h = n; h >= 0; h--) {
            papers += buckets[h]; // papers with >= h citations
            if (papers >= h) { return h; } // first h qualifies
        }
        return 0;
    }
};
