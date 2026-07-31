// bfs + indices-map

#include <vector>
#include <unordered_map>
#include <cstdint>

class Solution {
public:
    int minJumps(std::vector<int>& arr) {
        const int n = static_cast<int>(arr.size());
        if (n <= 1) { return 0; }

        std::unordered_map<int, std::vector<int>> map; // val: [indices,]
        map.reserve(n);
        for (int i = 0; i < n; i++) { map[arr[i]].push_back(i); }

        std::vector<uint8_t> visited(n, false);
        std::vector<int> q; // store idx of arr
        q.reserve(n);
        q.push_back(0);
        int head = 0;
        visited[0] = 1;

        int steps = 0;
        while (head < static_cast<int>(q.size())) {
            int sz = static_cast<int>(q.size()) - head;

            for (int i = head; i < head + sz; i++) {
                int idx = q[i];
                if (idx == n - 1) { return steps; }

                auto it = map.find(arr[idx]);
                if (it != map.end()) {
                    for (int j : it->second) {
                        if (!visited[j]) {
                            visited[j] = 1;
                            q.push_back(j);
                        }
                    }
                    it->second.clear(); // exhuast this value group
                }

                if (idx - 1 >= 0 && !visited[idx - 1]) {
                    visited[idx - 1] = 1;
                    q.push_back(idx - 1);
                }
                if (idx + 1 < n && !visited[idx + 1]) {
                    visited[idx + 1] = 1;
                    q.push_back(idx + 1);
                }
            }

            head = sz;
            steps++;
        }
        return -1;
    }
};
