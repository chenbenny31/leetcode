// Next-Word Predictor (Google, interview report)
// train(sentences): only adjacent words inside one sentence count as (word, next)
// (1) mostFrequentNext(word): most frequent successor, ties -> smallest word
// (2) predict(word): random successor w/ probability proportional to its cnt
//
// clarify:
// words already split / normalized -> yes
// pairs cross sentences? -> never
// unseen word / no successor -> ""
// train called again -> cnts add up

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <algorithm>
#include <cstdlib>
#include <cmath>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::map;
using std::upper_bound;
using std::rand;
using std::srand;
using std::abs;

// core idea: cnt bigrams in word -> (next -> cnt); (1) scans that word's cnts
// (2) prefix sums of cnts; map random r in [0, total) to its owner, upper_bound (LC 528)
// T: train O(words), (1) O(d), (2) O(log d), S: O(pairs) — d = successors of the word
class NextWordPredictor {
    // cnts[word][next] = times next followed word
    unordered_map<string, unordered_map<string, int>> counts;
    // nextWords[word] = successors of word, in a fixed order
    unordered_map<string, vector<string>> nextWords;
    // prefixCount[word][i] = sum of cnts of nextWords[word][0..i]
    unordered_map<string, vector<int>> prefixCount;

public:
    void train(const vector<vector<string>>& sentences) {
        for (const auto& sentence : sentences) {
            int n = sentence.size();
            for (int i = 0; i + 1 < n; i++) { counts[sentence[i]][sentence[i + 1]]++; }
        }
        for (const auto& [word, followers] : counts) {
            vector<string>& words = nextWords[word];
            vector<int>& prefix = prefixCount[word];
            words.clear();
            prefix.clear();
            int running = 0;
            for (const auto& [next, count] : followers) {
                running += count;
                words.push_back(next);
                prefix.push_back(running);
            }
        }
    }

    // (1) scan successors of word for the max cnt
    string mostFrequentNext(const string& word) {
        auto it = counts.find(word);
        if (it == counts.end()) { return ""; }
        string best;
        int bestCount = 0;
        for (const auto& [next, count] : it->second) {
            if (count > bestCount || (count == bestCount && next < best)) {
                best = next;
                bestCount = count;
            }
        }
        return best;
    }

    // (2) prefix[i] = end of successor i's stretch; r belongs to first end > r
    string predict(const string& word) {
        auto it = prefixCount.find(word);
        if (it == prefixCount.end()) { return ""; }
        const vector<int>& prefix = it->second;
        int r = rand() % prefix.back();
        int index = upper_bound(prefix.begin(), prefix.end(), r) - prefix.begin();
        return nextWords[word][index];
    }
};

void test(const string& label, const vector<vector<string>>& sentences,
          const string& word, const string& want) {
    NextWordPredictor predictor;
    predictor.train(sentences);
    cout << label << (predictor.mostFrequentNext(word) == want ? " PASS\n" : " FAIL\n");
}

// wantShare[next] = expected fraction of draws that return next
void testSample(const string& label, const vector<vector<string>>& sentences,
                const string& word, const map<string, double>& wantShare) {
    NextWordPredictor predictor;
    predictor.train(sentences);
    srand(7);
    int draws = 3000;
    // seen[next] = times predict returned next
    map<string, int> seen;
    for (int i = 0; i < draws; i++) { seen[predictor.predict(word)]++; }
    bool ok = seen.size() == wantShare.size();
    for (const auto& [next, share] : wantShare) {
        ok = ok && abs(1.0 * seen[next] / draws - share) < 0.05;
    }
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    vector<vector<string>> corpus = {{"i", "like", "tea"}, {"i", "like", "milk"},
                                     {"i", "want", "tea"}};
    test("most-frequent", corpus, "i", "like");
    test("tie-smallest", corpus, "like", "milk");
    test("sentence-end", corpus, "tea", "");
    test("unseen", corpus, "coffee", "");

    vector<vector<string>> weighted = {{"i", "like", "tea"}, {"we", "like", "tea"},
                                       {"you", "like", "milk"}};
    testSample("sample-2-to-1", weighted, "like", {{"tea", 2.0 / 3}, {"milk", 1.0 / 3}});
    testSample("sample-single", weighted, "we", {{"like", 1.0}});
    testSample("sample-unseen", weighted, "tea", {{"", 1.0}});
}

// follow-up:
// top-k next words -> sort that word's (-cnt, next) pairs, take first k
// O(1) mostFrequentNext -> keep best[word] in train; only the pair just counted can win
// O(1) sampling -> one alias table per word over its cnts
// cnts keep changing (streaming) -> Fenwick tree per word: O(log d) update and pick
// huge vocabulary -> keep only top-N successors per word, or sorted vector of (next, cnt)
// more context (previous two words) -> key by "w1 w2"; fall back to one-word table
