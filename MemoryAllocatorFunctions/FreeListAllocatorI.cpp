// Simple free-list allocator (teaching version)
#include <cstdint>
#include <cstddef>
#include <cassert>
#include <iostream>
#include <utils.hpp>

struct FreeBlock {
    size_t size;       
    FreeBlock* next;  
};

static constexpr size_t ARENA_SIZE = 1024 * 64;
static alignas(std::max_align_t) char arena[ARENA_SIZE];

static FreeBlock* free_head = nullptr;
static bool initialized = false;

void init_simple_allocator() {
    if (initialized) return;
    free_head = reinterpret_cast<FreeBlock*>(arena);
    free_head->size = ARENA_SIZE;
    free_head->next = nullptr;
    initialized = true;
}

void* simple_malloc(size_t user_size) {
    init_simple_allocator();
    size_t alignment = alignof(std::max_align_t);
    // payload must be aligned. header size rounded up too
    size_t header_size = align_up(sizeof(FreeBlock), alignment);
    size_t needed = align_up(user_size, alignment) + header_size;

    FreeBlock* prev = nullptr;
    FreeBlock* curr = free_head;
    while (curr) {
        if (curr->size >= needed) {
            // found a block; if big enough, split, simple hai - first fit types
            if (curr->size >= needed + header_size + 16) {
                // split
                char* block_mem = reinterpret_cast<char*>(curr);
                FreeBlock* split = reinterpret_cast<FreeBlock*>(block_mem + needed);
                split->size = curr->size - needed;
                split->next = curr->next;
                if (prev) prev->next = split; else free_head = split;
                curr->size = needed;
            } else {
                // exact-ish, remove from free list
                if (prev) prev->next = curr->next;
                else free_head = curr->next;
            }
            // payload pointer is just after header
            char* payload = reinterpret_cast<char*>(curr) + header_size;
            return static_cast<void*>(payload);
        }
        prev = curr;
        curr = curr->next;
    }
    return nullptr;
}

void simple_free(void* p) {
    if (!p) return;
    size_t alignment = alignof(std::max_align_t);
    size_t header_size = align_up(sizeof(FreeBlock), alignment);
    char* b = reinterpret_cast<char*>(p) - header_size;
    FreeBlock* block = reinterpret_cast<FreeBlock*>(b);
    // no coalescing
    block->next = free_head;
    free_head = block;
}

// Demo
int main() {
    init_simple_allocator();
    void* a = simple_malloc(32);
    void* b = simple_malloc(64);
    void* c = simple_malloc(100);
    std::cout << "Allocated offsets: " << (char*)a - arena << ", " << (char*)b - arena << ", " << (char*)c - arena << "\n";
    simple_free(b);
    void* d = simple_malloc(48); // may reuse b's block, lol
    std::cout << "d offset: " << (char*)d - arena << "\n";
}
