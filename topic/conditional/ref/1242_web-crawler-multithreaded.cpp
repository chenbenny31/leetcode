// LC 1242 Web Crawler Multithreaded: starting from startUrl, use htmlParser.getUrls(url)
// to follow links, visiting only urls with the same hostname as startUrl, fetching
// pages concurrently. Return every url reached, in any order.
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <unordered_set>
#include <algorithm>
#include <thread>
#include <mutex>
using std::cout;
using std::string;
using std::vector;
using std::map;
using std::unordered_set;
using std::sort;
using std::thread;
using std::mutex;
using std::lock_guard;

// test-local fake of the judge's interface; links[url] = urls on that page
struct HtmlParser {
    map<string, vector<string>> links;
    vector<string> getUrls(string url) {
        auto it = links.find(url);
        return it == links.end() ? vector<string>() : it->second;
    }
};

// "http://host/path" -> "host"
string hostOf(const string& url) {
    int begin = 7;
    int end = begin;
    while (end < (int)url.size() && url[end] != '/') { end++; }
    return url.substr(begin, end - begin);
}

// Level-by-level BFS, one thread per url of the level, joined before the next level.
// Production would use a fixed thread pool with a shared work queue instead.
// T: O(V+E) work, S: O(V + L) for L urls in the widest level.
vector<string> crawl(string startUrl, HtmlParser htmlParser) {
    string host = hostOf(startUrl);
    unordered_set<string> seen = {startUrl};   // urls already scheduled
    vector<string> found = {startUrl};         // result, in discovery order
    vector<string> cur = {startUrl};           // urls of the current level
    mutex mu;                                  // guards seen and nxt
    while (!cur.empty()) {
        vector<string> nxt;                    // new urls for the next level
        vector<thread> workers;
        for (const string& url : cur) {
            workers.emplace_back([&, url]() {
                for (const string& link : htmlParser.getUrls(url)) {
                    if (hostOf(link) != host) { continue; }
                    lock_guard<mutex> lock(mu);
                    if (seen.insert(link).second) { nxt.push_back(link); }
                }
            });
        }
        for (thread& t : workers) { t.join(); }
        found.insert(found.end(), nxt.begin(), nxt.end());
        cur = nxt;
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
