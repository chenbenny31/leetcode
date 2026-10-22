// vector + index-map, swap-with-last removal, T: O(1) insert/remove/getRandom, S: O(n)

#include <vector>
#include <unordered_map>
#include <cstdlib> // std::rand, std::srand;
#include <ctime> // std::time

class RandomizedSet {
private:
    std::vector<int> vals;
    std::unordered_map<int, int> idxOf; // val -> idx

public:
    RandomizedSet() {
        idxOf.max_load_factor(0.25f);
        std::srand(static_cast<unsigned>(std::time(nullptr)));
    }

    bool insert(int val) {
        auto [it, inserted] = idxOf.insert({val, static_cast<int>(vals.size())});
        if (!inserted) { return false; }
        vals.push_back(val);
        return true;
    }

    bool remove(int val) {
        auto it = idxOf.find(val);
        if (it == idxOf.end()) { return false; }
        int idx = it->second;
        int lastVal = vals.back();
        vals[idx] = lastVal;
        idxOf[lastVal] = idx; // must precede the erase: case lastVal==val
        vals.pop_back();
        idxOf.erase(val);
        return true;
    }

    int getRandom() {
        return vals[std::rand() % vals.size()];
    }
};
