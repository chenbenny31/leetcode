// LC 1396 Design Underground System: checkIn(id, stationName, t), checkOut(id,
// stationName, t) for a customer's single trip at a time; getAverageTime(start, end)
// returns the average duration of all completed trips from start to end (directional).
#include <iostream>
#include <string>
#include <unordered_map>
#include <cmath>
using std::cout;
using std::fabs;
using std::pair;
using std::string;
using std::unordered_map;

class UndergroundSystem {
    // customer id -> (station, time) of the check-in still open
    unordered_map<int, pair<string, int>> open;
    // "start>end" -> (total duration, trip count) of completed trips
    unordered_map<string, pair<long long, int>> trips;

public:
    // T: O(1) average
    void checkIn(int id, const string& stationName, int t) {
        open[id] = {stationName, t};
    }

    // close the open trip and add it to its route totals. T: O(len of names).
    void checkOut(int id, const string& stationName, int t) {
        auto [start, startTime] = open[id];
        open.erase(id);
        auto& total = trips[start + ">" + stationName];
        total.first += t - startTime;
        total.second++;
    }

    // S: O(open trips + routes)
    double getAverageTime(const string& startStation, const string& endStation) {
        const auto& total = trips[startStation + ">" + endStation];
        return (double)total.first / total.second;
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
    UndergroundSystem s;
    s.checkIn(1, "A", 0);
    s.checkOut(1, "B", 10);
    check(fabs(s.getAverageTime("A", "B") - 10.0) < 1e-9, "one-trip");
    s.checkIn(2, "A", 5);
    s.checkIn(3, "B", 6);
    s.checkOut(3, "A", 9);
    s.checkOut(2, "B", 20);
    check(fabs(s.getAverageTime("A", "B") - 12.5) < 1e-9, "average");
    check(fabs(s.getAverageTime("B", "A") - 3.0) < 1e-9, "directional");
    s.checkIn(1, "A", 30);
    s.checkOut(1, "B", 31);
    check(fabs(s.getAverageTime("A", "B") - 26.0 / 3) < 1e-9, "customer-reuse");
}
