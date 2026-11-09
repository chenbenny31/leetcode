// Cloudflare (reported): Edge LRU Cache by Bytes (Medium). Pattern: list + hash map LRU.
// A CDN edge caches objects under a BYTE budget. put(key, size) inserts or updates the
// object as most recent, then evicts least recently used objects until total <= capacity;
// an object bigger than capacity is not cached. get(key) -> hit?, and refreshes recency.

#include <iostream>
#include <list>
#include <unordered_map>
#include <string>
#include <utility>

using std::cout;
using std::list;
using std::pair;
using std::string;
using std::unordered_map;

// This is the classic LRU (list + map of list iterators), evicting by a running byte
// total instead of an item count. T: O(1) amortized per op, S: O(objects).
class EdgeCache {
    // objects by recency, front = most recent: (key, size in bytes)
    list<pair<string, int>> order;
    // key -> its node in order
    unordered_map<string, list<pair<string, int>>::iterator> where;
    // byte budget
    long long capacity;
    // bytes of all cached objects
    long long used = 0;

    void remove(const string& key) {
        auto it = where.find(key);
        if (it == where.end()) { return; }
        used -= it->second->second;
        order.erase(it->second);
        where.erase(it);
    }

public:
    EdgeCache(long long cap) : capacity(cap) {}

    bool get(const string& key) {
        auto it = where.find(key);
        if (it == where.end()) { return false; }
        // move the node to the front; iterators stay valid
        order.splice(order.begin(), order, it->second);
        return true;
    }

    void put(const string& key, int size) {
        // an update drops the old copy first, so its bytes are not counted twice
        remove(key);
        // never evict others for an object that can't fit anyway
        if (size > capacity) { return; }
        order.push_front({key, size});
        where[key] = order.begin();
        used += size;
        while (used > capacity) {
            // copy: remove() frees the node that holds this string
            string lruKey = order.back().first;
            remove(lruKey);
        }
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    EdgeCache lru(100);
    lru.put("/a.css", 40);
    lru.put("/b.js", 40);
    lru.get("/a.css");
    lru.put("/c.png", 40);
    check(lru.get("/a.css") && !lru.get("/b.js") && lru.get("/c.png"), "get-refreshes");

    EdgeCache big(100);
    big.put("/a.css", 30);
    big.put("/b.js", 30);
    big.put("/c.png", 30);
    big.put("/hero.mp4", 80);
    check(!big.get("/a.css") && !big.get("/b.js") && !big.get("/c.png") &&
          big.get("/hero.mp4"), "evicts-several");

    EdgeCache upd(100);
    upd.put("/a.css", 40);
    upd.put("/b.js", 40);
    upd.put("/a.css", 70);
    check(upd.get("/a.css") && !upd.get("/b.js"), "update-resizes");

    EdgeCache huge(100);
    huge.put("/a.css", 40);
    huge.put("/iso.img", 500);
    check(huge.get("/a.css") && !huge.get("/iso.img"), "oversized-skipped");
}

// follow-up: one-hit wonders flush the cache -> admit on 2nd request, or segmented LRU.
// follow-up: TTL per object -> store expiry in the node; get() treats expired as a miss.
