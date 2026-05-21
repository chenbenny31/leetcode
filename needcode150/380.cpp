// hash-map + dynamic-array, T: O(1) avg all ops, S: O(n)

#include <vector>
#include <unordered_map>
#include <cstdlib> // std::rand

class RandomizedSet {
private:
    std::vector<int> vals;
    std::unordered_map<int, int> idx_map; // val -> idx in vals[]

public:
    RandomizedSet() {
        idx_map.reserve(128);
        idx_map.max_load_factor(0.25f);
    }

    bool insert(int val) {
        if (idx_map.count(val)) { return false; }
        idx_map[val] = static_cast<int>(vals.size());
        vals.push_back(val);
        return true;
    }

    bool remove(int val) {
        auto it = idx_map.find(val);
        if (it == idx_map.end()) { return false; }

        int pos = it->second;
        int last_val = vals.back();

        idx_map.erase(it);
        vals.pop_back();
        if (last_val != val) {
            vals[pos] = last_val;
            idx_map[last_val] = pos;
        }
        return true;
    }

    int getRandom() {
        int rand_pos = std::rand() % vals.size();
        return vals[rand_pos];
    }
};

// swap with last for delete in O(1): vals.pop_back() and idx_map.erase(it) are both O(1)
// edge case - delete last element
