// Cloudflare (JD): Longest Prefix Match (Medium). Pattern: binary trie on address bits.
// A routing table maps IPv4 CIDR prefixes "a.b.c.d/len" to a next-hop id (len 0..32).
// lookup("a.b.c.d") returns the next hop of the longest prefix that matches the address,
// or -1 if no prefix matches.

#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <memory>
#include <cstdint>

using std::cout;
using std::make_unique;
using std::pair;
using std::stoi;
using std::string;
using std::uint32_t;
using std::unique_ptr;
using std::vector;

// "a.b.c.d" -> 32-bit address with a in the top byte
uint32_t parseIp(const string& addr) {
    uint32_t ip = 0;
    uint32_t octet = 0;
    for (char c : addr) {
        if (c == '.') {
            ip = (ip << 8) | octet;
            octet = 0;
        } else {
            octet = octet * 10 + (c - '0');
        }
    }
    return (ip << 8) | octet;
}

struct Node {
    // child[bit] = subtree for the next address bit, null if none
    unique_ptr<Node> child[2];
    // next hop of the prefix ending exactly here, -1 if none
    int nextHop = -1;
};

// This is a binary trie on address bits, most significant first: insert walks len bits,
// lookup walks the address and keeps the deepest next hop seen. T: O(32) per op,
// S: O(32 * prefixes).
class Router {
    // the /0 prefix
    Node root;

public:
    void insert(const string& cidr, int nextHop) {
        int slash = cidr.find('/');
        uint32_t ip = parseIp(cidr.substr(0, slash));
        int len = stoi(cidr.substr(slash + 1));
        Node* cur = &root;
        for (int depth = 0; depth < len; depth++) {
            int bit = (ip >> (31 - depth)) & 1;
            if (!cur->child[bit]) { cur->child[bit] = make_unique<Node>(); }
            cur = cur->child[bit].get();
        }
        cur->nextHop = nextHop;
    }

    int lookup(const string& addr) {
        uint32_t ip = parseIp(addr);
        Node* cur = &root;
        int best = root.nextHop;
        for (int depth = 0; depth < 32; depth++) {
            int bit = (ip >> (31 - depth)) & 1;
            cur = cur->child[bit].get();
            if (cur == nullptr) { break; }
            if (cur->nextHop != -1) { best = cur->nextHop; }
        }
        return best;
    }
};

void test(const string& label, const vector<pair<string, int>>& table, const string& addr,
          int want) {
    Router router;
    for (const auto& [cidr, nextHop] : table) {
        router.insert(cidr, nextHop);
    }
    cout << label << (router.lookup(addr) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    vector<pair<string, int>> table = {{"0.0.0.0/0", 0}, {"10.0.0.0/8", 1},
                                       {"10.16.0.0/12", 2}};
    test("longest-wins", table, "10.20.1.1", 2);
    test("mid-octet-boundary", table, "10.32.0.1", 1);
    test("default-route", table, "8.8.8.8", 0);
    test("no-match", {{"10.0.0.0/8", 1}}, "8.8.8.8", -1);
}

// follow-up: faster lookups -> multibit trie (stride 8: 4 steps) or path-compressed trie;
//   alternative: 33 hash maps (one per length), probe /32 down to /0. IPv6 -> 128 bits.
