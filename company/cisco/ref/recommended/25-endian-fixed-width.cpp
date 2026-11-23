// 25 · embedded basics list
// copy of `company/scientech/ref/recommended/21-endian-fixed-width.cpp`
// fixed-width ints in a byte buffer: write / read a uint32 as exactly 4 bytes
// big-endian (BE) = high byte first (network order), little-endian (LE) = low byte first
// e.g. 0x12345678 -> BE: 12 34 56 78, LE: 78 56 34 12
// (1) encode / decode in both orders  (2) which order is this machine, swap the order

// usage:
// g++ -std=c++17 -Wall -Wextra 21-endian-fixed-width.cpp -o a && ./a
//
// network / file format says BE -> FixedCodec::encodeBE, FixedCodec::decodeBE
// format says LE -> FixedCodec::encodeLE, FixedCodec::decodeLE

// clarify / assumptions
// width is 4 bytes (uint32), uint16 / uint64 are the same loop with 2 / 8
// the byte order is fixed by the format, not by the machine

// refer to read(), write() logic
// encode: val, buf, cap -> write 4 bytes into buf, return 4, -1 if fail
// decode: buf, len, val -> read 4 bytes from buf, update val&, return 4, -1 if fail

// bit-ops:
// (val >> shift) & 0xff -> the byte at bit pos shift
// shifts work on the value, not on memory -> same result on any machine

#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <iostream>

using std::uint32_t;
using std::uint8_t;
using std::memcpy;
using std::cout;
using std::string;
using std::vector;

// same loops as the VLQ codec with 8-bit chunks and no MSB flag
class FixedCodec {
private:
    static constexpr int WIDTH = 4; // bytes of a uint32
    static constexpr int BYTE_MASK = 0xff; // lower 8 bits
public:
    // write from high to low byte, T: O(1), S: O(1)
    static int encodeBE(uint32_t val, uint8_t* buf, int cap) {
        if (cap < WIDTH) { return -1; }

        int shift = 8 * (WIDTH - 1); // bit pos of cur highest byte
        for (int i = 0; i < WIDTH; i++) {
            buf[i] = (val >> shift) & BYTE_MASK;
            shift -= 8;
        }
        return WIDTH;
    }

    // keep shift 8 bits and accum bytes, first byte ends up highest
    static int decodeBE(const uint8_t* buf, int len, uint32_t& val) {
        if (len < WIDTH) { return -1; }

        uint32_t result = 0;
        for (int i = 0; i < WIDTH; i++) {
            result <<= 8;
            result |= buf[i];
        }
        val = result;
        return WIDTH;
    }

    // write from low to high byte
    static int encodeLE(uint32_t val, uint8_t* buf, int cap) {
        if (cap < WIDTH) { return -1; }

        for (int i = 0; i < WIDTH; i++) {
            buf[i] = val & BYTE_MASK; // cur lowest byte
            val >>= 8;
        }
        return WIDTH;
    }

    // same accum, but walk buf backward: last byte is the highest
    static int decodeLE(const uint8_t* buf, int len, uint32_t& val) {
        if (len < WIDTH) { return -1; }

        uint32_t result = 0;
        for (int i = WIDTH - 1; i >= 0; i--) {
            result <<= 8;
            result |= buf[i];
        }
        val = result;
        return WIDTH;
    }
};

// (2) look at the byte at the lowest address of the int 1
bool isLittleEndian() {
    uint32_t one = 1;
    uint8_t first = 0;
    memcpy(&first, &one, 1);
    return first == 1; // low byte comes first
}

// reverse the 4 bytes, BE <-> LE
uint32_t swapBytes(uint32_t val) {
    uint32_t result = 0;
    for (int i = 0; i < 4; i++) {
        result <<= 8;
        result |= val & 0xff; // cur lowest byte goes to the top
        val >>= 8;
    }
    return result;
}

void testBE(const string& label, uint32_t val, const vector<uint8_t>& want) {
    uint8_t buf[4];
    int nwrite = FixedCodec::encodeBE(val, buf, 4);
    vector<uint8_t> got(buf, buf + nwrite);

    uint32_t res = 0;
    int nread = FixedCodec::decodeBE(buf, nwrite, res);
    bool ok = (got == want && nread == nwrite && res == val);

    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void testLE(const string& label, uint32_t val, const vector<uint8_t>& want) {
    uint8_t buf[4];
    int nwrite = FixedCodec::encodeLE(val, buf, 4);
    vector<uint8_t> got(buf, buf + nwrite);

    uint32_t res = 0;
    int nread = FixedCodec::decodeLE(buf, nwrite, res);
    bool ok = (got == want && nread == nwrite && res == val);

    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void check(const string& label, bool ok) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    // test valid cases
    testBE("be", 0x12345678, {0x12, 0x34, 0x56, 0x78});
    testBE("be-small", 1, {0x00, 0x00, 0x00, 0x01});
    testLE("le", 0x12345678, {0x78, 0x56, 0x34, 0x12});
    testLE("le-small", 1, {0x01, 0x00, 0x00, 0x00});

    check("swap", swapBytes(0x12345678) == 0x78563412);

    // memory layout of this machine must match the order it reports
    uint32_t val = 0x12345678;
    uint8_t raw[4];
    memcpy(raw, &val, 4);
    check("host-order", (raw[0] == 0x78) == isLittleEndian());

    // test invalid cases
    uint8_t buf[4];
    check("cap<4", FixedCodec::encodeBE(val, buf, 3) == -1);
    check("len<4", FixedCodec::decodeLE(buf, 3, val) == -1);
}
