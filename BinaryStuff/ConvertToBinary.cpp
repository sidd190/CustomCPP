#include <iostream>
#include <string.h>


void print_bits_u64(u_int64_t v, int bits){
    for (int i = bits-1; i >=0; --i)
    {
        putchar(((v>>i) & 1ULL)? '1' : '0');
        if(i%4 == 0 && i != 0) putchar(' ');
    }
    putchar('\n');    
}

void print_unsigned_by_size(const void *ptr, size_t size){
    u_int64_t v =0;
    if(size==1) v = *(const u_int8_t *)ptr;
    else if(size==2) v = *(const u_int16_t *)ptr;
    else if(size==4) v = *(const u_int32_t *)ptr;
    else if(size==8) v = *(const u_int64_t *)ptr;
    else{
        //this is the fallback if we provide non generic sizes {follows little endian system for assembling shi}
        const unsigned char *b = (const unsigned char*)ptr;
        v=0;
        for(size_t i =0; i<size && i<8; ++i){
            v |= ((u_int64_t)b[i])<<(8*i);
        }
    }
    print_bits_u64(v, (int)size* 8);
}

void print_signed_by_size(const void *ptr, size_t size){
    int64_t s = 0;
    if(size == 1) s = *(const int8_t *) ptr;
    if(size == 2) s = *(const int16_t *) ptr;
    if(size == 4) s = *(const int32_t *) ptr;
    if(size == 8) s = *(const int64_t *) ptr;
    u_int64_t v = (u_int64_t)s;
    print_bits_u64(v, (int)size*8);
}

void print_float_bits(float f){
    u_int32_t bits;
    memcpy(&bits,&f,sizeof(bits));
    print_bits_u64(bits, 32);
    unsigned sign = (bits >> 31) & 0x1u;
    unsigned exp = (bits >> 23) & 0xFFu;
    unsigned mant = bits & 0x7FFFFFu;
    printf(" sign=%u exp=0x%02x mantissa=0x%06x\n",sign,exp,mant);
}

void print_double_bits(double d){
    u_int64_t bits;
    mempcpy(&bits,&d,sizeof(bits));
    print_bits_u64(bits, 64);
    unsigned sign = (unsigned)((bits>>63) & 1ULL);
    unsigned exp = (unsigned)((bits>>52) & 0x7FFULL);
    u_int64_t mant = bits & 0xFFFFFFFFFFFFFULL;
    printf(" sign=%u exp=0x%x mantissa=0x%llx\n", sign, exp, (unsigned long long)mant);    
}

int is_little_endian(void) {
    u_int32_t x = 0x01020304;
    unsigned char *b = (unsigned char*)&x;
    return b[0] == 0x04; // if first byte is LSB (0x04), we are little-endian
}

void print_memory_bytes(const void *ptr, size_t size) {
    const unsigned char *b = (const unsigned char*)ptr;
    for (size_t i=0; i < size; ++i)
        printf("%02x ", b[i]);
    putchar('\n');
}

// To Trim leading zeros (print minimal bits):
// to be implemented properly -  a function that finds the highest set bit and prints from there (but always print at least 1 bit).
// int highest_bit_index(u_int64_t v) {
//     if (v == 0) return 0;
//     int idx = 0;
//     while (v >> idx) ++idx;
//     return idx - 1; // index of highest set bit
// }

// to be implemented properly - Count ones (popcount) with bitwise ops (Brian Kernighan’s):
// int popcount(u_int64_t v) {
//     int c = 0;
//     while (v) {
//         v &= v - 1; // clears lowest set bit
//         ++c;
//     }
//     return c;
// }


int main() {
    // u_int8_t a = 255;
    // u_int16_t b = 1024;
    // u_int32_t c = 65535;
    // print_unsigned_by_size(&a,1);
    // print_unsigned_by_size(&b,2);
    // print_unsigned_by_size(&c, 4);
    // print_signed_by_size(&c, 4);

    // int16_t d = -300; // Use signed 16-bit integer
    // print_unsigned_by_size(&d, 2);
    // print_signed_by_size(&d, 2);

    printf("=== Binary tools demo ===\n\n");

    u_int32_t n32 = 37;
    printf("uint32_t n = %u\nbits: ", n32);
    print_unsigned_by_size(&n32, sizeof(n32));
    putchar('\n');

    int32_t s32 = -5;
    printf("int32_t s = %d\nbits: ", s32);
    print_signed_by_size(&s32, sizeof(s32));
    putchar('\n');

    float f = 3.5f;
    printf("float f = %f\nbits: ", f);
    print_float_bits(f);
    putchar('\n');

    double d = -0.125;
    printf("double d = %f\nbits: ", d);
    print_double_bits(d);
    putchar('\n');

    printf("System endianness: %s-endian\n", is_little_endian() ? "Little" : "Big");

    u_int32_t x = 0x01020304;
    printf("Memory bytes of 0x01020304: ");
    print_memory_bytes(&x, sizeof(x));
    putchar('\n');


    return 0;
}