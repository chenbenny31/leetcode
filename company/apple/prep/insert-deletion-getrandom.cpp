// RandomizedSet with O(1) insert/remove/getRand

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <cstdlib>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::rand;

class RandomizedSet {
private:
    vector<int> vals;
    unordered_map<int, int> indices; // val -> its-pos

public:
    // append and record the pos, T: O(1)
    bool insert(int val) {
        if (indices.count(val)) { return false; }
        indices[val] = vals.size();
        vals.push_back(val);
        return true;
    }

    // move last elem into val's slot, then pop back, T: O(1)
    bool remove(int val) {
        auto it = indices.find(val);
        if (it == indices.end()) { return false; }
        int lastVal = vals.back();
        vals[it->second] = lastVal;
        indices[lastVal] = it->second; // val could be lastVal, write before erase
        vals.pop_back();
        indices.erase(val);
        return true;
    }
    
    // random postion pick up, T: O(1)
    int getRandom() {
        return vals[rand() % vals.size()];
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    RandomizedSet s;
    check(s.insert(1) && s.insert(2) && !s.insert(1), "insert");
    check(!s.remove(3) && s.remove(1), "remove");
    check(s.getRandom() == 2, "random");
    check(s.remove(2) && s.insert(2), "last");
}
