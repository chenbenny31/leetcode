// hash-set, T: O(1) insert/remove, O(1) getRandom, S: O(n)

// vector + index-map, swap-with-last removal, T: O(1) insert/remove/getRandom, S: O(n)

#include <vector>
#include <unordered_map>
#include <random> // std::mt19937, std::random_device, std::uniform_int_distribution

class RandomizedSet {
private:
    std::vector<int> vals;
    std::unordered_map<int, int> idx_map;
    std::mt19937 rng;

public:
    RandomizedSet() : rng(std::random_device{}()) {
        idx_map.max_load_factor(0.25f);
    }

    bool insert(int val) {
        if (idx_map.count(val) > 0) { return false; }
        idx_map[val] = static_cast<int>(vals.size());
        vals.push_back(val);
        return true;
    }

    bool remove(int val) {
        auto it = idx_map.find(val);
        if (it == idx_map.end()) { return false; }
        int idx = it->second;
        int last_val = vals.back();
        vals[idx] = last_val;
        idx_map[last_val] = idx;
        vals.pop_back();
        idx_map.erase(val);
        return true;
    }

    int getRandom() {
        std::uniform_int_distribution<int> dist(0, static_cast<int>(vals.size()) - 1);
        return vals[dist(rng)];
    }
};

// self-removal order issue: idx_map[last_val] = idx before erase(val) to avoid re-insert if last_val==val
// mt19937 + uniform_int_distribution not rand() % n: rand()%n slightly over-represents the low end or range
