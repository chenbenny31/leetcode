// 15 · LEB128 varint — OA follow-up (not reported)
// same 7-bit chunks + MSB flag as the VLQ codec (oa/01), but the low chunk is written
// first (protobuf, DWARF, WebAssembly); 558 = 0000100 0101110 -> 0xAE 0x04
//
// clarify:
// width -> up to 64 bits, so at most 10 bytes
// MSB = 1 -> more bytes follow, same as the OA
// contract -> same as the OA: return bytes written / read, -1 on fail
// signed ints -> zigzag first, same as SignedCodec in oa/01

#include <iostream>
#include <vector>
#include <string>
#include <cstdint>

using std::cout;
using std::string;
using std::vector;
using std::uint64_t;
using std::uint8_t;

// core idea: encode peels 7 bits off the low end each round; decode puts chunk i at
// bit pos 7 * i and stops at the first byte with MSB = 0
// T: O(num of bytes), S: O(1)
class Leb128Codec {
    static constexpr int DATA_MASK = 0x7f; // lower 7 bits
    static constexpr int MSB_BIT = 0x80;   // top bit of a byte
    static constexpr int MAX_BYTES = 10;   // 64 / 7 + 1

public:
    static int encode(uint64_t val, uint8_t* buf, int cap) {
        int count = 1; // num of 7-bit chunks
        uint64_t remain = val >> 7;
        while (remain != 0) {
            count++;
            remain >>= 7;
        }
        if (count > cap) { return -1; }

        for (int i = 0; i < count; i++) {
            uint8_t byte = val & DATA_MASK;
            if (i < count - 1) { byte |= MSB_BIT; } // not the last byte
            buf[i] = byte;
            val >>= 7;
        }
        return count;
    }

    static int decode(const uint8_t* buf, int len, uint64_t& val) {
        uint64_t result = 0;
        // an 11th byte would shift by 70, and a shift >= 64 is undefined
        for (int i = 0; i < len && i < MAX_BYTES; i++) {
            uint64_t chunk = buf[i] & DATA_MASK; // uint64 first, then shift
            result |= chunk << (7 * i);
            if ((buf[i] & MSB_BIT) == 0) {
                val = result;
                return i + 1;
            }
        }
        return -1;
    }
};

void test(const string& label, uint64_t val, const vector<uint8_t>& want) {
    uint8_t buf[10];
    int nwrite = Leb128Codec::encode(val, buf, 10);
    vector<uint8_t> got(buf, buf + nwrite);

    uint64_t res = 0;
    int nread = Leb128Codec::decode(buf, nwrite, res);
    bool ok = got == want && nread == nwrite && res == val;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("0", 0, {0x00});
    test("min-2bytes", 128, {0x80, 0x01});
    test("558", 558, {0xAE, 0x04});
    test("UINT64_MAX", UINT64_MAX,
         {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x01});

    uint8_t two[] = {0xAE, 0x04, 0x05};
    uint64_t val = 0;
    check(Leb128Codec::decode(two, 3, val) == 2 && val == 558, "stop-follow");

    uint8_t buf[10];
    uint8_t cut[] = {0xAE};
    uint8_t eleven[] = {0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00};
    check(Leb128Codec::encode(558, buf, 1) == -1, "cap<count");
    check(Leb128Codec::decode(cut, 1, val) == -1, "cut");
    check(Leb128Codec::decode(eleven, 11, val) == -1, "11-bytes");
}

// follow-up:
// 1. vs the OA order -> high first needs the chunk count up front; low first does not
// 2. signed -> zigzag (n << 1) ^ (n >> 63), then the same encode
// 3. 10th byte above 0x01 -> the value passes 64 bits; reject it in decode
// 4. many values in one buffer -> count prefix, then buf + pos (24-varint-series)
// 5. fixed width instead -> 4 or 8 bytes with shifts (14-endian-fixed-width)
