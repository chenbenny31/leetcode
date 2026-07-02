// ordered map, T: O(nlogn), S: O(n)

#include <vector>
#include <map>

class Solution {
public:
    bool isNStraightHand(std::vector<int>& hand, int groupSize) {
        int n = static_cast<int>(hand.size());
        if (n % groupSize != 0) { return false; }

        std::map<int, int> count;
        for (int card : hand) { count[card]++; }

        for (auto& [start, cnt] : count) {
            if (cnt == 0) { continue; }
            int need = cnt;
            for (int i = 0; i < groupSize; i++) {
                auto it = count.find(start + i);
                if (it == count.end() || it->second < need) { return false; }
                it->second -= need;
            }
        }
        return true;
    }
};

// sort + unordered map, T: O(nlogn), S: O(n)

#include <vector>
#include <unordered_map>
#include <algorithm> // std::sort

class Solution {
public:
    bool isNStraightHand(std::vector<int>& hand, int groupSize) {
        int n = static_cast<int>(hand.size());
        if (n % groupSize != 0) { return false; }

        std::unordered_map<int, int> count;
        count.reserve(n);
        count.max_load_factor(0.25f);
        for (int card : hand) { count[card]++; }

        std::sort(hand.begin(), hand.end());
        for (int card : hand) {
            auto it = count.find(card);
            if (it == count.end() || it->second == 0) { continue; }
            int need = it->second;
            for (int i = 0; i < groupSize; i++) {
                auto jt = count.find(card + i);
                if (jt == count.end() || jt->second < need) { return false; }
                jt->second -= need;
            }
        }
        return true;
    }
};
