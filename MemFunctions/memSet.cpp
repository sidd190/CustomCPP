// |                   Modern Implementation Strategies                                                              |
// | ------------------ | ------------------------------------------------------------------------------------------ |
// |   glibc `memset`   | Assembly optimized for x86-64, targeting SSE2, AVX2, AVX-512 with dynamic dispatch         |
// |   GCC/Clang        | Auto-vectorization, loop unrolling/interleaving, alignment-aware codegen, cost-model based |
// |   Developer        | Override with pragmas (`simd`, loop vectorize), use `restrict`, or explicit intrinsics     |
// -------------------------------------------------------------------------------------------------------------------
// These Real memset implementations do additional micro-optimizations (loop unrolling; non-temporal stores; vector stores).

//  What memset does is -> {void *memset(void *s, int c, size_t n)}
//  fills n bytes of memory with value c at memory starting at s
//          0x100: [   ] [   ] [   ] [   ] ...
//  address ↑      ^s
//  memset(s, 0xAA, 6) -> stores 0xAA into 6 consecutive bytes

// Now there can be multiple implementations for it but those that I do here are - Naive / Optimised

#include <iostream>
#include <cstdint>

// I'd like to take an example of painting a wall, through memset, we paint a wall of size n, with color c, starting from point s

// Naive Implementation -> takes a starting point and just paints the wall byte by byte at once
void *memset_naive(void *s, int c, std::size_t n){
    unsigned char *p = static_cast<unsigned char*>(s);
    unsigned char byte = static_cast<unsigned char>(c);
    for(std::size_t i = 0; i < n; ++i) p[i] = byte;
    return s;
}

// Optimised Implementation (optimised on my part) -> gets a starting point and checks if n = 0 -> returns s, job done
// If not, finds the natural byte size, paints byte by byte till it finds a straight line, then kaboom paints a big chunk at once 
// and finally paints the leftover spots
void *memset_optimised(void *s, int c, std::size_t n){
    unsigned char *p = static_cast<unsigned char*>(s);
    if(n==0) return s;
    
    // To get a natural machine word type
    using word_t = uintptr_t;
    constexpr std::size_t W = sizeof(word_t);
    unsigned char byte = static_cast<unsigned char>(c);

    // Align to word boundary by bytes
    while(n && (reinterpret_cast<uintptr_t>(p) % W != 0)){
        *p++ = byte;
        --n;
    }

    if(n>=W){
        //build the repeated byte pattern in word_t
        word_t pattern = 0;
        for(std::size_t i = 0; i<W ; ++i ){
            pattern = (pattern <<8 ) | word_t(byte);
        }

        word_t *wp = reinterpret_cast<word_t*>(p);
        std::size_t words = n/W;

        //Maybe will optimise further (by unrolling this rool)
        while(words--){
            *wp++ = pattern;
        }

        p = reinterpret_cast<unsigned char*>(wp);
        n %= W;
    }

    // tail bytes
    while (n--) *p++ = byte;
    return s;
}

int main() {
    char x[20];
    memset_naive(x, 0xFF, 20);
    memset_optimised(x, 0xFF, 20);
    return 0;
}
