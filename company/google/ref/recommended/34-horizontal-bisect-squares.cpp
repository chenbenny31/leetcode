// Horizontal Line Bisecting Squares (LC 3453 Separate Squares I; Google, interview
// report). squares[i] = {x, y, side}, axis-aligned, bottom-left corner (x, y),
// side > 0. Find the lowest y = h such that the total square area above the line equals
// the area below. Overlapping areas count once per square. Answers within 1e-5 pass.

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <cmath>

using std::cout;
using std::string;
using std::vector;
using std::pair;
using std::sort;
using std::min;
using std::max;
using std::fabs;

// total area of the squares below the line y = h
double areaBelow(const vector<vector<int>>& squares, double h) {
    double area = 0;
    for (const vector<int>& square : squares) {
        double side = square[2];
        double inside = min(max(h - square[1], 0.0), side);
        area += side * inside;
    }
    return area;
}

// areaBelow(h) is continuous and non-decreasing: binary search the lowest h with
// areaBelow(h) >= total / 2. T: O(n * 100), S: O(1).
double bisectSearch(const vector<vector<int>>& squares) {
    double lo = 1e18;
    double hi = -1e18;
    double total = 0;
    for (const vector<int>& square : squares) {
        lo = min(lo, (double)square[1]);
        hi = max(hi, (double)square[1] + square[2]);
        total += (double)square[2] * square[2];
    }
    for (int iter = 0; iter < 100; iter++) {
        double mid = (lo + hi) / 2;
        if (areaBelow(squares, mid) >= total / 2) {
            hi = mid;
        } else {
            lo = mid;
        }
    }
    return hi;
}

// sweep the bottom / top edges upward; between two edges the area grows linearly with
// slope = sum of the active widths, so solve the crossing segment exactly.
// T: O(n log n), S: O(n).
double bisectSweep(const vector<vector<int>>& squares) {
    // (y, width change): +side at the bottom edge, -side at the top edge
    vector<pair<long long, long long>> events;
    double total = 0;
    for (const vector<int>& square : squares) {
        long long y = square[1];
        long long side = square[2];
        events.push_back({y, side});
        events.push_back({y + side, -side});
        total += (double)side * side;
    }
    sort(events.begin(), events.end());
    double half = total / 2;
    double covered = 0;
    // sum of the widths of the squares crossing the current y
    long long width = 0;
    long long prevY = events[0].first;
    for (const auto& [y, change] : events) {
        double segment = (double)width * (y - prevY);
        if (covered + segment >= half) { return prevY + (half - covered) / width; }
        covered += segment;
        width += change;
        prevY = y;
    }
    return prevY;
}

void test(const string& label, const vector<vector<int>>& squares, double want) {
    bool ok = fabs(bisectSearch(squares) - want) < 1e-5 &&
              fabs(bisectSweep(squares) - want) < 1e-5;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("one-square", {{0, 0, 2}}, 1.0);
    test("stacked", {{0, 0, 1}, {0, 1, 1}}, 1.0);
    test("gap-lowest", {{0, 0, 1}, {2, 2, 1}}, 1.0);
    test("overlap", {{0, 0, 2}, {1, 1, 1}}, 7.0 / 6);
}

// follow-up: overlaps counted once (LC 3454 Separate Squares II) -> same sweep, but the
//   active width is the union of x-intervals: segment tree over compressed x
// follow-up: vertical line instead -> swap the roles of x and y
// follow-up: rectangles -> same code with height and width split (area = w * h)
