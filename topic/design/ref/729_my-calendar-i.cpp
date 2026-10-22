// LC 729 My Calendar I: book(startTime, endTime) adds the half-open event
// [startTime, endTime) and returns true only if it overlaps no booked event.
#include <iostream>
#include <string>
#include <map>
using std::cout;
using std::map;
using std::prev;
using std::string;

class MyCalendar {
    // start -> end of each booked event; events are disjoint, so ends are sorted too
    map<int, int> events;

public:
    // only the neighbours of the new start can overlap.
    // T: O(log n) per booking, S: O(n).
    bool book(int startTime, int endTime) {
        auto nxt = events.lower_bound(startTime);   // first event starting at or after
        if (nxt != events.end() && nxt->first < endTime) { return false; }
        if (nxt != events.begin() && prev(nxt)->second > startTime) { return false; }
        events[startTime] = endTime;
        return true;
    }
};

void check(bool ok, const string& label) {
    if (ok) {
        cout << label << " PASS\n";
    } else {
        cout << label << " FAIL\n";
    }
}

int main() {
    MyCalendar c;
    check(c.book(10, 20), "first");
    check(!c.book(15, 25), "overlap-tail");
    check(c.book(20, 30) && c.book(5, 10), "adjacent");
    check(!c.book(0, 40), "enclosing");
}
