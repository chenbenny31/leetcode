// ByteDance (FastPrep, phone / onsite): implement the forward pass of multi-head
// scaled dot-product attention with no learned projections. query, key, value are
// row-major integer matrices whose rows all have width d; key and value have the same
// number of rows; heads divides d. Split every row into heads contiguous slices of
// width d / heads. For each query row and each head:
//   1. dot the query slice with every key slice,
//   2. divide each score by sqrt(d / heads),
//   3. take a numerically stable softmax over the keys,
//   4. return the probability-weighted sum of the value slices.
// Concatenate the heads in order. Return one comma-separated string per query row, each
// coordinate with 6 decimals. No mask, bias, dropout or backward pass.
// (The original report only said "hand-write multi-head attention"; this is the
// FastPrep adaptation.)

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <cstdio>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::exp;
using std::sqrt;
using std::fabs;
using std::max;
using std::snprintf;

string format6(double x) {
    // avoid printing "-0.000000" for tiny negative rounding noise
    if (fabs(x) < 5e-7) {
        x = 0.0;
    }
    char buf[64];
    snprintf(buf, sizeof(buf), "%.6f", x);
    return string(buf);
}

// Every query row looks at every key row once per head, over a slice of width dh, and
// heads * dh = d. T: O(q * k * d), S: O(k) for one head's scores.
vector<string> multiHeadAttention(const vector<vector<int>>& query,
                                  const vector<vector<int>>& key,
                                  const vector<vector<int>>& value, int heads) {
    int d = query[0].size();
    int dh = d / heads;
    int numKeys = key.size();
    double scale = sqrt((double)dh);
    vector<string> result;

    for (const auto& q : query) {
        string row;
        for (int h = 0; h < heads; h++) {
            int lo = h * dh;

            // scores[j] = (query slice . key slice j) / sqrt(dh)
            vector<double> scores(numKeys);
            double best = -1e300;
            for (int j = 0; j < numKeys; j++) {
                double dot = 0;
                for (int c = lo; c < lo + dh; c++) {
                    dot += (double)q[c] * key[j][c];
                }
                scores[j] = dot / scale;
                best = max(best, scores[j]);
            }

            // stable softmax: subtracting the max keeps exp() from overflowing
            double total = 0;
            for (int j = 0; j < numKeys; j++) {
                scores[j] = exp(scores[j] - best);
                total += scores[j];
            }

            // output slice = sum over keys of probability * value slice
            for (int c = lo; c < lo + dh; c++) {
                double out = 0;
                for (int j = 0; j < numKeys; j++) {
                    out += scores[j] / total * value[j][c];
                }
                if (!row.empty()) {
                    row += ",";
                }
                row += format6(out);
            }
        }
        result.push_back(row);
    }
    return result;
}

void test(const string& label, const vector<vector<int>>& query,
          const vector<vector<int>>& key, const vector<vector<int>>& value, int heads,
          const vector<string>& want) {
    bool ok = multiHeadAttention(query, key, value, heads) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("uniform", {{0}}, {{1}, {2}}, {{2}, {4}}, 1, {"3.000000"});
    test("two-heads", {{0, 0}}, {{5, 1}, {7, 3}}, {{1, 10}, {3, 20}}, 2,
         {"2.000000,15.000000"});
    test("peaked", {{100}}, {{1}, {0}}, {{7}, {-7}}, 1, {"7.000000"});
    test("two-rows", {{0}, {100}}, {{1}, {0}}, {{4}, {-4}}, 1,
         {"0.000000", "4.000000"});
}
