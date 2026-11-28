// 30 · Linear regression on every range — reported as one line (research phone screen)
// x, y of length n; least-squares fit y = alpha + beta * x on contiguous ranges [l, r]
// all O(n^2) ranges must take O(n^2) in total -> O(1) per range
//
// clarify:
// which ranges -> every contiguous [l, r]; a fixed window of size k is the follow-up
// return -> (alpha, beta) per range
// all x equal in the range (incl. length 1) -> slope undefined; beta = 0, alpha = mean y
// values -> doubles; exact match never expected, compare with a tolerance

#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <cmath>

using std::cout;
using std::string;
using std::vector;
using std::pair;
using std::fabs;

// core idea: prefix sums of x, y, x * x, x * y -> the 4 sums of any range in O(1)
// beta = (k * Sxy - Sx * Sy) / (k * Sxx - Sx * Sx), alpha = (Sy - beta * Sx) / k
// T: O(n) build, O(1) per range, S: O(n)
class RangeRegression {
    // prefixX[i] = x[0] + .. + x[i - 1]; the other three are the same for y, x*x, x*y
    vector<double> prefixX;
    vector<double> prefixY;
    vector<double> prefixXX;
    vector<double> prefixXY;

public:
    RangeRegression(const vector<double>& x, const vector<double>& y) {
        int n = x.size();
        prefixX.assign(n + 1, 0);
        prefixY.assign(n + 1, 0);
        prefixXX.assign(n + 1, 0);
        prefixXY.assign(n + 1, 0);
        for (int i = 0; i < n; i++) {
            prefixX[i + 1] = prefixX[i] + x[i];
            prefixY[i + 1] = prefixY[i] + y[i];
            prefixXX[i + 1] = prefixXX[i] + x[i] * x[i];
            prefixXY[i + 1] = prefixXY[i] + x[i] * y[i];
        }
    }

    // (alpha, beta) of the least-squares line on [l, r], both inclusive
    pair<double, double> fit(int l, int r) const {
        double k = r - l + 1;
        double sumX = prefixX[r + 1] - prefixX[l];
        double sumY = prefixY[r + 1] - prefixY[l];
        double sumXX = prefixXX[r + 1] - prefixXX[l];
        double sumXY = prefixXY[r + 1] - prefixXY[l];
        // denom = k^2 * variance of x; 0 when every x in the range is equal
        double denom = k * sumXX - sumX * sumX;
        double beta = 0;
        if (fabs(denom) > 1e-12) { beta = (k * sumXY - sumX * sumY) / denom; }
        double alpha = (sumY - beta * sumX) / k;
        return {alpha, beta};
    }
};

void test(const string& label, const vector<double>& x, const vector<double>& y, int l,
          int r, double wantAlpha, double wantBeta) {
    RangeRegression reg(x, y);
    auto [alpha, beta] = reg.fit(l, r);
    bool ok = fabs(alpha - wantAlpha) < 1e-9 && fabs(beta - wantBeta) < 1e-9;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("exact-line", {0, 1, 2, 3}, {1, 3, 5, 7}, 0, 3, 1, 2);
    test("sub-range", {0, 1, 2, 3}, {1, 3, 5, 7}, 2, 3, 1, 2);
    test("peak-is-flat", {0, 1, 2}, {0, 3, 0}, 0, 2, 1, 0);
    test("peak-left-half", {0, 1, 2}, {0, 3, 0}, 0, 1, 0, 3);
    test("same-x", {2, 2}, {1, 3}, 0, 1, 2, 0);
}

// follow-up:
// 1. all ranges -> two loops over l <= r calling fit: O(n^2)
// 2. fixed window k -> slide: add the new point's 4 terms, subtract the old one's; O(n)
// 3. precision with large values -> subtract the means first, or use long double
// 4. r^2 too -> one more prefix sum of y * y
// 5. streaming points -> keep the 4 running sums + count; O(1) per update
