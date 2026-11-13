// Next-Word Predictor (Google, interview report): train on sentences (lists of words);
// only adjacent words inside one sentence count as (word, next).
// Part 1: mostFrequentNext(word) = the successor seen most often after word; ties go to
//   the smallest word; "" if word has no successor.
// Part 2: predict(word) = a random successor, chosen with probability proportional to its
//   count; "" if word has no successor.
//
// Clarify:
// - words already split and normalized (case, punctuation)? -> assume yes
// - a pair never crosses two sentences -> stated; a sentence's last word gets no
//   successor from it
// - ties in part 1 -> smallest word by string <
// - unseen word, or a word only seen last -> ""
// - train called more than once -> counts add up; the sampling arrays are rebuilt
// Idea: count bigrams in a map word -> (next -> count). Part 1 scans that word's counts.
//   Part 2 lays the counts end to end (prefix sums per word) and maps a random r in
//   [0, total) to its owner with upper_bound, as in LC 528.
// Pitfalls:
// - loop while i + 1 < n with int n; i < sentence.size() - 1 underflows on an empty
//   sentence
// - tie test: count == bestCount && next < best
// - clear and rebuild nextWords / prefixCount in train; appending to the old arrays
//   skews the picks
// - fill nextWords[word] and prefixCount[word] in the same loop, so index i names the
//   same successor in both
// - upper_bound, not lower_bound: r == prefix[i] belongs to index i + 1
// Tests to write: most frequent; tie -> smaller word; last word of a sentence; unseen
//   word; sampled shares 2:1; a single successor; top-k with a tie.

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <utility>
#include <algorithm>
#include <cstdlib>
#include <cmath>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::map;
using std::pair;
using std::sort;
using std::upper_bound;
using std::rand;
using std::srand;
using std::abs;

// T: train O(total words), mostFrequentNext O(d), predict O(log d) with d = distinct
// successors of the word. S: O(distinct pairs).
class NextWordPredictor {
    // counts[word][next] = times next followed word
    unordered_map<string, unordered_map<string, int>> counts;
    // nextWords[word] = successors of word, in a fixed order
    unordered_map<string, vector<string>> nextWords;
    // prefixCount[word][i] = sum of counts of nextWords[word][0..i]
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

    // part 1: scan the successors of word for the max count.
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

    // part 2: the weights lie end to end on [0, total); prefix[i] is where word i's
    // stretch ends, so the owner of r is the first end > r.
    string predict(const string& word) {
        auto it = prefixCount.find(word);
        if (it == prefixCount.end()) { return ""; }
        const vector<int>& prefix = it->second;
        int r = rand() % prefix.back();
        int index = upper_bound(prefix.begin(), prefix.end(), r) - prefix.begin();
        return nextWords[word][index];
    }

    // follow-up: the k most frequent successors, ties by smaller word, most frequent
    // first; fewer than k if word has fewer successors. T: O(d log d), S: O(d).
    vector<string> topNext(const string& word, int k) {
        vector<string> result;
        auto it = counts.find(word);
        if (it == counts.end()) { return result; }
        // (-count, next): ascending order = most frequent first, then smaller word
        vector<pair<int, string>> ranked;
        for (const auto& [next, count] : it->second) { ranked.push_back({-count, next}); }
        sort(ranked.begin(), ranked.end());
        for (int i = 0; i < k && i < (int)ranked.size(); i++) {
            result.push_back(ranked[i].second);
        }
        return result;
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

void testTop(const string& label, const vector<vector<string>>& sentences,
             const string& word, int k, const vector<string>& want) {
    NextWordPredictor predictor;
    predictor.train(sentences);
    cout << label << (predictor.topNext(word, k) == want ? " PASS\n" : " FAIL\n");
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

    testTop("top-count-first", corpus, "i", 5, {"like", "want"});
    testTop("top-tie-smaller", corpus, "like", 1, {"milk"});
    testTop("top-no-successor", corpus, "tea", 2, {});
}

// follow-up: top-k next words -> topNext above
// follow-up: O(1) mostFrequentNext -> keep best[word] = (next, count) up to date in
//   train; counts only grow, so only the pair just counted can take over:
//     counts[w][next]++;
//     int count = counts[w][next];
//     auto& [bestNext, bestCount] = best[w];
//     if (count > bestCount || (count == bestCount && next < bestNext)) {
//         bestNext = next;
//         bestCount = count;
//     }
// follow-up: O(1) sampling -> one AliasPicker per word over its counts
//   (recommended/44-random-pick-weight.cpp)
// follow-up: counts keep changing (streaming) -> the prefix array costs O(d) per update;
//   one DynamicWeightedPicker (Fenwick tree, file 44) per word gives O(log d) update and
//   pick, with a map successor -> its index in that word's tree
// follow-up: huge vocabulary -> keep only the top-N successors per word, or store each
//   word's successors as a sorted vector<pair<string, int>> instead of a nested map
// follow-up: use more context (previous two words) -> key counts by the pair "w1 w2",
//   and fall back to the one-word table when the pair was never seen:
//     for (int i = 0; i + 2 < n; i++) {
//         counts2[sentence[i] + " " + sentence[i + 1]][sentence[i + 2]]++;
//     }
