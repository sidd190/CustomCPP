#include <iostream>
#include <cstdint>
#include <string.h>

//For memory overlap, we use memmove, an example on why it doesn't work with memcpy is-
// src:  0x100..0x10F: A B C D E F G H ...
// dest: 0x104..0x113 -> overlaps
// If we copy forward byte-by-byte we might overwrite data we later need.


// Naive implementation
void *memcpy_naive(void *dest, const void *src, std::size_t n) {
    if (dest == src || n == 0) return dest;
    unsigned char *d = static_cast<unsigned char*>(dest);
    const unsigned char *s = static_cast<const unsigned char*>(src);
    for (std::size_t i = 0; i < n; ++i) d[i] = s[i];
    return dest;
}
//optimised implementation
void *memcpy_optimised(void *dest, const void *src, std::size_t n){
    if(dest == src || n == 0) return dest;
    
    unsigned char *d = static_cast<unsigned char*>(dest);
    const unsigned char *s = static_cast<const unsigned char*>(src);

    using word_t = uintptr_t;
    constexpr std::size_t W = sizeof(word_t);

    // If both pointers have the same alignment mod W, we can do word copies
    if ((reinterpret_cast<uintptr_t>(d) % W) == (reinterpret_cast<uintptr_t>(s) % W)) {
        // copy bytes to align dest
        while (n && (reinterpret_cast<uintptr_t>(d) % W)) {
            *d++ = *s++; --n;
        }
        // word copies
        word_t* dw = reinterpret_cast<word_t*>(d);
        const word_t* sw = reinterpret_cast<const word_t*>(s);
        while (n >= W) {
            *dw++ = *sw++;
            n -= W;
        }
        // update pointers to tail
        d = reinterpret_cast<unsigned char*>(dw);
        s = reinterpret_cast<const unsigned char*>(sw);
    }

    // tail bytes (or all bytes if alignments differ)
    while (n--) *d++ = *s++;
    return dest;
}

struct POD {
    int a;
    double b;
};

int main() {
    char s[] = "Hello, world!";
    char d[20];
    memcpy_optimised(d, s, strlen(s)+1);
    std::cout << "d: " << d << '\n';
    return 0;

     // POD copy using memcpy (allowed)
    POD p1{42, 3.14};
    POD p2;
    // memcpy_optimised(&p2, &p1, sizeof p1); // allowed for POD / trivially copyable
    memcpy(&p1, &p2, sizeof(p1));
    std::cout << p2.a << ", " << p2.b << '\n';

    //can also check whether it's trivially copyable or naah - 
    // #include <type_traits>
    // std::is_trivially_copyable<T>::value
}