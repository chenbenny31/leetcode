// 04 · Register Bit Ops (Cisco embedded, Easy) — masks on a 32-bit register
// (1) set / clear / toggle / test bit i of a register
// (2) read the field of `width` bits starting at bit `pos`; write a value into it
//
// clarify:
// bit 0 -> least significant bit
// i, pos in 0..31, 1 <= width <= 31, pos + width <= 32 -> assume valid, no checks
// value wider than the field -> extra high bits are dropped
// return the new value or write through a pointer -> return (see follow-up for MMIO)

#include <iostream>
#include <string>
#include <cstdint>

using std::cout;
using std::string;
using std::uint32_t;

// ==================== (1) ====================
// set / clear / toggle / test bit i

// core idea: mask = 1u << i; OR sets, AND ~mask clears, XOR flips
// T: O(1), S: O(1)
uint32_t setBit(uint32_t reg, int i) { return reg | (1u << i); }
uint32_t clearBit(uint32_t reg, int i) { return reg & ~(1u << i); }
uint32_t toggleBit(uint32_t reg, int i) { return reg ^ (1u << i); }
bool testBit(uint32_t reg, int i) { return ((reg >> i) & 1u) != 0; }

// ==================== (2) ====================
// read / write the `width`-bit field that starts at bit `pos`

// core idea: read = shift down then mask; write = clear the field, then OR the value in
// T: O(1), S: O(1)
uint32_t getField(uint32_t reg, int pos, int width) {
    return (reg >> pos) & ((1u << width) - 1);
}

uint32_t setField(uint32_t reg, int pos, int width, uint32_t val) {
    // ones exactly over the field
    uint32_t mask = ((1u << width) - 1) << pos;
    return (reg & ~mask) | ((val << pos) & mask);
}

// ==================== tests ====================

void test(const string& label, uint32_t got, uint32_t want) {
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("set-clear", setBit(clearBit(0b1010, 3), 0), 0b0011);
    test("toggle-test", testBit(toggleBit(0b1010, 3), 3), 0);
    test("get-field", getField(0xABCD1234, 8, 8), 0x12);
    test("set-field-truncates", setField(0xABCD1234, 8, 4, 0x1F), 0xABCD1F34);
    return 0;
}

// follow-up:
// 1. memory-mapped register -> volatile uint32_t* reg = (volatile uint32_t*)addr
// 2. why volatile -> hardware changes the value; compiler must not cache or drop accesses
// 3. reg |= mask also done in an ISR -> read-modify-write race: disable IRQs around it
// 4. write-1-to-clear status bit -> write only the mask (reg = mask), not reg |= mask
// 5. power of two (LC 231, reported) -> x != 0 && (x & (x - 1)) == 0
// 6. count / reverse bits (LC 191 / 190) -> select/16-bit-math-matrix/04-bit-ops
// 7. swap byte order -> ref/recommended/25-endian-fixed-width
// 8. C macros -> #define BIT(n) (1u << (n)); parenthesize every argument
// 9. width == 32 -> 1u << 32 is undefined behavior; use mask = ~0u for that case
