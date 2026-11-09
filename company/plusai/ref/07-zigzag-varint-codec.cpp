// PlusAI (JD): Zigzag Varint Codec (Medium). Pattern: bit manipulation.
// Encode timestamps as deltas (first value as is, then differences), zigzag each delta to
// unsigned ((x << 1) ^ (x >> 63)), and write LEB128 varint bytes (7 bits per byte, low
// group first, high bit = more bytes follow). decode reverses it exactly.

#include <iostream>
#include <vector>
#include <string>
#include <cstdint>

using std::cout;
using std::string;
using std::vector;
using std::uint8_t;

// This is protobuf's sint64 wire format applied to deltas: small (even negative) deltas
// become 1-2 bytes. Shifts are done on unsigned long long. T: O(n), S: O(n).
vector<uint8_t> encode(const vector<long long>& ts) {
    vector<uint8_t> bytes;
    long long prev = 0;
    for (long long cur : ts) {
        long long delta = cur - prev;
        prev = cur;
        // >> 63 on the signed value is all ones for negatives, 0 otherwise
        unsigned long long zigzag = ((unsigned long long)delta << 1)
                                    ^ (unsigned long long)(delta >> 63);
        while (zigzag >= 0x80) {
            bytes.push_back((zigzag & 0x7F) | 0x80);
            zigzag >>= 7;
        }
        bytes.push_back(zigzag);
    }
    return bytes;
}

vector<long long> decode(const vector<uint8_t>& bytes) {
    vector<long long> ts;
    long long prev = 0;
    // varint being assembled, and where its next 7-bit group goes
    unsigned long long zigzag = 0;
    int shift = 0;
    for (uint8_t byte : bytes) {
        zigzag |= (unsigned long long)(byte & 0x7F) << shift;
        shift += 7;
        if (byte & 0x80) { continue; }
        // undo zigzag: low bit set means negative
        long long delta = (long long)(zigzag >> 1) ^ -(long long)(zigzag & 1);
        prev += delta;
        ts.push_back(prev);
        zigzag = 0;
        shift = 0;
    }
    return ts;
}

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    check(encode({300}) == vector<uint8_t>{0xD8, 0x04}, "bytes-300");
    check(encode({5, 4}) == vector<uint8_t>{0x0A, 0x01}, "bytes-neg-delta");
    vector<long long> ts = {1730800000000, 1730800000033, 1730800000020, 0};
    check(decode(encode(ts)) == ts, "roundtrip-epoch-ms");
    check(decode(encode({})).empty(), "roundtrip-empty");
}

// follow-up: fixed-rate sensors (deltas all ~33 ms) -> delta-of-delta (Gorilla), most
//   values become zigzag 0 = one byte, or pack them in a few bits each
// follow-up: corrupt / truncated input -> stop with an error if shift >= 64 or the last
//   byte still has the high bit set
