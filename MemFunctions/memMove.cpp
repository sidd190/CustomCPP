#include <iostream>

// Non-overlapping or dest < src — safe to copy forward:
// src: [S0][S1][S2][S3] ...
// dest:   [D0][D1][D2] ...
// dest < src: copy from low addresses ascending.

// Overlap where dest > src — must copy backward:
// src (0x100): A B C D E F
// dest (0x102):   ^ overlaps starting at src+2
// If we copy forward, byte at dest[0] = src[0] will overwrite src[2] before it's read.
// Solution: copy from the end backward.

// Implementation (safe & simple)
void* memmove(void* dest, const void* src, std::size_t n) {
    if (dest == src || n == 0) return dest;
    unsigned char* d = static_cast<unsigned char*>(dest);
    const unsigned char* s = static_cast<const unsigned char*>(src);

    if (d < s) {
        // non-overlapping or dest starts before src: forward copy okay
        for (std::size_t i = 0; i < n; ++i) d[i] = s[i];
    } else {
        // dest >= src: copy backward
        for (std::size_t i = n; i-- > 0; ) d[i] = s[i];
    }
    return dest;
}

// Further steps ---

// OPTIMISED MEMMOVE
// For forward copy you can use the same word-copy technique as memcpy when alignment permits.
// For backward copy, align the end and copy whole words backward (be careful with pointer arithmetic).
// Some implementations combine memcpy-like fast-paths for non-overlap and a backward-word copy for overlap.

// BACKWORD WORD-COPY SKETCH:
// Move d and s pointers to the end positions: d += n; s += n.
// Copy tail-bytes until both pointers aligned.
// Copy words backward (--dw; --sw; *dw = *sw;) etc.
// Copy remaining head bytes.

int main() {
    char o[] = "1234567890";
    // move "345678" to start
    memmove(o, o+2, 6);
    std::cout << "overlap result: " << o << '\n'; // more supreme than memcpy ig?
    return 0;
}