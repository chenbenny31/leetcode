// length-prefix, prefix each string with its sep and sep, T: O(nk), S: O(nk)

#include <vector>
#include <string>

class Codec {
    static constexpr char SEP = '#';

public:
    std::string encode(std::vector<std::string>& strs) {
        std::string out;
        size_t total = 0;
        for (auto& s : strs) {
            total += std::to_string(s.size()).size() + 1 + s.size();
        }
        out.reserve(total);

        for (auto& s : strs) {
            for (char c : std::to_string(s.size())) { out += c; }
            out += SEP;
            out.append(s);
        }
        return out;
    }

    std::vector<std::string> decode(std::string s) {
        std::vector<std::string> out;
        int i = 0;
        int n = static_cast<int>(s.size());

        while (i < n) {
            int len = 0;
            while (s[i] != SEP) { len = len * 10 + (s[i++] - '0'); }
            i++; // skip SEP
            out.push_back(s.substr(i, len));
            i += len;
        }
        return out;
    }
};


// fixed-width binary with explicit big-endian encoding

#include <vector>
#include <string>
#include <cstdint> // uint32_t, uint8_t
#include <string_view> // std::string_view

class Codec {
public:
    std::string encode(std::vector<std::string>& strs) {
        std::string out;
        size_t total = 0;
        for (auto& s : strs) { total += 4 + s.size(); }
        out.reserve(total);

        for (auto& s : strs) {
            uint32_t len = static_cast<uint32_t>(s.size());
            out += static_cast<char>((len >> 24) & 0xFF);
            out += static_cast<char>((len >> 16) & 0xFF);
            out += static_cast<char>((len >> 8) & 0xFF);
            out += static_cast<char>((len >> 0) & 0xFF);
            out.append(s);
        }
        return out;
    }

    std::vector<std::string> decode(std::string s) {
        std::vector<std::string> out;
        std::string_view buf(s);
        size_t i = 0;

        while (i < buf.size()) {
            uint32_t len = (static_cast<uint8_t>(buf[i]) << 24)
                         | (static_cast<uint8_t>(buf[i+1]) << 16)
                         | (static_cast<uint8_t>(buf[i+2]) << 8)
                         | (static_cast<uint8_t>(buf[i+3]) << 0);
            i += 4;
            out.emplace_back(buf.substr(i, len));
            i += len;
        }
        return out;
    }
};

// escaped-delim vs ASCII-len-prefix vs bin-len-prefix: bin is 4 bytes + content and no sep, for prod
// reserve-upfront: single alloc
// zero-copy via string_view
// uint8_t cast for char(signed), mandatory
// fixed-width headers: 2^32 ~ 4GB, whereas uint8_t for 255, uint16_t for 65535 bytes
// string_view lifetime: binds with buf, should outlive
