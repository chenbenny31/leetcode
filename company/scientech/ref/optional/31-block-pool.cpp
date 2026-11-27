// 31 · Fixed-size memory pool — quant core (not reported)
// all memory is taken once at start; allocate / deallocate of a fixed-size block are
// then O(1) and never call malloc (no surprise latency on the hot path)
//
// clarify:
// blocks -> all the same size; their num is fixed at construction
// pool used up -> allocate returns nullptr, the caller decides what to do
// threads -> one; the caller returns each block once, and only blocks of this pool

#include <iostream>
#include <string>
#include <algorithm>
#include <cstddef>
#include <new>

using std::cout;
using std::string;
using std::max;
using std::max_align_t;
using std::size_t;

// core idea: one array of numBlocks * blockSize bytes; each free block stores a ptr to
// the next free block in its own bytes, so the free list costs no extra memory
// T: O(1) allocate / deallocate, S: O(numBlocks * blockSize)
class BlockPool {
    struct FreeNode {
        FreeNode* next; // next free block, nullptr = last one
    };

    size_t blockSize;      // bytes per block
    size_t numBlocks;
    unsigned char* memory; // all blocks, back to back
    FreeNode* freeHead;    // first free block, nullptr = pool is used up

public:
    BlockPool(size_t objSize, size_t count) {
        // a free block must fit a FreeNode; round up so every block starts aligned
        size_t align = alignof(max_align_t);
        blockSize = max(objSize, sizeof(FreeNode));
        blockSize = (blockSize + align - 1) / align * align;
        numBlocks = count;
        memory = new unsigned char[blockSize * numBlocks];

        // last block first, so block 0 ends on top of the free list
        freeHead = nullptr;
        for (size_t i = numBlocks; i > 0; i--) {
            deallocate(memory + (i - 1) * blockSize);
        }
    }

    ~BlockPool() { delete[] memory; }

    // a copy would free the same memory twice
    BlockPool(const BlockPool&) = delete;
    BlockPool& operator=(const BlockPool&) = delete;

    // take the first free block, nullptr if none is left
    void* allocate() {
        if (freeHead == nullptr) { return nullptr; }
        FreeNode* node = freeHead;
        freeHead = node->next;
        return node;
    }

    // put the block back on top of the free list
    void deallocate(void* ptr) {
        FreeNode* node = new (ptr) FreeNode; // the block's own bytes hold the link
        node->next = freeHead;
        freeHead = node;
    }
};

struct Order {
    int id;
    long long price;
    long long qty;
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    BlockPool pool(sizeof(Order), 2);
    void* first = pool.allocate();
    void* second = pool.allocate();
    check(first != nullptr && second != nullptr && first != second, "two-blocks");
    check(pool.allocate() == nullptr, "used-up");

    pool.deallocate(first);
    check(pool.allocate() == first, "reuse-last-freed");

    Order* order = new (first) Order{7, 100, 5};
    check(order->id == 7 && order->price == 100 && order->qty == 5, "placement-new");
    order->~Order();
    pool.deallocate(order);
    pool.deallocate(second);

    int count = 0;
    while (pool.allocate() != nullptr) { count++; }
    check(count == 2, "all-back");
}

// follow-up:
// 1. typed pool -> template on T: create(args) = placement new, destroy = dtor + free
// 2. pool used up -> chain a second chunk of blocks instead of returning nullptr
// 3. threads -> a mutex, or one pool per thread; lock-free needs CAS + an ABA tag
// 4. double free / foreign ptr -> debug check: range, block alignment, a used bitmap
// 5. why it is fast -> no size search, no system call, blocks stay close in the cache
// 6. variable sizes -> one pool per size class (what malloc's small bins do)
