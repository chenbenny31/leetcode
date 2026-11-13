// Horizontal Line Bisecting Squares (LC 3453 Separate Squares I; Google report)
// (1) squares[i] = {x, y, side}; lowest y = h w/ equal total area above / below (1e-5 ok)
// (2) LC 3454: overlapping area counts once (union)
//
// clarify:
// value ranges -> side * side and y + side overflow int
// overlaps -> per square (1) or union (2)
// flat gap at half -> return the lowest h
// n >= 1 -> no empty input

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
using std::unique;
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

// (1) solution 1: binary search on h; simplest
// core idea: areaBelow(h) is continuous, non-decreasing -> lowest h w/ area >= total / 2
// T: O(n * 100), S: O(1) — fixed 100 halvings
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

// (1) solution 2: sweep; exact answer, no iteration cnt
// core idea: sweep edges upward; between edges area grows linearly, slope = active width
// T: O(n log n), S: O(n)
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
    // sum of widths of the squares crossing the current y
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

// union len of the x-intervals of the squares covering the strip that starts at y
long long unionWidth(const vector<vector<int>>& squares, long long y) {
    // (start, end) of each covering square on the x axis
    vector<pair<long long, long long>> spans;
    for (const vector<int>& square : squares) {
        long long bottom = square[1];
        long long top = bottom + square[2];
        if (bottom > y || y >= top) { continue; }
        long long left = square[0];
        spans.push_back({left, left + square[2]});
    }
    if (spans.empty()) { return 0; }
    sort(spans.begin(), spans.end());
    long long width = 0;
    // right end of the merged union so far
    long long reach = spans[0].first;
    for (const auto& [start, end] : spans) {
        long long from = max(start, reach);
        if (end > from) { width += end - from; }
        reach = max(reach, end);
    }
    return width;
}

// (2) union area (LC 3454)
// core idea: cut y at every edge; union width is constant in a strip -> same linear solve
// T: O(n^2 log n), S: O(n)
double bisectUnion(const vector<vector<int>>& squares) {
    // every bottom and top edge, sorted, dups removed
    vector<long long> ys;
    for (const vector<int>& square : squares) {
        ys.push_back(square[1]);
        ys.push_back((long long)square[1] + square[2]);
    }
    sort(ys.begin(), ys.end());
    ys.erase(unique(ys.begin(), ys.end()), ys.end());
    int strips = (int)ys.size() - 1;
    // widths[k] = union width on the strip [ys[k], ys[k + 1])
    vector<long long> widths(strips);
    double total = 0;
    for (int k = 0; k < strips; k++) {
        widths[k] = unionWidth(squares, ys[k]);
        total += (double)widths[k] * (ys[k + 1] - ys[k]);
    }
    double half = total / 2;
    double covered = 0;
    for (int k = 0; k < strips; k++) {
        double strip = (double)widths[k] * (ys[k + 1] - ys[k]);
        if (covered + strip >= half) { return ys[k] + (half - covered) / widths[k]; }
        covered += strip;
    }
    return ys.back();
}

void test(const string& label, const vector<vector<int>>& squares, double want) {
    bool ok = fabs(bisectSearch(squares) - want) < 1e-5 &&
              fabs(bisectSweep(squares) - want) < 1e-5;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void testUnion(const string& label, const vector<vector<int>>& squares, double want) {
    bool ok = fabs(bisectUnion(squares) - want) < 1e-5;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("one-square", {{0, 0, 2}}, 1.0);
    test("stacked", {{0, 0, 1}, {0, 1, 1}}, 1.0);
    test("gap-lowest", {{0, 0, 1}, {2, 2, 1}}, 1.0);
    test("overlap", {{0, 0, 2}, {1, 1, 1}}, 7.0 / 6);

    testUnion("union-inside", {{0, 0, 2}, {1, 1, 1}}, 1.0);
    testUnion("union-partial", {{0, 0, 2}, {1, 1, 2}}, 1.5);
    testUnion("union-side-by-side", {{0, 0, 2}, {2, 0, 2}}, 1.0);
    testUnion("union-gap", {{0, 0, 1}, {5, 2, 1}}, 1.0);
}

// follow-up:
// LC 3454 in O(n log n) -> segment tree over compressed x keeps covered len in the sweep
// vertical line instead -> swap the roles of x and y
// rectangles {x, y, w, h} -> events {y, +w} and {y + h, -w}, total += w * h
