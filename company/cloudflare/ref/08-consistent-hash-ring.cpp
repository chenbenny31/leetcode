// Cloudflare (JD): Consistent Hash Ring (Medium). Pattern: ordered map + lower_bound.
// Each cache server owns V virtual nodes at ring positions hash(server + "#" + replica),
// hash = FNV-1a 32-bit. getServer(key) = owner of the first position >= hash(key),
// wrapping to the smallest position; "" if the ring is empty. addServer / removeServer.

#include <iostream>
#include <string>
#include <map>
#include <cstdint>

using std::cout;
using std::map;
using std::string;
using std::to_string;
using std::uint32_t;

// FNV-1a 32-bit: deterministic, easy to type
uint32_t fnv1a(const string& s) {
    uint32_t h = 2166136261u;
    for (char c : s) {
        h ^= (unsigned char)c;
        h *= 16777619u;
    }
    return h;
}

// This is a sorted map position -> server; lookup is lower_bound plus wrap-around.
// T: add/remove O(V log(S*V)), lookup O(log(S*V)); S: O(S*V) for S servers.
class HashRing {
    // virtual nodes per server
    int replicas;
    // ring position -> server that owns it
    map<uint32_t, string> ring;

public:
    HashRing(int v) : replicas(v) {}

    void addServer(const string& server) {
        for (int replica = 0; replica < replicas; replica++) {
            ring[fnv1a(server + "#" + to_string(replica))] = server;
        }
    }

    void removeServer(const string& server) {
        // assumes no two virtual nodes collide (else this erases another server's node)
        for (int replica = 0; replica < replicas; replica++) {
            ring.erase(fnv1a(server + "#" + to_string(replica)));
        }
    }

    string getServer(const string& key) {
        if (ring.empty()) { return ""; }
        auto it = ring.lower_bound(fnv1a(key));
        // past the largest position: wrap to the smallest
        if (it == ring.end()) { it = ring.begin(); }
        return it->second;
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    HashRing ring(1);
    check(ring.getServer("/index.html") == "", "empty-ring");
    ring.addServer("edge-a");
    ring.addServer("edge-b");
    ring.addServer("edge-c");
    // fnv1a order: edge-b#0 < /index.html < edge-a#0 < edge-c#0 < /img/logo.png
    check(ring.getServer("edge-a#0") == "edge-a" &&
          ring.getServer("/index.html") == "edge-a", "hit-or-next");
    check(ring.getServer("/img/logo.png") == "edge-b", "wrap-around");

    HashRing live(3);
    live.addServer("edge-a");
    live.addServer("edge-b");
    live.removeServer("edge-b");
    check(live.getServer("/img/cat.png") == "edge-a" &&
          live.getServer("/js/app.js") == "edge-a", "add-then-remove");
}

// follow-up: why virtual nodes -> spread each server's arc so load stays even and a
//   removed server's keys go to many servers; weight a server by giving it more replicas.
// follow-up: O(1) lookup, near-perfect balance -> Maglev table; or rendezvous hashing.
