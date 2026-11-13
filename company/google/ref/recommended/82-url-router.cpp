// URL Router (Google, interview report). addHandler(pattern) registers a pattern such as
// "/users/{userId}/posts"; getHandler(url) returns the registered pattern matching url,
// or "" if none. Paths are split on '/' and empty pieces are dropped (so a trailing '/'
// is ignored). A literal segment matches only itself; {name} matches exactly one
// non-empty segment. If several patterns match, a literal beats {name} at the leftmost
// segment where they differ. Assume no two patterns differ only in placeholder names.
//
// Clarify:
// - trailing or doubled '/' -> ignored (empty pieces dropped, stated)
// - {name} matches exactly one non-empty segment; "/" matches only the root pattern
// - several matches -> literal beats {name} at the leftmost differing segment (stated)
// - no match -> ""; segment counts must be equal (no prefix match)
// Idea: a trie over segments: literal children in a hash map plus one param child. The
//   lookup DFS tries the literal child first, then the param child, and backtracks when
//   the literal branch dead-ends deeper; that gives the leftmost-literal priority.
// Pitfalls:
// - backtrack: "/users/me/posts" falls back to {userId} when "me" has no "posts"
// - a node matches only if a pattern ends there (pattern != ""), not any node on the path
// - nodes.push_back may reallocate: hold node indices, never a TrieNode& across it
// - every {name} at a node shares the one param child (names are only labels)
// Tests to write: literal beats param; backtrack out of a literal branch; no match (too
//   short, too long, other prefix); the root pattern "/".

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;

// non-empty pieces of path between '/'
vector<string> split(const string& path) {
    vector<string> parts;
    string part;
    for (char c : path) {
        if (c != '/') {
            part += c;
            continue;
        }
        if (!part.empty()) { parts.push_back(part); }
        part.clear();
    }
    if (!part.empty()) { parts.push_back(part); }
    return parts;
}

bool isParam(const string& segment) { return segment.front() == '{'; }

// straightforward: compare the url with every pattern segment by segment; among matches
// keep the one whose literal / param signature ('L' < 'P') is smallest.
// T: add O(L), get O(P L) for P patterns of L segments, S: O(P L).
class RouterScan {
    // registered patterns
    vector<string> patterns;

public:
    void addHandler(const string& pattern) { patterns.push_back(pattern); }

    string getHandler(const string& url) {
        vector<string> parts = split(url);
        string best;
        string bestSignature;
        for (const string& pattern : patterns) {
            vector<string> segments = split(pattern);
            if (segments.size() != parts.size()) { continue; }
            string signature;
            bool match = true;
            for (int i = 0; i < (int)parts.size() && match; i++) {
                if (isParam(segments[i])) {
                    signature += 'P';
                } else {
                    signature += 'L';
                    match = segments[i] == parts[i];
                }
            }
            if (match && (best.empty() || signature < bestSignature)) {
                best = pattern;
                bestSignature = signature;
            }
        }
        return best;
    }
};

struct TrieNode {
    // literal segment -> child node index
    unordered_map<string, int> children;
    // child node index for a {name} segment, -1 if none
    int param = -1;
    // pattern ending at this node, "" if none
    string pattern;
};

// improved: trie over segments. Matching is a DFS that tries the literal child first,
// then the param child, backtracking when the literal branch fails deeper.
// T: add O(L), get O(L) without backtracking, O(trie nodes) worst case, S: O(total L).
class Router {
    // node pool, node 0 = root (the pattern "/")
    vector<TrieNode> nodes = {TrieNode()};

    // pattern matching parts[pos..] from node, "" if none
    string dfs(const vector<string>& parts, int pos, int node) {
        if (pos == (int)parts.size()) { return nodes[node].pattern; }
        auto it = nodes[node].children.find(parts[pos]);
        if (it != nodes[node].children.end()) {
            string found = dfs(parts, pos + 1, it->second);
            if (!found.empty()) { return found; }
        }
        if (nodes[node].param == -1) { return ""; }
        return dfs(parts, pos + 1, nodes[node].param);
    }

public:
    void addHandler(const string& pattern) {
        int node = 0;
        for (const string& segment : split(pattern)) {
            int child = -1;
            if (isParam(segment)) {
                child = nodes[node].param;
            } else if (nodes[node].children.count(segment)) {
                child = nodes[node].children[segment];
            }
            if (child == -1) {
                child = nodes.size();
                // push_back may reallocate, so index nodes again afterwards
                nodes.push_back(TrieNode());
                if (isParam(segment)) {
                    nodes[node].param = child;
                } else {
                    nodes[node].children[segment] = child;
                }
            }
            node = child;
        }
        nodes[node].pattern = pattern;
    }

    string getHandler(const string& url) { return dfs(split(url), 0, 0); }
};

void test(const string& label, const vector<string>& patterns,
          const vector<string>& urls, const vector<string>& want) {
    RouterScan scan;
    Router trie;
    for (const string& pattern : patterns) {
        scan.addHandler(pattern);
        trie.addHandler(pattern);
    }
    bool ok = true;
    for (int i = 0; i < (int)urls.size(); i++) {
        ok = ok && scan.getHandler(urls[i]) == want[i];
        ok = ok && trie.getHandler(urls[i]) == want[i];
    }
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("literal-wins", {"/users/{userId}/posts", "/users/me/posts"},
         {"/users/42/posts", "/users/me/posts"},
         {"/users/{userId}/posts", "/users/me/posts"});
    test("backtrack", {"/users/me/settings", "/users/{userId}/posts"},
         {"/users/me/posts", "/users/me/settings"},
         {"/users/{userId}/posts", "/users/me/settings"});
    test("no-match", {"/users/{userId}"},
         {"/users", "/users/", "/users/42/posts", "/orders/42", "/users/42/"},
         {"", "", "", "", "/users/{userId}"});
    test("root", {"/", "/health"}, {"/", "/health", "/healthz"}, {"/", "/health", ""});
}

// follow-up: return parameter values -> store the name at each param node; in dfs, on
//   the param branch set params[name] = parts[pos] (erase it if that branch fails)
// follow-up: '*' matches the rest of the path (one or more segments) -> a catchAll
//   pattern per node; in dfs, after literal and param fail, return it if pos < size
// follow-up: HTTP methods -> one pattern per method at the end node
//   (unordered_map<string, string> handlerOf instead of a single pattern)
