// difference-array, T: O(n + R), R = max stop, S: O(R) stack-allocated

#include <vector>
#include <algorithm> // std::fill

class Solution {
public:
    bool carPooling(std::vector<std::vector<int>>& trips, int capacity) {
        constexpr int N = 1001;
        int diff[N];
        std::fill(diff, diff + N, 0);

        for (auto& t : trips) {
            diff[t[1]] += t[0];
            diff[t[2]] -= t[0];
        }

        int curr = 0;
        for (int i = 0; i < N; i++) {
            curr += diff[i];
            if (curr > capacity) { return false; }
        }
        return true;
    }
};

// sweep-line, T: O(nlogn), S: O(n)

#include <vector>
#include <algorithm> // std::sort, std::fill
#include <utility> // std::pair

class Solution {
public:
    bool carPooling(std::vector<std::vector<int>>& trips, int capacity) {
        std::vector<std::pair<int, int>> events;
        events.reserve(trips.size() * 2);

        for (auto& t : trips) {
            events.push_back({t[1], t[0]}); // pickup
            events.push_back({t[2], -t[0]}); // dropff
        }

        std::sort(events.begin(), events.end(), [](const std::pair<int, int>& a,
                                                   const std::pair<int, int>& b) {
            return a.first != b.first ? a.first < b.first : a.second < b.second;
        });

        int curr = 0;
        for (auto& [stop, delta] : events) {
            curr += delta;
            if (curr > capacity) { return false; }
        }
        return true;
    }
};

// difference-array: range update, point query, O(n) mark, O(R) scan, avoid O(nR) brute force
// sweep-line sort tierbreaker: a.second < b.second, dropoff before pickup
// std::fill over std::memset: type-safe, works for any value vs fill byte may cause issue
// difference-array vs sweep-line: better performance vs allow unbounded range
