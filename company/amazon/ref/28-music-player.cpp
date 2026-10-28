// Music player: addSongs(user, songs) adds 1 to each listed song's global frequency
// (repeats count). playNext() plays the most frequent song not yet played in the current
// cycle (ties: alphabetical); once every song has been played, the cycle resets.
// playNext is called only when at least one song exists.

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <unordered_set>
#include <utility>

using std::cout;
using std::string;
using std::vector;
using std::map;
using std::set;
using std::unordered_set;
using std::pair;

// straightforward: scan every song for the best unplayed one.
// T: addSongs O(s log u), playNext O(u). S: O(u).
class MusicPlayer {
    // freq[song] = total times added; map order gives alphabetical ties
    map<string, int> freq;
    // songs played in the current cycle
    unordered_set<string> played;

public:
    void addSongs(const string& user, const vector<string>& songs) {
        (void)user;
        for (const string& song : songs) { freq[song]++; }
    }

    string playNext() {
        if (played.size() == freq.size()) { played.clear(); }
        string best;
        int bestFreq = 0;
        for (const auto& [song, f] : freq) {
            if (!played.count(song) && f > bestFreq) {
                best = song;
                bestFreq = f;
            }
        }
        played.insert(best);
        return best;
    }
};

// improved: an ordered set of (-freq, song) for unplayed songs; its first element is the
// next song. A frequency change is erase + insert. T: O(log u) per song added or played,
// O(u log u) per reset. S: O(u).
class MusicPlayerSet {
    // freq[song] = total times added
    map<string, int> freq;
    // (-freq, song) for songs not yet played this cycle
    set<pair<int, string>> unplayed;

public:
    void addSongs(const string& user, const vector<string>& songs) {
        (void)user;
        for (const string& song : songs) {
            // a song is unplayed if new, or if its entry is in the set
            bool isNew = !freq.count(song);
            bool waiting = !isNew && unplayed.count({-freq[song], song});
            if (waiting) { unplayed.erase({-freq[song], song}); }
            freq[song]++;
            if (isNew || waiting) { unplayed.insert({-freq[song], song}); }
        }
    }

    string playNext() {
        if (unplayed.empty()) {
            for (const auto& [song, f] : freq) { unplayed.insert({-f, song}); }
        }
        string song = unplayed.begin()->second;
        unplayed.erase(unplayed.begin());
        return song;
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    MusicPlayer a;
    MusicPlayerSet b;
    a.addSongs("u1", {"x", "y", "x", "z"});
    b.addSongs("u1", {"x", "y", "x", "z"});
    a.addSongs("u2", {"x", "y"});
    b.addSongs("u2", {"x", "y"});
    check(a.playNext() == "x" && b.playNext() == "x", "top");
    check(a.playNext() == "y" && b.playNext() == "y", "second");
    check(a.playNext() == "z" && b.playNext() == "z", "last");
    check(a.playNext() == "x" && b.playNext() == "x", "reset");
    MusicPlayer c;
    MusicPlayerSet d;
    c.addSongs("u", {"q", "p"});
    d.addSongs("u", {"q", "p"});
    check(c.playNext() == "p" && d.playNext() == "p", "alpha-tie");
    c.addSongs("v", {"q", "q", "r"});
    d.addSongs("v", {"q", "q", "r"});
    check(c.playNext() == "q" && d.playNext() == "q", "updated");
    check(c.playNext() == "r" && d.playNext() == "r", "new-song");
}
