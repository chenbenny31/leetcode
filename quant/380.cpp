// hash-set + tmp-vector + random -> O(n) removal

// hash-map + vector + swap-with-last removal -> O(1) removal

#include <vector>
#include <random> // std::mt19937, std::random_device, std::uniform_int_distribution
#include <unordered_map>

class RandomizedSet {
private:
    std::vector<int> vals; // vals.size() -> next idx of input val
    std::unordered_map<int, int> idxOf; // idxOf provies O(1) lookup from val to idx
    std::mt19937 rng; // getRandom() applies on vals

public:
    RandomizedSet() : rng(std::random_device{}()) {
        idxOf.max_load_factor(0.25f); 
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
        // replace idx with lastVal and erase/pop_back in vec/map
        vals[idx] = lastVal;
        idxOf[lastVal] = idx;
        vals.pop_back();
        idxOf.erase(val);
        return true;
    }

    int getRandom() {
        std::uniform_int_distribution<int> dist(0, static_cast<int>(vals.size()) - 1);
        return vals[dist(rng)];
    }
};
