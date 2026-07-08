// length-prefix, T: O(n), S: O(n)

#include <vector>
#include <string> // std::to_string, std::stoi

class Codec {
private:
    static constexpr char DELIM = '#';

public:
    std::string encode(std::vector<std::string>& strs) {
        std::string out;
        for (int s = 0; s < static_cast<int>(strs.size()); s++) {
            out += std::to_string(strs[s].length());
            out += DELIM;
            out += strs[s];
        }
        return out;
    }

    std::vector<std::string> decode(std::string s) {
        std::vector<std::string> out;
        int i = 0;
        const int n = static_cast<int>(s.length());

        while (i < n) {
            int j = i;
            while (s[j] != DELIM) { j++; }

            int len = std::stoi(s.substr(i, j - i));
            int start = j + 1;
            out.push_back(s.substr(start, len));
            i = start + len;
        }
        return out;
    }
};

// len+DELIM+str: DELIM is to term len, avoid ambiguity between len and str
// from_chars + string_view + emplace_back: save one copy from std::stoi(s.substr(i,j-i))
// follow-up big-endian encoding: fixed header size, no reserve delimiter, shift + OR for parsing, network byte order
