// OA 01 · VLQ (variable-length quantity) codec — Quant Dev OA, 2 hours, 2 parts
// (1) unsigned int -> bytes: 7-bit groups, high group first, one group per byte
//     MSB = 1 -> more bytes follow, MSB = 0 -> last byte; 558 -> 0x84 0x2E
//     encode writes raw bytes into the caller's buffer
//     decode must find the end itself: other bytes may follow the encoding
// (2) support negative ints too: say how the binary form changes, then reuse (1)
//
// clarify:
// int width -> uint64 / int64, so at most 10 bytes (64 / 7 rounded up)
// output -> raw bytes, not the text "842e"
// buffer -> caller owns it and gives >= 10 bytes; encode returns num of bytes written
// decode result -> the value + num of bytes consumed; -1 if the input ends mid-value
// 0 -> one byte 0x00

#include <iostream>
#include <vector>
#include <string>
#include <cstdint>

using std::cout;
using std::string;
using std::vector;
using std::uint8_t;
using std::uint64_t;
using std::int64_t;

// longest encoding of a 64-bit value: 64 / 7 rounded up
const int MAX_BYTES = 10;

// (1) encode
// core idea: count the 7-bit groups, then write them from the highest group down
// T: O(num of bytes), S: O(1)
int encode(uint64_t value, uint8_t* buf) {
    int count = 1;
    uint64_t rest = value >> 7;
    while (rest != 0) {
        count++;
        rest >>= 7;
    }
    for (int i = 0; i < count; i++) {
        // group i from the top sits 7 * (count - 1 - i) bits up
        int shift = 7 * (count - 1 - i);
        uint8_t byte = (value >> shift) & 0x7F;
        // every byte except the last one sets the continuation bit
        if (i < count - 1) { byte |= 0x80; }
        buf[i] = byte;
    }
    return count;
}

// (1) decode
// core idea: value = value << 7 | low 7 bits of each byte; stop at the first MSB = 0
// returns num of bytes consumed -> caller knows where the next field starts
// T: O(num of bytes), S: O(1)
int decode(const uint8_t* buf, int len, uint64_t& value) {
    value = 0;
    for (int i = 0; i < len && i < MAX_BYTES; i++) {
        value = (value << 7) | (buf[i] & 0x7F);
        if ((buf[i] & 0x80) == 0) { return i + 1; }
    }
    // ran out of input (or past 10 bytes) before a byte w/ MSB = 0
    return -1;
}

// (2) signed -> unsigned (zigzag): 0, -1, 1, -2, 2 -> 0, 1, 2, 3, 4
// core idea: sign goes to bit 0, magnitude to the bits above -> small |x| stays short
// x >= 0 -> 2x; x < 0 -> 2|x| - 1
uint64_t zigzag(int64_t x) {
    if (x >= 0) { return (uint64_t)x << 1; }
    // -(x + 1) = |x| - 1: no overflow at INT64_MIN
    return ((uint64_t)(-(x + 1)) << 1) | 1;
}

int64_t unzigzag(uint64_t z) {
    if ((z & 1) == 0) { return (int64_t)(z >> 1); }
    return -(int64_t)(z >> 1) - 1;
}

// (2) the byte format is unchanged: map to unsigned, then reuse (1) as is
int encodeSigned(int64_t x, uint8_t* buf) {
    return encode(zigzag(x), buf);
}

int decodeSigned(const uint8_t* buf, int len, int64_t& x) {
    uint64_t z = 0;
    int used = decode(buf, len, z);
    if (used == -1) { return -1; }
    x = unzigzag(z);
    return used;
}

void test(const string& label, uint64_t value, const vector<uint8_t>& want) {
    uint8_t buf[MAX_BYTES];
    int written = encode(value, buf);
    vector<uint8_t> got(buf, buf + written);
    uint64_t back = 0;
    int used = decode(buf, written, back);
    bool ok = got == want && used == written && back == value;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void testDecode(const string& label, const vector<uint8_t>& bytes, int wantUsed,
                uint64_t wantValue) {
    uint64_t value = 0;
    int used = decode(bytes.data(), bytes.size(), value);
    bool ok = used == wantUsed && (used == -1 || value == wantValue);
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void testSigned(const string& label, int64_t x, const vector<uint8_t>& want) {
    uint8_t buf[MAX_BYTES];
    int written = encodeSigned(x, buf);
    vector<uint8_t> got(buf, buf + written);
    int64_t back = 0;
    int used = decodeSigned(buf, written, back);
    bool ok = got == want && used == written && back == x;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("zero", 0, {0x00});
    test("one-byte-max", 127, {0x7F});
    test("two-bytes-min", 128, {0x81, 0x00});
    test("given-558", 558, {0x84, 0x2E});
    test("u64-max", UINT64_MAX,
         {0x81, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F});
    testDecode("trailing-bytes", {0x84, 0x2E, 0xFF}, 2, 558);
    testDecode("cut-mid-value", {0x84}, -1, 0);
    testSigned("minus-one", -1, {0x01});
    testSigned("plus-one", 1, {0x02});
    testSigned("minus-65", -65, {0x81, 0x01});
    testSigned("i64-min", INT64_MIN,
               {0x81, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F});
}

// follow-up:
// why not two's complement as is -> -1 = 64 one-bits -> always 10 bytes
// alt for (2): sign bit in the first byte -> first byte holds 6 bits; (1) not reusable
// overflow on decode -> a 10-byte input may carry > 64 bits; reject if byte 0 > 0x81
// many values in one buffer -> advance the read position by the returned count
// low group first (LEB128, protobuf varint) -> no group count needed; emit, then shift
// buffer size given -> return -1 before writing if count > capacity
// branch-free zigzag ->
/*
uint64_t zigzag(int64_t x) { return ((uint64_t)x << 1) ^ (uint64_t)(x >> 63); }
int64_t unzigzag(uint64_t z) { return (int64_t)(z >> 1) ^ -(int64_t)(z & 1); }
*/
