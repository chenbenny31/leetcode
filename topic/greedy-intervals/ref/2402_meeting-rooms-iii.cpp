// LC 2402 Meeting Rooms III: each meeting takes the lowest free room, or waits for the
// room that frees first; return the room that held the most meetings.
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <utility>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::priority_queue;
using std::pair;
using std::greater;
using std::sort;
using std::max_element;

// Two heaps in start order: free rooms by number, busy rooms by (end, number). With no
// free room, the meeting moves to the busy room that ends first.
// T: O(m log m + m log n), S: O(n + m).
int mostBooked(int n, vector<vector<int>> meetings) {
    sort(meetings.begin(), meetings.end());
    priority_queue<int, vector<int>, greater<int>> freeRooms;   // free room numbers
    for (int r = 0; r < n; r++) { freeRooms.push(r); }
    // (end time, room) of rooms in use; ends can pass int once meetings are delayed
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> busy;
    vector<int> held(n, 0);   // held[r] = meetings room r has hosted

    for (const auto& m : meetings) {
        long long start = m[0], end = m[1];
        while (!busy.empty() && busy.top().first <= start) {
            freeRooms.push(busy.top().second);
            busy.pop();
        }

        int room;
        if (!freeRooms.empty()) {
            room = freeRooms.top();
            freeRooms.pop();
        } else {
            auto [freeAt, r] = busy.top();
            busy.pop();
            end = freeAt + (end - start);
            room = r;
        }

        busy.push({end, room});
        held[room]++;
    }

    return max_element(held.begin(), held.end()) - held.begin();
}

void test(const string& label, int n, const vector<vector<int>>& meetings, int want) {
    cout << label << (mostBooked(n, meetings) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("reuse-free", 2, {{0, 10}, {1, 2}, {3, 4}}, 1);
    test("delay-earliest-end", 2, {{0, 10}, {1, 3}, {2, 4}}, 1);
    test("delay-tie", 2, {{0, 10}, {1, 5}, {2, 7}, {3, 4}}, 0);
}
