// 25 · embedded basics list
// copy of `company/scientech/ref/recommended/14-endian-fixed-width.cpp`
// Fixed-width ints and byte order — codec family (the OA mentions endianness)
// write / read a uint32 as exactly 4 bytes; 0x12345678 -> BE 12 34 56 78, LE 78 56 34 12
// big-endian (BE) = high byte first (network order), little-endian (LE) = low byte first
// (1) encode / decode in both orders  (2) byte order of this machine, swap the order
//
// clarify:
// byte order -> fixed by the file / wire format, not by the machine
// width -> 4 bytes (uint32); uint16 / uint64 are the same loops with 2 / 8
// contract -> same as the OA codec: return bytes written / read, -1 if buf is too short

#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
#include <cstring>

using std::cout;
using std::string;
using std::vector;
using std::uint32_t;
using std::uint8_t;
using std::memcpy;

// ==================== (1) ====================
// encode / decode a uint32 as 4 bytes in both byte orders

// core idea: shifts work on the value, not on memory, so the bytes come out the same on
// any machine; BE handles the high byte first, LE the low byte first
// T: O(1), S: O(1)
int encodeBE(uint32_t val, uint8_t* buf, int cap) {
    if (cap < 4) { return -1; }
    for (int i = 0; i < 4; i++) {
        buf[i] = (val >> (8 * (3 - i))) & 0xff; // i = 0 takes the high byte
    }
    return 4;
}

int decodeBE(const uint8_t* buf, int len, uint32_t& val) {
    if (len < 4) { return -1; }
    val = 0;
    for (int i = 0; i < 4; i++) {
        val = (val << 8) | buf[i]; // the first byte ends up highest
    }
    return 4;
}

int encodeLE(uint32_t val, uint8_t* buf, int cap) {
    if (cap < 4) { return -1; }
    for (int i = 0; i < 4; i++) {
        buf[i] = (val >> (8 * i)) & 0xff; // i = 0 takes the low byte
    }
    return 4;
}

int decodeLE(const uint8_t* buf, int len, uint32_t& val) {
    if (len < 4) { return -1; }
    val = 0;
    for (int i = 3; i >= 0; i--) {
        val = (val << 8) | buf[i]; // the last byte ends up highest
    }
    return 4;
}

// ==================== (2) ====================
// which byte order does this machine use; reverse the 4 bytes of a value

// core idea: look at the byte at the lowest address of the int 1
bool isLittleEndian() {
    uint32_t one = 1;
    uint8_t first = 0;
    memcpy(&first, &one, 1);
    return first == 1;
}

// core idea: BE <-> LE; the lowest byte moves to the top, 4 times
// T: O(1), S: O(1)
uint32_t swapBytes(uint32_t val) {
    uint32_t result = 0;
    for (int i = 0; i < 4; i++) {
        result = (result << 8) | (val & 0xff);
        val >>= 8;
    }
    return result;
}

// ==================== tests ====================
void test(const string& label, uint32_t val, const vector<uint8_t>& wantBE) {
    uint8_t be[4];
    uint8_t le[4];
    encodeBE(val, be, 4);
    encodeLE(val, le, 4);
    vector<uint8_t> gotBE(be, be + 4);
    vector<uint8_t> gotLE(le, le + 4);
    vector<uint8_t> wantLE(wantBE.rbegin(), wantBE.rend());

    uint32_t fromBE = 0;
    uint32_t fromLE = 0;
    decodeBE(be, 4, fromBE);
    decodeLE(le, 4, fromLE);
    bool ok = gotBE == wantBE && gotLE == wantLE && fromBE == val && fromLE == val;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("four-bytes", 0x12345678, {0x12, 0x34, 0x56, 0x78});
    test("small", 1, {0x00, 0x00, 0x00, 0x01});
    check(swapBytes(0x12345678) == 0x78563412, "swap");

    uint32_t val = 0x12345678;
    uint8_t raw[4];
    memcpy(raw, &val, 4);
    check((raw[0] == 0x78) == isLittleEndian(), "host-order");
    check(encodeBE(val, raw, 3) == -1 && decodeLE(raw, 3, val) == -1, "too-short");
}

// follow-up:
// 1. uint16 / uint64 -> the same loops with width 2 / 8
// 2. record with several fields -> encode field by field at buf + pos, advance pos
// 3. why not cast buf to uint32_t* -> alignment + host order; memcpy, then swap
// 4. htonl / ntohl -> host <-> BE: swapBytes on an LE machine, nothing on a BE one
// 5. faster swap -> __builtin_bswap32, one instruction
// 6. signed int32 -> cast to uint32 first; the bit pattern is kept
