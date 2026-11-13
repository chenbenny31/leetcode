// Horizontal Line Bisecting Squares (LC 3453 Separate Squares I; Google, interview
// report). squares[i] = {x, y, side}, axis-aligned, bottom-left corner (x, y),
// side > 0. Find the lowest y = h such that the total square area above the line equals
// the area below. Overlapping areas count once per square. Answers within 1e-5 pass.
//
// Clarify:
// - ranges? -> n up to ~5e4, x, y, side up to ~1e9 (LC: total area <= 1e12): side *
//   side overflows int -> areas in double; y + side overflows int -> cast first
// - overlap -> counted once per square (stated); the union is LC 3454 (follow-up)
// - "lowest" -> between separated squares the area is flat over a range of h: return its
//   bottom end
// - only y and side matter here (x matters only for the union)
// - empty input? -> assume n >= 1
// Idea: areaBelow(h) is continuous and non-decreasing, so binary search the lowest h
//   with areaBelow(h) >= total / 2 (O(n) per step, 100 steps). Exact and faster: sweep
//   the bottom / top edges upward; between two edges the area grows linearly with slope
//   = sum of the active sides, so solve inside the segment that crosses half.
// Pitfalls:
// - search for the LOWEST h: test >= half and move hi, return hi (in a flat gap every h
//   has the same area; returning lo or mid can land anywhere in the gap)
// - a fixed 100 iterations, not while (hi - lo > eps): near 1e9 doubles are ~1e-7 apart,
//   so a tiny eps never ends
// - sweep: add the segment [prevY, y) with the width BEFORE this event's change; a gap
//   (width 0) never divides by zero because covered < half there
// - compare doubles with fabs(got - want) < 1e-5, never ==
// Tests to write: one square -> its middle; two stacked; a gap -> the bottom of the flat
//   range; overlapping squares counted twice.

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

// union length of the x-intervals [x, x + side) of the squares covering the strip that
// starts at y (bottom <= y < top). Sort by start; each interval adds the part right of
// everything merged so far. T: O(n log n), S: O(n).
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

// follow-up (LC 3454 Separate Squares II): overlaps count once. Cut y at every edge;
// inside a strip the union width is constant, so the area grows linearly as in
// bisectSweep. Union width per strip by merging x-intervals.
// Pitfall: x + side and y + side overflow int (2e9): widen to long long first.
// T: O(n^2 log n), S: O(n); a segment tree over compressed x makes it O(n log n).
double bisectUnion(const vector<vector<int>>& squares) {
    // every bottom and top edge, sorted, duplicates removed
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

// follow-up: overlaps counted once (LC 3454 Separate Squares II) -> bisectUnion above
// follow-up: vertical line instead -> swap the roles of x and y: pass squares with
//   square[0] and square[1] swapped
// follow-up: rectangles {x, y, w, h} -> the same code with width and height split:
//   double inside = min(max(mid - rect[1], 0.0), (double)rect[3]);
//   area += rect[2] * inside;  total += (double)rect[2] * rect[3];
//   sweep events: {y, +w} and {y + h, -w}
