// ByteDance (FastPrep x4, phone / onsite): multi-head attention forward pass, no learned
// projections and no mask. query is nq x d, key and value are nk x d, heads divides d.
// Head h uses columns [h * dh, (h + 1) * dh) with dh = d / heads and computes
// softmax(Q_h K_h^T / sqrt(dh)) V_h; the heads' outputs sit side by side (nq x d).

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::exp;
using std::sqrt;
using std::fabs;
using std::max;

// For each query row and head: scaled dot products with every key, stable softmax,
// then the weighted sum of value slices. T: O(nq * nk * d), S: O(nk) besides the output.
vector<vector<double>> multiHeadAttention(const vector<vector<double>>& query,
                                          const vector<vector<double>>& key,
                                          const vector<vector<double>>& value,
                                          int heads) {
    int nq = query.size();
    int nk = key.size();
    int d = query[0].size();
    int dh = d / heads;
    // scores are divided by sqrt(dh) so softmax doesn't saturate as dh grows
    double scale = sqrt(dh);
    vector<vector<double>> out(nq, vector<double>(d, 0.0));
    for (int i = 0; i < nq; i++) {
        for (int h = 0; h < heads; h++) {
            int lo = h * dh;
            // w[j] = weight of key j for query i in this head
            vector<double> w(nk);
            double best = -1e300;
            for (int j = 0; j < nk; j++) {
                double dot = 0;
                for (int c = lo; c < lo + dh; c++) { dot += query[i][c] * key[j][c]; }
                w[j] = dot / scale;
                best = max(best, w[j]);
            }
            // softmax; subtracting the max keeps exp from overflowing
            double total = 0;
            for (int j = 0; j < nk; j++) {
                w[j] = exp(w[j] - best);
                total += w[j];
            }
            for (int j = 0; j < nk; j++) {
                for (int c = lo; c < lo + dh; c++) {
                    out[i][c] += w[j] / total * value[j][c];
                }
            }
        }
    }
    return out;
}

void test(const string& label, const vector<vector<double>>& query,
          const vector<vector<double>>& key, const vector<vector<double>>& value,
          int heads, const vector<vector<double>>& want) {
    vector<vector<double>> got = multiHeadAttention(query, key, value, heads);
    bool ok = got.size() == want.size();
    for (int i = 0; ok && i < (int)want.size(); i++) {
        for (int c = 0; c < (int)want[i].size(); c++) {
            if (fabs(got[i][c] - want[i][c]) > 1e-9) { ok = false; }
        }
    }
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("equal-scores-average", {{0}}, {{1}, {2}}, {{2}, {4}}, 1, {{3}});
    test("heads-independent", {{0, 0}}, {{5, 1}, {7, 3}}, {{1, 10}, {3, 20}}, 2,
         {{2, 15}});
    test("large-score-stable", {{1000}}, {{1}, {0}}, {{7}, {-7}}, 1, {{7}});
}
