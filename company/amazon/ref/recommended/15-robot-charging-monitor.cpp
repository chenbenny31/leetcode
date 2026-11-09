// Robots report their battery level (%) through an existing API, and monitor(level) is
// called several times per second with the levels read from it. A charging robot above
// 95% leaves its station and returns to work; a working robot below 50% takes a free
// station. Stations are limited, so robots that find none wait for a later call (lowest
// battery served first).

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::sort;

// Release first, so a freed station can be reused in the same call.
// T: O(r log r) per call for r robots, S: O(r + s).
class ChargingMonitor {
    // station[r] = station robot r charges at, -1 while working
    vector<int> station;
    // ids of free stations
    vector<int> freeStations;

public:
    ChargingMonitor(int robots, int stations) : station(robots, -1) {
        for (int s = stations - 1; s >= 0; s--) { freeStations.push_back(s); }
    }

    // level[r] = robot r's battery, read once per call so one call sees consistent data
    void monitor(const vector<int>& level) {
        int n = station.size();
        for (int r = 0; r < n; r++) {
            if (station[r] != -1 && level[r] > 95) {
                freeStations.push_back(station[r]);
                station[r] = -1;
            }
        }
        vector<int> needy;
        for (int r = 0; r < n; r++) {
            if (station[r] == -1 && level[r] < 50) { needy.push_back(r); }
        }
        sort(needy.begin(), needy.end(),
             [&](int a, int b) { return level[a] < level[b]; });
        for (int r : needy) {
            if (freeStations.empty()) { break; }
            station[r] = freeStations.back();
            freeStations.pop_back();
        }
    }

    int stationOf(int robot) const { return station[robot]; }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    vector<int> level = {40, 30, 80};
    ChargingMonitor mon(3, 1);
    mon.monitor(level);
    check(mon.stationOf(1) == 0 && mon.stationOf(0) == -1, "lowest-first");
    level[1] = 96;
    mon.monitor(level);
    check(mon.stationOf(1) == -1 && mon.stationOf(0) == 0, "release-reuse");
    level[0] = 95;
    mon.monitor(level);
    check(mon.stationOf(0) == 0, "not-full");
    level[2] = 49;
    mon.monitor(level);
    check(mon.stationOf(2) == -1, "no-station");
}
