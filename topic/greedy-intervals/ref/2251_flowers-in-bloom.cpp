// LC 2251 Number of Flowers in Full Bloom: flowers[i] = {start, end} is the inclusive
// bloom range. For each arrival time in people, count the flowers in bloom.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::sort;
using std::upper_bound;
using std::lower_bound;

// In bloom at t = started (start <= t) minus ended (end < t); both counts come from
// binary search on sorted starts and ends. T: O((F + P) log F), S: O(F).
vector<int> fullBloomFlowers(const vector<vector<int>>& flowers,
                             const vector<int>& people) {
    vector<int> starts;   // bloom start days, sorted
    vector<int> ends;     // bloom end days, sorted
    for (const auto& f : flowers) {
        starts.push_back(f[0]);
        ends.push_back(f[1]);
    }
    sort(starts.begin(), starts.end());
    sort(ends.begin(), ends.end());
    vector<int> result;
    for (int t : people) {
        int started = upper_bound(starts.begin(), starts.end(), t) - starts.begin();
        int ended = lower_bound(ends.begin(), ends.end(), t) - ends.begin();
        result.push_back(started - ended);
    }
    return result;
}

void test(const string& label, const vector<vector<int>>& flowers,
          const vector<int>& people, const vector<int>& want) {
    cout << label << (fullBloomFlowers(flowers, people) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("two-overlap", {{1, 3}, {2, 5}}, {0, 2, 3, 6}, {0, 2, 2, 0});
    test("single-day", {{4, 4}}, {3, 4, 5}, {0, 1, 0});
    test("no-flowers", {}, {1}, {0});
}
