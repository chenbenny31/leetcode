// One package for all products: products and packages are [length, width, height], no
// rotation, touching allowed. Is there one package that can hold each product (tested
// one at a time)? No products -> any package works; no packages -> false.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;

// A package fits every product iff it is at least the per-dimension maximum over all
// products. T: O(p + b), S: O(1).
bool canFitAllProducts(const vector<vector<int>>& products,
                       const vector<vector<int>>& packages) {
    // need[d] = largest size of dimension d over all products
    vector<int> need(3, 0);
    for (const auto& p : products) {
        for (int d = 0; d < 3; d++) { need[d] = max(need[d], p[d]); }
    }
    for (const auto& box : packages) {
        if (box[0] >= need[0] && box[1] >= need[1] && box[2] >= need[2]) { return true; }
    }
    return false;
}

void test(const string& label, const vector<vector<int>>& products,
          const vector<vector<int>>& packages, bool want) {
    bool ok = canFitAllProducts(products, packages) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("second-box", {{1, 2, 3}, {3, 1, 1}}, {{3, 2, 2}, {3, 2, 3}}, true);
    test("each-fits-one", {{3, 1, 1}, {1, 3, 1}}, {{3, 1, 1}, {1, 3, 1}}, false);
    test("no-products", {}, {{1, 1, 1}}, true);
    test("no-packages", {{1, 1, 1}}, {}, false);
}
