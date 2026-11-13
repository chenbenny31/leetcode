// URL Router (Google, interview report)
// addHandler(pattern) registers a pattern such as "/users/{userId}/posts"
// getHandler(url) returns the matching pattern, "" if none
// reported: x2 in the 2026 Q4 list
//
// clarify:
// trailing / doubled '/' -> split on '/', drop empty pieces
// what {name} matches -> exactly one segment; a literal matches itself
// several matches -> literal beats {name} at the leftmost difference
// prefix match -> no, segment cnt must be equal
// patterns differing only in names -> assume none (scan keeps first, trie keeps last)

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

// solution 1: scan all patterns; fine for few patterns
// core idea: compare url w/ every pattern segment by segment
// among matches keep the smallest literal / param signature ('L' < 'P')
// T: add O(L), get O(P * L), S: O(P * L) — P patterns of L segments
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
    // literal segment -> child node idx
    unordered_map<string, int> children;
    // child node idx for a {name} segment, -1 if none
    int param = -1;
    // pattern ending at this node, "" if none
    string pattern;
};

// solution 2: trie over segments; get does not depend on num patterns
// core idea: node = map of literal children + one param child
// lookup DFS tries literal child first, then param; backtracks if literal fails deeper
// T: add O(L), get O(L), S: O(total L) — get O(trie nodes) worst w/ backtracking
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
                // push_back may reallocate -> index nodes again afterwards
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

// follow-up:
// return param values -> name at each param node; dfs sets params[name] = parts[pos]
// param branch fails -> erase params[name]
// '*' matches the rest of the path (>= 1 segments) -> a catchAll pattern per node
// '*' lookup -> after literal and param fail, return catchAll if segments remain
// HTTP methods -> map method -> pattern at the end node instead of a single pattern
