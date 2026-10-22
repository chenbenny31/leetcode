// LC 1236 Web Crawler: starting from startUrl, use htmlParser.getUrls(url) to follow
// links, visiting only urls with the same hostname as startUrl (urls look like
// "http://host/path"). Return every url reached, in any order.
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <map>
#include <unordered_set>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::map;
using std::unordered_set;
using std::sort;

// test-local fake of the judge's interface; links[url] = urls on that page
struct HtmlParser {
    map<string, vector<string>> links;
    vector<string> getUrls(string url) {
        return links[url];
    }
};

// "http://host/path" -> "host"
string hostOf(const string& url) {
    int begin = 7;
    int end = begin;
    while (end < (int)url.size() && url[end] != '/') { end++; }
    return url.substr(begin, end - begin);
}

// BFS from startUrl, skipping other hosts and urls already queued.
// T: O(V+E), S: O(V); V = urls on the host.
vector<string> crawl(string startUrl, HtmlParser htmlParser) {
    string host = hostOf(startUrl);
    unordered_set<string> seen = {startUrl};   // urls already queued
    vector<string> found = {startUrl};         // result, in discovery order
    queue<string> q;                           // urls whose links are not read yet
    q.push(startUrl);
    while (!q.empty()) {
        string cur = q.front();
        q.pop();
        for (const string& nxt : htmlParser.getUrls(cur)) {
            if (hostOf(nxt) != host) { continue; }
            if (!seen.insert(nxt).second) { continue; }
            found.push_back(nxt);
            q.push(nxt);
        }
    }
    return found;
}

void test(const string& label, const string& startUrl, const HtmlParser& parser,
          vector<string> want) {
    vector<string> got = crawl(startUrl, parser);
    sort(got.begin(), got.end());
    sort(want.begin(), want.end());
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    HtmlParser parser;
    parser.links = {
        {"http://a.com", {"http://a.com/x", "http://b.com", "http://a.com/y"}},
        {"http://a.com/x", {"http://a.com", "http://a.com/z"}},
        {"http://a.com/y", {"http://a.com/z"}},
        {"http://b.com", {"http://a.com/hidden"}}};
    test("same-host-and-cycle", "http://a.com", parser,
         {"http://a.com", "http://a.com/x", "http://a.com/y", "http://a.com/z"});
    test("no-links", "http://a.com/z", parser, {"http://a.com/z"});
    test("other-host-start", "http://b.com", parser, {"http://b.com"});
}
