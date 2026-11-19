// LEB128 varint: same 7-bit chunks + MSB flag as the VLQ codec (oa/01),
// but the low chunk is written first (protobuf, DWARF, WebAssembly use this order)
// e.g. 558 = 0000100 0101110 -> 1010 1110, 0000 0100 = 0xAE04

// usage:
// g++ -std=c++17 -Wall -Wextra 05-leb128-codec.cpp -o a && ./a
//
// non-neg ints -> Leb128Codec::encode, Leb128Codec::decode
// signed ints -> zigzag first, same as SignedCodec in oa/01

// clarify / assumptions
// int width up to 64-bit -> at most 10 bytes
// byte order: low chunk first, MSB=1 still means more bytes follow

// refer to read(), write() logic
// encode: val, buf, cap -> write coded into buf, return num of bytes written, -1 if fail
// decode: buf, len, val -> read coded from buf, update val&, return num read, -1 if fail

// bit-ops:
// & 0111 1111 = & 0x7f -> get lower 7 bit
// | 1000 0000 = | 0x80 -> set top bit (MSB)
// chunk << shift -> put a 7-bit chunk at bit pos shift, chunk must be uint64 first

#include <cstdint>
#include <string>
#include <vector>
#include <iostream>

using std::uint64_t;
using std::uint8_t;
using std::cout;
using std::string;
using std::vector;

class Leb128Codec {
private:
    static constexpr int DATA_MASK = 0x7f; // lower 7 bits
    static constexpr int MSB_BIT = 0x80; // top bit of byte
    static constexpr int MAX_BYTES = 10; // 64 / 7 + 1
public:
    // count 7-bit chunks, write from low to high bits
    // return num of bytes written, -1 if fail, T: O(n), S: O(1), n = num of bytes
    static int encode(uint64_t val, uint8_t* buf, int cap) {
        int count = 1; // num of 7-bit chunks
        uint64_t remain = val >> 7;
        while (remain != 0) {
            count++;
            remain >>= 7;
        }
        if (count > cap) { return -1; }

        for (int i = 0; i < count; i++) {
            uint8_t byte = val & DATA_MASK; // 7bit of cur lowest
            if (i < count - 1) { byte |= MSB_BIT; } // not last byte -> put MSB
            buf[i] = byte;
            val >>= 7;
        }
        return count;
    }

    // chunk i goes to bit pos 7 * i, stop at first byte with MSB = 0
    // return num of bytes read, -1 if fail, T: O(n), S: O(1)
    static int decode(const uint8_t* buf, int len, uint64_t& val) {
        uint64_t result = 0;
        int shift = 0; // bit pos of cur chunk

        for (int i = 0; i < len; i++) {
            if (i == MAX_BYTES) { return -1; } // shift would be 70, >= 64 is UB
            uint64_t chunk = buf[i] & DATA_MASK;
            result |= chunk << shift;
            shift += 7;

            if ((buf[i] & MSB_BIT) == 0) { // no MSB, is last byte
                val = result;
                return i + 1;
            }
        }
        return -1;
    }
};

void testCodec(const string& label, uint64_t val, const vector<uint8_t>& want) {
    uint8_t buf[10]; // 64 / 7 + 1
    int nwrite = Leb128Codec::encode(val, buf, 10);
    vector<uint8_t> got(buf, buf + nwrite);

    uint64_t res = 0;
    int nread = Leb128Codec::decode(buf, nwrite, res);
    bool ok = (got == want && nread == nwrite && res == val);

    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void check(const string& label, bool ok) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    // test valid cases
    testCodec("0", 0, {0x00});
    testCodec("max-1byte", 127, {0x7F});
    testCodec("min-2bytes", 128, {0x80, 0x01});
    testCodec("558", 558, {0xAE, 0x04});
    testCodec("UINT64_MAX", UINT64_MAX,
              {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x01});

    // test on following bytes
    uint8_t two[] = {0xAE, 0x04, 0x05}; // 558 + following
    uint64_t val = 0;
    check("stop-follow", Leb128Codec::decode(two, 3, val) == 2 && val == 558);

    // test invalid cases
    uint8_t buf[10];
    uint8_t cut[] = {0xAE}; // 1010 1110 MSB=1, no more bytes
    uint8_t eleven[] = {0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00};
    check("cap<count", Leb128Codec::encode(558, buf, 1) == -1);
    check("cut", Leb128Codec::decode(cut, 1, val) == -1);
    check("len=0", Leb128Codec::decode(cut, 0, val) == -1);
    check("11-bytes", Leb128Codec::decode(eleven, 11, val) == -1);
}
