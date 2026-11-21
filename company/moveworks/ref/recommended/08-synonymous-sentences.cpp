// 08 · Synonymous Sentences (Moveworks onsite, Medium) — LC 1258, union-find + backtrack
// synonym pairs (transitive) and a sentence of single-space words; return every
// sentence made by swapping words for synonyms, sorted lexicographically
//
// clarify:
// synonyms transitive (a~b, b~c -> a~c) -> yes
// words split by one space, lowercase -> yes (LC)
// word w/o synonyms -> stays as it is
// output size -> product of group sizes; small on LC (<= 10 pairs)

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::sort;

// path-halving find. T: ~O(1) amortized
int findRoot(vector<int>& parent, int x) {
    while (parent[x] != x) {
        parent[x] = parent[parent[x]];
        x = parent[x];
    }
    return x;
}

// T: O(len), S: O(len)
vector<string> splitWords(const string& text) {
    vector<string> words;
    string cur;
    for (char c : text) {
        if (c == ' ') {
            words.push_back(cur);
            cur.clear();
        } else {
            cur += c;
        }
    }
    words.push_back(cur);
    return words;
}

// choose a word for position pos, recurse; at the end join the chosen words
void build(const vector<vector<string>>& options, int pos, vector<string>& chosen,
           vector<string>& out) {
    int n = options.size();
    if (pos == n) {
        string sentence = chosen[0];
        for (int i = 1; i < n; i++) { sentence += " " + chosen[i]; }
        out.push_back(sentence);
        return;
    }
    for (const string& word : options[pos]) {
        chosen[pos] = word;
        build(options, pos + 1, chosen, out);
    }
}

// core idea: union-find groups the synonyms; each word's options are its group, sorted;
// backtracking over sorted options already yields sorted sentences
// T: O(P log P + product of group sizes * len), S: same as the output
vector<string> generateSentences(const vector<vector<string>>& synonyms,
                                 const string& text) {
    // word -> id in the union-find
    unordered_map<string, int> id;
    vector<string> words;
    for (const auto& pair : synonyms) {
        for (const string& w : pair) {
            if (id.count(w) == 0) {
                id[w] = words.size();
                words.push_back(w);
            }
        }
    }
    int m = words.size();
    vector<int> parent(m);
    for (int i = 0; i < m; i++) { parent[i] = i; }
    for (const auto& pair : synonyms) {
        int a = findRoot(parent, id[pair[0]]);
        int b = findRoot(parent, id[pair[1]]);
        if (a != b) { parent[a] = b; }
    }
    // groups[root] = every word in that synonym group
    unordered_map<int, vector<string>> groups;
    for (int i = 0; i < m; i++) { groups[findRoot(parent, i)].push_back(words[i]); }
    for (auto& entry : groups) { sort(entry.second.begin(), entry.second.end()); }

    vector<string> tokens = splitWords(text);
    // options[i] = words that can stand at position i, sorted
    vector<vector<string>> options;
    for (const string& t : tokens) {
        auto it = id.find(t);
        if (it == id.end()) {
            options.push_back({t});
        } else {
            options.push_back(groups[findRoot(parent, it->second)]);
        }
    }
    vector<string> chosen(tokens.size());
    vector<string> out;
    build(options, 0, chosen, out);
    return out;
}

void test(const string& label, const vector<vector<string>>& synonyms,
          const string& text, const vector<string>& want) {
    cout << label << (generateSentences(synonyms, text) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("transitive",
         {{"happy", "joy"}, {"sad", "sorrow"}, {"joy", "cheerful"}},
         "I am happy today but was sad yesterday",
         {"I am cheerful today but was sad yesterday",
          "I am cheerful today but was sorrow yesterday",
          "I am happy today but was sad yesterday",
          "I am happy today but was sorrow yesterday",
          "I am joy today but was sad yesterday",
          "I am joy today but was sorrow yesterday"});
    test("no-synonyms", {}, "hello world", {"hello world"});
    return 0;
}

// follow-up:
// why sorted output w/o a final sort -> positions are filled left to right from sorted
//   options, so sentences come out in order (words never contain ' ')
// a synonym is a phrase ("new york" ~ "nyc") -> match the longest phrase at each
//   position (trie of phrases or try lengths), replace that span, keep backtracking
// only the count -> product of the group sizes of the words in the sentence
// query rewrite for search (reported) -> same groups; expand the query into an OR
