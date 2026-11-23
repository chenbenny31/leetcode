// 31 · embedded basics list
// copy of `company/scientech/ref/recommended/13-block-pool.cpp`
// memory pool: all memory is taken once at start, then allocate / deallocate of a
// fixed-size block are O(1) and never call malloc (no surprise latency on the hot path)

// usage:
// g++ -std=c++17 -Wall -Wextra 13-block-pool.cpp -o a && ./a
//
// BlockPool pool(sizeof(Order), 1024);
// Order* order = new (pool.allocate()) Order{...};
// order->~Order();  pool.deallocate(order);

// clarify / assumptions
// every block has the same size; the num of blocks is fixed at construction
// pool is used up -> allocate returns nullptr, the caller decides what to do
// one thread; the caller gives back each block once, and only blocks from this pool

// data:
// memory: one array of numBlocks * blockSize bytes
// free list: each free block stores a pointer to the next free block in its own bytes,
//            so the list costs no extra memory; freeHead = first free block

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <new>
#include <string>
#include <iostream>

using std::int64_t;
using std::max;
using std::max_align_t;
using std::size_t;
using std::uint8_t;
using std::cout;
using std::string;

class BlockPool {
private:
    struct FreeNode {
        FreeNode* next; // next free block, nullptr = last one
    };

    size_t blockSize; // bytes per block
    size_t numBlocks;
    uint8_t* memory; // all blocks, back to back
    FreeNode* freeHead; // first free block, nullptr = pool is used up
public:
    BlockPool(size_t objSize, size_t count) {
        // a free block must fit a FreeNode; round up so every block starts aligned
        size_t align = alignof(max_align_t);
        blockSize = max(objSize, sizeof(FreeNode));
        blockSize = (blockSize + align - 1) / align * align;
        numBlocks = count;
        memory = new uint8_t[blockSize * numBlocks];

        // put every block on the free list, last block first -> block 0 ends on top
        freeHead = nullptr;
        for (size_t i = numBlocks; i > 0; i--) {
            deallocate(memory + (i - 1) * blockSize);
        }
    }

    ~BlockPool() {
        delete[] memory;
    }

    // a copy would free the same memory twice
    BlockPool(const BlockPool&) = delete;
    BlockPool& operator=(const BlockPool&) = delete;

    // take the first free block, nullptr if none is left, T: O(1)
    void* allocate() {
        if (freeHead == nullptr) { return nullptr; }

        FreeNode* node = freeHead;
        freeHead = node->next;
        return node;
    }

    // put the block back on top of the free list, T: O(1)
    void deallocate(void* ptr) {
        FreeNode* node = new (ptr) FreeNode; // the block's own bytes hold the link
        node->next = freeHead;
        freeHead = node;
    }
};

struct Order {
    int id;
    int64_t price;
    int64_t qty;
};

void check(const string& label, bool ok) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    BlockPool pool(sizeof(Order), 2);
    void* first = pool.allocate();
    void* second = pool.allocate();
    check("two-blocks", first != nullptr && second != nullptr && first != second);
    check("used-up", pool.allocate() == nullptr);

    pool.deallocate(first);
    check("reuse-last-freed", pool.allocate() == first);

    // build an object inside a block, then destroy it and give the block back
    Order* order = new (first) Order{7, 100, 5};
    check("placement-new", order->id == 7 && order->price == 100 && order->qty == 5);
    order->~Order();
    pool.deallocate(order);
    pool.deallocate(second);

    int count = 0;
    void* block = pool.allocate();
    while (block != nullptr) {
        count++;
        block = pool.allocate();
    }
    check("all-back", count == 2);
}
