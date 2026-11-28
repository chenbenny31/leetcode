// usage:
// g++ -std=c++17 -Wall -Wextra codec.cpp -o codec && ./codec
//
// non-neg ints -> UnsignedCodec::encode, UnsignedCodec::decode
// signed ints -> SignedCodec::encode, SignedCodec::decode


// clarify / assumptions
// int width up to 64-bit

// refer to read(), write() logic
// encode: val, buf, cap -> write coded into buf, return nwrite size, -1 if fail
// decode: buf, len, val -> read coded from buf, update val&, return read size, -1 if fail

// bit-ops:
// & 0111 1111 = & 0x7f -> get lower 7 bit
// | 1000 0000 = | 0x80 -> set top bit (MSB)

#include <cstdint>
#include <climits>
#include <string>
#include <vector>
#include <iostream>

using std::uint64_t;
using std::uint8_t;
using std::int64_t;
using std::cout;
using std::string;
using std::vector;

// uint64 takes 8 bytes, drop leading 0's, MSB=1 mean more bytes folow
// e.g. 558 = 0000100 0101110 -> 1000 0100, 0010 1110 = 0x842E
class UnsignedCodec {
private:
    static constexpr int DATA_MASK = 0x7f; // lower 7 bits
    static constexpr int MSB_BIT = 0x80; // top bit of byte
public:
    // count 7-bit chunks, write from high to low bits
    // return num of bytes nwrite, -1 if fail, T: O(n), S: O(1), n = num of bytes
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

    // keep shift 7 bits and accum chunks, stop at first byte with MSB = 0
    // return num of bytes read, -1 if fail, T: O(n), S: O(1)
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

// for signed int, -1's two complement will be 64 1's, no prefix 0's to save
// need a way to mapping negative int to less 1's representation
// refer to ProtoBuf: zigzag mapping, (pos -> even, neg -> odd), pass to UnsignedCodec
class SignedCodec {
private:
    // signed -> unsigned mapping, 0, -1, 1, -2, 2 -> 0, 1, 2, 3, 4
    static uint64_t zigzag(int64_t num) {
        if (num >= 0) { // if non-neg, cast and double
            uint64_t x = num;
            return 2 * x;
        }

        // if negative, ordered cast, then double + 1
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

void testUnsigned(const string& label, uint64_t val, const vector<uint8_t>& want) {
    uint8_t buf[10]; // 64 / 7 + 1
    int nwrite = UnsignedCodec::encode(val, buf, 10);
    vector<uint8_t> got(buf, buf + nwrite);

    uint64_t res = 0;
    int nread = UnsignedCodec::decode(buf, nwrite, res);
    bool ok = (got == want && nread == nwrite && res == val);

    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void testSigned(const string& label, int64_t val, const vector<uint8_t>& want) {
    uint8_t buf[10];
    int nwrite = SignedCodec::encode(val, buf, 10);
    vector<uint8_t> got(buf, buf + nwrite);

    int64_t res = 0;
    int nread = SignedCodec::decode(buf, nwrite, res);
    bool ok = (got == want && nread == nwrite && res == val);

    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void check(const string& label, bool ok) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    // test valid cases
    testUnsigned("0", 0, {0x00});
    testUnsigned("max-1byte", 127, {0x7F});
    testUnsigned("min-2bytes", 128, {0x81, 0x00});
    testUnsigned("558", 558, {0x84, 0x2E});
    testUnsigned("UINT64_MAX", UINT64_MAX,
                 {0x81, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F});

    testSigned("-1", -1, {0x01});
    testSigned("+1", 1, {0x02});
    testSigned("-65", -65, {0x81, 0x01});
    testSigned("INT64_MIN", INT64_MIN,
               {0x81, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F});

    // test on following bytes
    uint8_t two[] = {0x84, 0x2E, 0x05}; // 558 + following
    uint64_t val = 0;
    check("stop-follow", UnsignedCodec::decode(two, 3, val) == 2 && val == 558);


    // test invalid cases
    uint8_t buf[10];
    uint8_t cut[] = {0x84}; // 1000 0100 MSB=1, no more bytes
    val = 0;
    int64_t sval = 0;
    check("cap<count", UnsignedCodec::encode(558, buf, 1) == -1);
    check("unsigned-cut", UnsignedCodec::decode(cut, 1, val) == -1);
    check("len=0", UnsignedCodec::decode(cut, 0, val) == -1);
    check("signed-cut", SignedCodec::decode(cut, 1, sval) == -1);
}
