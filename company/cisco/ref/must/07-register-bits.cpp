// 07 · Register Bit Ops (Cisco embedded, Easy) — masks on a 32-bit register
// (1) set / clear / toggle / test bit i of a register
// (2) read the field of `width` bits starting at bit `pos`; write a value into it
//
// clarify:
// bit 0 -> least significant bit
// i, pos in 0..31, pos + width <= 32 -> assume valid, no checks
// value wider than the field -> extra high bits are dropped
// return the new value or write through a pointer -> return (see follow-up for MMIO)

#include <iostream>
#include <string>
#include <cstdint>

using std::cout;
using std::string;
using std::uint32_t;

// (1) single bit
// core idea: mask = 1u << i; OR sets, AND ~mask clears, XOR flips
// T: O(1), S: O(1)
uint32_t setBit(uint32_t reg, int i) { return reg | (1u << i); }
uint32_t clearBit(uint32_t reg, int i) { return reg & ~(1u << i); }
uint32_t toggleBit(uint32_t reg, int i) { return reg ^ (1u << i); }
bool testBit(uint32_t reg, int i) { return ((reg >> i) & 1u) != 0; }

// `width` low bits set
uint32_t lowMask(int width) {
    // 1u << 32 is undefined behavior, so a full-width field is its own case
    if (width >= 32) { return ~0u; }
    return (1u << width) - 1;
}

// (2) field
// core idea: read = shift down then mask; write = clear the field, then OR the value in
// T: O(1), S: O(1)
uint32_t getField(uint32_t reg, int pos, int width) {
    return (reg >> pos) & lowMask(width);
}

uint32_t setField(uint32_t reg, int pos, int width, uint32_t val) {
    // ones exactly over the field
    uint32_t mask = lowMask(width) << pos;
    return (reg & ~mask) | ((val << pos) & mask);
}

void test(const string& label, uint32_t got, uint32_t want) {
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("set", setBit(0b1000, 1), 0b1010);
    test("clear", clearBit(0b1010, 3), 0b0010);
    test("toggle", toggleBit(0b1010, 0), 0b1011);
    test("test", testBit(0b1010, 3), 1);
    test("get-field", getField(0xABCD1234, 8, 8), 0x12);
    test("set-field", setField(0xABCD1234, 8, 8, 0xFF), 0xABCDFF34);
    test("set-field-truncates", setField(0, 4, 4, 0x1F), 0xF0);
    test("full-width", getField(0xABCD1234, 0, 32), 0xABCD1234);
    return 0;
}

// follow-up:
// memory-mapped register -> go through a volatile pointer, see the block below
// why volatile -> hardware changes the value; compiler must not cache or drop accesses
// reg |= mask is read-modify-write, not atomic -> an ISR's write in between is lost;
//   disable IRQs around it
// write-1-to-clear status bit -> write only the mask (reg = mask), not reg |= mask
// power of two (LC 231, reported) -> x != 0 && (x & (x - 1)) == 0
// count set bits (LC 191) / reverse bits (LC 190) -> select/16-bit-math-matrix/04-bit-ops
// swap byte order -> company/scientech/ref/recommended/07-endian-fixed-width.cpp
// C macros -> #define BIT(n) (1u << (n)); parenthesize every argument
/*
volatile uint32_t* ctrl = (volatile uint32_t*)0x40021000;
*ctrl |= (1u << 3);
*ctrl = (*ctrl & ~(0xFu << 4)) | (0x5u << 4);
*/
