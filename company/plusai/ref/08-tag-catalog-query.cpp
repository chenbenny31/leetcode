// PlusAI (JD): Tag Catalog Query (Medium). Pattern: inverted index + sorted intersection.
// Recordings have an id and a list of distinct tags. Build an inverted index tag -> ids;
// query(tags) returns the ids that carry ALL the tags, ascending (empty query -> {}).

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <utility>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::pair;
using std::sort;

// This is an inverted index (hash map of sorted id lists) + LC 349 style two-pointer
// intersection, one tag at a time. T: build O(N log N) over all (tag, id) pairs,
// query O(sum of the queried list sizes).
class TagCatalog {
    // tag -> ids of recordings with that tag, ascending
    unordered_map<string, vector<int>> index;

    // ids present in both ascending lists
    vector<int> intersect(const vector<int>& listA, const vector<int>& listB) {
        vector<int> both;
        int posA = 0;
        int posB = 0;
        while (posA < (int)listA.size() && posB < (int)listB.size()) {
            if (listA[posA] < listB[posB]) {
                posA++;
            } else if (listA[posA] > listB[posB]) {
                posB++;
            } else {
                both.push_back(listA[posA]);
                posA++;
                posB++;
            }
        }
        return both;
    }

public:
    TagCatalog(const vector<pair<int, vector<string>>>& recordings) {
        for (const auto& [id, tags] : recordings) {
            for (const string& tag : tags) { index[tag].push_back(id); }
        }
        // ids may arrive in any order
        for (auto& entry : index) { sort(entry.second.begin(), entry.second.end()); }
    }

    vector<int> query(const vector<string>& tags) {
        if (tags.empty()) { return {}; }
        // ids carrying every tag so far; an unknown tag gets an empty list -> {}
        vector<int> result = index[tags[0]];
        for (int i = 1; i < (int)tags.size(); i++) {
            result = intersect(result, index[tags[i]]);
        }
        return result;
    }
};

void test(const string& label, TagCatalog& catalog, const vector<string>& tags,
          const vector<int>& want) {
    cout << label << (catalog.query(tags) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    TagCatalog catalog({{3, {"rain", "night", "highway"}},
                        {1, {"rain", "night"}},
                        {2, {"rain", "highway"}}});
    test("two-tags", catalog, {"rain", "night"}, {1, 3});
    test("three-tags", catalog, {"highway", "rain", "night"}, {3});
    test("one-tag-sorted", catalog, {"rain"}, {1, 2, 3});
    test("unknown-tag", catalog, {"rain", "snow"}, {});
}

// follow-up: one tiny list vs huge ones -> start from the shortest list and binary search
//   each of its ids in the others, O(small * log big); at scale use Roaring bitmaps
// follow-up: OR / NOT queries -> union / difference with the same two-pointer merge
