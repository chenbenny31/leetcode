// encode(val, buf, cap) -> nwrite, -1 if fail
// decode(buf, len, val) -> nread, -1 if fail

// &01111111 = &0x7f -> get lower 7 bits
// |10000000 = |0x80 -> set top bit (MSB)

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

class UnsignedCodec {
private:
    static constexpr int DATA_MASK = 0x7f;
    static constexpr int MSB_BIT = 0x80;

public:
    static int encode(uint64_t val, uint8_t* buf, int cap) {
        int count = 1; // num of 7-bit chunks
        uint64_t remain = val >> 7;
        while(remain != 0) {
            count++;
            remain >>= 7;
        }
        if (count > cap) { return -1; }

        int shift = 7 * (count - 1);
        for (int i = 0; i < count; i++) {
            uint8_t byte = (val >> shift) & DATA_MASK;
            if (i < count - 1) { byte |= MSB_BIT; }
            buf[i] = byte;
            shift -= 7;
        }
        return count;
    }

    static int decode(const uint8_t* buf, int len, uint64_t& val) {
        uint64_t result = 0;

        for (int i = 0; i < len; i++) {
            uint8_t byte = buf[i] & DATA_MASK;
            result << 7;
            result |= byte;

            if ((buf[i] & MSB_BIT) == 0) {
                val = result;
                return i + 1;
            }
        }
        return -1;
    }
};
