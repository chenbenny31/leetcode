// varint series: many ints in one buffer, built on the VLQ codec (oa/01)
// (1) a list of non-neg ints -> one buffer, and back
// (2) a price series (in ticks) moves a little each step -> store the diff to the
//     previous price instead of the price itself

// usage:
// g++ -std=c++17 -Wall -Wextra 10-varint-series.cpp -o a && ./a
//
// list of non-neg ints -> SeriesCodec::encodeAll, SeriesCodec::decodeAll
// price series -> SeriesCodec::encodeDeltas, SeriesCodec::decodeDeltas

// clarify / assumptions
// decoder does not know how many values follow -> write the count first
// diff of two neighbor prices fits in int64
// on fail, buf / vals may be partly filled

// refer to read(), write() logic
// encode: vals, buf, cap -> write coded into buf, return total bytes written, -1 if fail
// decode: buf, len, vals -> append to vals, return total bytes read, -1 if fail

// layout:
// encodeAll:    [count][val 0][val 1]...            each an unsigned varint
// encodeDeltas: [count][val 0 - 0][val 1 - val 0]...  each a signed varint

#include <cstdint>
#include <string>
#include <vector>
#include <iostream>

using std::uint64_t;
using std::uint8_t;
using std::int64_t;
using std::cout;
using std::string;
using std::vector;

// same code as oa/01-vlq-codec.cpp
class UnsignedCodec {
private:
    static constexpr int DATA_MASK = 0x7f; // lower 7 bits
    static constexpr int MSB_BIT = 0x80; // top bit of byte
public:
    static int encode(uint64_t val, uint8_t* buf, int cap) {
        int count = 1; // num of 7-bit chunks
        uint64_t remain = val >> 7;
        while (remain != 0) {
            count++;
            remain >>= 7;
        }
        if (count > cap) { return -1; }

        int shift = 7 * (count - 1); // bit pos of cur highest chunk
        for (int i = 0; i < count; i++) {
            uint8_t byte = (val >> shift) & DATA_MASK; // 7bit of cur highest
            if (i < count - 1) { byte |= MSB_BIT; } // not last byte -> put MSB
            buf[i] = byte;
            shift -= 7;
        }
        return count;
    }

    static int decode(const uint8_t* buf, int len, uint64_t& val) {
        uint64_t result = 0;

        for (int i = 0; i < len && i < 10; i++) {
            uint8_t byte = buf[i] & DATA_MASK;
            if (result > (UINT64_MAX >> 7)) { return -1; }
            result <<= 7;
            result |= byte;

            if ((buf[i] & MSB_BIT) == 0) { // no MSB, is last byte
                val = result;
                return i + 1;
            }
        }
        return -1;
    }
};

// same code as oa/01-vlq-codec.cpp
class SignedCodec {
private:
    // signed -> unsigned mapping, 0, -1, 1, -2, 2 -> 0, 1, 2, 3, 4
    static uint64_t zigzag(int64_t num) {
        if (num >= 0) { // if non-neg, cast and double
            uint64_t x = num;
            return 2 * x;
        }

        uint64_t x = -(num + 1); // -1 be the 0-th neg
        return 2 * x + 1; // -1 -> 2 * 0 + 1 = 1
    }

    // unsigned -> signed mapping
    static int64_t unzigzag(uint64_t num) {
        int64_t x = num / 2;
        if (num % 2 == 0) { return x; }
        return -x - 1; // x is k-th negative, 0-th is -1
    }

public:
    static int encode(int64_t val, uint8_t* buf, int cap) {
        return UnsignedCodec::encode(zigzag(val), buf, cap);
    }

    static int decode(const uint8_t* buf, int len, int64_t& val) {
        uint64_t x = 0;
        int read = UnsignedCodec::decode(buf, len, x);
        if (read == -1) { return -1; }
        val = unzigzag(x);
        return read;
    }
};

// each value starts where the last one ended: buf + pos, with cap - pos left
class SeriesCodec {
public:
    // (1) count first, then the values one by one
    // return total bytes written, -1 if fail, T: O(total bytes), S: O(1)
    static int encodeAll(const vector<uint64_t>& vals, uint8_t* buf, int cap) {
        int pos = UnsignedCodec::encode(vals.size(), buf, cap); // next write pos
        if (pos == -1) { return -1; }

        for (uint64_t val : vals) {
            int nwrite = UnsignedCodec::encode(val, buf + pos, cap - pos);
            if (nwrite == -1) { return -1; }
            pos += nwrite;
        }
        return pos;
    }

