// LC 855 Exam Room (Medium). Pattern: ordered set, scan gaps.
// n seats in a row. seat() takes the seat that maximizes the distance to the closest
// student (ties -> lowest index; seat 0 if the room is empty); leave(p) frees seat p.

#include <iostream>
#include <string>
#include <set>

using std::cout;
using std::string;
using std::set;

// Candidates: seat 0, the middle of each gap between neighbors, seat n - 1.
// T: O(p) seat, O(log p) leave; p = students seated. S: O(p).
class ExamRoom {
    // seats are 0..n-1
    int n;
    // occupied seats, sorted
    set<int> taken;

public:
    ExamRoom(int seats) : n(seats) {}

    int seat() {
        int bestSeat = 0;
        if (!taken.empty()) {
            // sitting at 0: distance to the first student
            int bestDist = *taken.begin();
            int prev = *taken.begin();
            for (int cur : taken) {
                // sit in the middle of the gap (prev, cur)
                int dist = (cur - prev) / 2;
                // strict > keeps the lowest index on ties
                if (dist > bestDist) {
                    bestDist = dist;
                    bestSeat = prev + dist;
                }
                prev = cur;
            }
            // sitting at n - 1: distance to the last student
            int lastDist = n - 1 - prev;
            if (lastDist > bestDist) {
                bestSeat = n - 1;
            }
        }
        taken.insert(bestSeat);
        return bestSeat;
    }

    void leave(int p) { taken.erase(p); }
};

void test(const string& label, int got, int want) {
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    ExamRoom room(8);
    test("empty-room", room.seat(), 0);
    test("right-end", room.seat(), 7);
    test("middle", room.seat(), 3);
    test("bigger-gap", room.seat(), 5);
    room.leave(0);
    test("left-end", room.seat(), 0);
    test("tie-lowest", room.seat(), 1);
}

// follow-up: O(log p) seat -> set of gaps ordered by (dist desc, start asc) plus
//   left/right neighbor maps; leave merges the two gaps around p. Hard to write live
