# Mem* Functions

![Mem Functions](memfuncs.png)

This covers memset, memcpy, memcmp, and memmove from C/C++. I learnt and tried to share about what they do, how they work at byte level, custom code from simple to faster versions, diagrams, C++ object stuff, and tips.

## Basics
These funcs handle raw bytes, not C++ objects. They use unsigned char for safe access.

- memset(void *s, int c, size_t n): Fills n bytes at s with byte c.
- memcpy(void *dest, const void *src, size_t n): Copies n bytes from src to dest. Crashes if overlap.
- memmove(void *dest, const void *src, size_t n): Copies safely even with overlap.
- memcmp(const void *s1, const void *s2, size_t n): Compares n bytes; returns <0, 0, >0.

Notes: No constructors/destructors. Good for simple types (trivially copyable), bad for complex objects.

#### A non-trivial object is any type that:

 - Has a user-defined constructor, copy constructor, move constructor, or destructor.
 - Has virtual functions (→ uses a vtable internally).
 - Has base classes or virtual inheritance.
 - Manages internal resources (heap pointers, file descriptors, locks, etc.).

## Code
Naive: Byte loops.
Optimized: Word copies, align pointers, handle tails.
- memset: Pattern fill.
- memcpy: Align if same mod word size.
- memmove: Forward/backward based on overlap.
- memcmp: Word compare, then byte diff.

## Few Gotchas
- Don't use on non-trivial objects—UB.
- Aliasing: unsigned char is safe.
- Overlap: memcpy no, memmove yes.
- memcmp: Not constant-time for crypto.
- memset(0): Not always zero-init.

## Real Stuff
- Libs use SIMD, unrolling, CPU checks and optimisations on the assembly level.
- Loop unrolling (copy 8 words per iteration).
- Non-temporal stores to avoid polluting caches for huge copies.
- Prefetch to hide memory latency.
- Using size_t/uintptr_t as word type for best throughput.
- Using memcmp optimizations: compare words and then search for differing byte using bitwise operations (e.g., find first set bit in XOR result).

These are used everywhere in systems programming, embedded systems, game dev,etc majorly for styff like initializing and clearing buffers, moving data where speed matters even in Linux kernels. 

*Very important stuff at the core of a lot of other important stuff imo*

## Tips
- Use for bytes or simple structs.
- For objects: Constructors or std::copy.
- Memmove if overlap maybe.
- Lib versions are faster—use them.
