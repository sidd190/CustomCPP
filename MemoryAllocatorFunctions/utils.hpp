// no point exactly, just tried and figured out which to be used why and how
#pragma once
#include <cstddef>
#include <cstdint>
#include <cassert>

inline bool is_power_of_two(std::size_t x) { return x && ((x & (x - 1)) == 0); }

inline std::size_t align_up(std::size_t n, std::size_t align) {
    assert(is_power_of_two(align));
    return (n + (align - 1)) & ~(align - 1);
}

inline void* align_ptr(void* p, size_t align) {
    uintptr_t v = reinterpret_cast<uintptr_t>(p);
    uintptr_t av = (v + align - 1) & ~(uintptr_t)(align - 1);
    return reinterpret_cast<void*>(av);
}