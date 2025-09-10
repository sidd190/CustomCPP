#include <iostream>
#include <cstdint>

// Naive Implementation
int memcmp_naive(const void* s1, const void* s2, std::size_t n) {
    const unsigned char* a = static_cast<const unsigned char*>(s1);
    const unsigned char* b = static_cast<const unsigned char*>(s2);
    for (std::size_t i = 0; i < n; ++i) {
        if (a[i] != b[i]) return int(a[i]) - int(b[i]);
    }
    return 0;
}

// Optimized Implementation (word-wise)

// Compare word-sized blocks (uintptr_t) while they are equal.
// On the first differing word, locate the first differing byte inside that word (scan bytes in that word).
// This reduces loop overhead for long equal ranges.
int memcmp_optimised(const void* s1, const void* s2, std::size_t n) {
    const unsigned char* a = static_cast<const unsigned char*>(s1);
    const unsigned char* b = static_cast<const unsigned char*>(s2);

    using word_t = uintptr_t;
    constexpr std::size_t W = sizeof(word_t);

    // Align checks — only do word compare if both have same alignment modulo W
    if ((reinterpret_cast<uintptr_t>(a) % W) == (reinterpret_cast<uintptr_t>(b) % W)) {
        // align a (and b)
        while (n && (reinterpret_cast<uintptr_t>(a) % W)) {
            if (*a != *b) return int(*a) - int(*b);
            ++a; ++b; --n;
        }
        const word_t* aw = reinterpret_cast<const word_t*>(a);
        const word_t* bw = reinterpret_cast<const word_t*>(b);
        while (n >= W) {
            if (*aw != *bw) {
                // differing word — find the differing byte
                const unsigned char* aa = reinterpret_cast<const unsigned char*>(aw);
                const unsigned char* bb = reinterpret_cast<const unsigned char*>(bw);
                for (std::size_t i = 0; i < W; ++i) {
                    if (aa[i] != bb[i]) return int(aa[i]) - int(bb[i]);
                }
                // unreachable
            }
            ++aw; ++bw; n -= W;
        }
        a = reinterpret_cast<const unsigned char*>(aw);
        b = reinterpret_cast<const unsigned char*>(bw);
    }
    while (n--) {
        if (*a != *b) return int(*a) - int(*b);
        ++a; ++b;
    }
    return 0;
}



int main() {
    const char *x = "abcde";
    const char *y = "abCde";
    std::cout << "memcmp: " << memcmp_optimised(x, y, 5) << '\n';
    return 0;
}