// learned projection? Q,K,V are given directly
// shapes? Q=q*d, K and V=k*d, heads dev d, each head works on width, dh = d/heads
// how to split head? contiguous col slices: head h use cols[h*dh, (h+1)*dh)
// mask, bias, dropout: none, before softmax
// output format: concat heads per query row, return 2d double array
// numerics: stable softmax, subtract the row max before `exp`

// attention: weighted avg by similarity - key, value, query
// attention let query comp each key, then return the weighted avg of values
// score_j = q * k_j -> score_j /= sqrt(dh) -> p_j = e^{score_j} / sum(e^{score}) -> out = sum(p_j * v_j)
// softmax(x) = softmax(x - c), prevent exp overflow

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <sstream>
#include <iomanip>

using std::cout;
using std::string;
using std::vector;
using std::exp;
using std::sqrt;
using std::fabs;
using std::max;
using std::ostringstream;
using std::fixed;
using std::setprecision;


// for each query row and head: scaled dot with every key
// stable softmax then weighted sum of value slices
// TT: O(nq * nk * d), S: O(nk)
vector<vector<double>> multiHeadAttention(const vector<vector<double>>& query,
                                          const vector<vector<double>>& key,
                                          const vector<vector<double>>& value,
                                          int heads) {
    int nq = query.size();
    int nk = key.size();
    int d = query[0].size();
    int dh = d / heads;
    double scale = sqrt(dh); // make softmax not saturate as dh grows
    vector<vector<double>> out(nq, vector<double>(d, 0.0));
    for (int i = 0; i < nq; i++) {
        for (int h = 0; h < heads; h++) {
            int lo = h * dh;
            vector<double> w(nk); // w[j]=weight of key j for query i in this head
            double best = -1e300;
            for (int j = 0; j < nk; j++) {
                double dot = 0;
                for (int c = lo; c < lo + dh; c++) {
                    dot += query[i][c] * key[j][c];
                }
                w[j] = dot / scale;
                best = max(best, w[j]);
            }

            // softmax, subtract the max keep exp from overflow
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