    // read count, then that many values; bytes after them are left for the caller
    // return total bytes read, -1 if fail, T: O(total bytes), S: O(1)
    static int decodeAll(const uint8_t* buf, int len, vector<uint64_t>& vals) {
        uint64_t count = 0;
        int pos = UnsignedCodec::decode(buf, len, count); // next read pos
        if (pos == -1) { return -1; }

        // count comes from the buffer, don't reserve(count): it may be garbage
        for (uint64_t i = 0; i < count; i++) {
            uint64_t val = 0;
            int nread = UnsignedCodec::decode(buf + pos, len - pos, val);
            if (nread == -1) { return -1; }
            vals.push_back(val);
            pos += nread;
        }
        return pos;
    }

    // (2) store val - prev: diffs are small, mostly 1 byte; can be negative -> signed
    // return total bytes written, -1 if fail, T: O(total bytes), S: O(1)
    static int encodeDeltas(const vector<int64_t>& vals, uint8_t* buf, int cap) {
        int pos = UnsignedCodec::encode(vals.size(), buf, cap);
        if (pos == -1) { return -1; }

        int64_t prev = 0; // first value is stored as diff to 0
        for (int64_t val : vals) {
            int nwrite = SignedCodec::encode(val - prev, buf + pos, cap - pos);
            if (nwrite == -1) { return -1; }
            pos += nwrite;
            prev = val;
        }
        return pos;
    }

    // add each diff back onto the running value
    // return total bytes read, -1 if fail, T: O(total bytes), S: O(1)
    static int decodeDeltas(const uint8_t* buf, int len, vector<int64_t>& vals) {
        uint64_t count = 0;
        int pos = UnsignedCodec::decode(buf, len, count);
        if (pos == -1) { return -1; }

        int64_t prev = 0;
        for (uint64_t i = 0; i < count; i++) {
            int64_t diff = 0;
            int nread = SignedCodec::decode(buf + pos, len - pos, diff);
            if (nread == -1) { return -1; }
            prev += diff;
            vals.push_back(prev);
            pos += nread;
        }
        return pos;
    }
};

void testAll(const string& label, const vector<uint64_t>& vals,
             const vector<uint8_t>& want) {
    uint8_t buf[64];
    int nwrite = SeriesCodec::encodeAll(vals, buf, 64);
    vector<uint8_t> got(buf, buf + nwrite);

    vector<uint64_t> res;
    int nread = SeriesCodec::decodeAll(buf, nwrite, res);
    bool ok = (got == want && nread == nwrite && res == vals);

    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void testDeltas(const string& label, const vector<int64_t>& vals,
                const vector<uint8_t>& want) {
    uint8_t buf[64];
    int nwrite = SeriesCodec::encodeDeltas(vals, buf, 64);
    vector<uint8_t> got(buf, buf + nwrite);

    vector<int64_t> res;
    int nread = SeriesCodec::decodeDeltas(buf, nwrite, res);
    bool ok = (got == want && nread == nwrite && res == vals);

    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void check(const string& label, bool ok) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    // test valid cases
    testAll("no-vals", {}, {0x00});
    testAll("3-vals", {1, 558, 128}, {0x03, 0x01, 0x84, 0x2E, 0x81, 0x00});

    testDeltas("no-prices", {}, {0x00});
    testDeltas("prices", {100, 101, 99, 99}, {0x04, 0x81, 0x48, 0x02, 0x03, 0x00});
    testDeltas("neg-first", {-5, 5}, {0x02, 0x09, 0x14});

    // test on following bytes
    uint8_t two[] = {0x01, 0x07, 0xFF}; // list {7} + following
    vector<uint64_t> vals;
    check("stop-follow", SeriesCodec::decodeAll(two, 3, vals) == 2 && vals.size() == 1);

    // test invalid cases
    uint8_t buf[2];
    uint8_t cut[] = {0x03, 0x01, 0x02}; // count 3, only 2 values
    vector<uint64_t> part;
    check("cap<total", SeriesCodec::encodeAll({1, 558}, buf, 2) == -1);
    check("cut", SeriesCodec::decodeAll(cut, 3, part) == -1);
    check("len=0", SeriesCodec::decodeAll(cut, 0, part) == -1);
}
