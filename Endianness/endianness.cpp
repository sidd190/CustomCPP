#include <iostream>

enum Endianness{
    little_Endian,
    big_Endian
};

Endianness detectEndianness() {
    unsigned int x = 0x1;
    char *c = (char*)&x;
    return (*c) ? little_Endian : big_Endian;
}

//Conditional byte swapping
// unsigned int swapEndian(unsigned int num) {
//     unsigned int b0 = (num & 0x000000FF) << 24;
//     unsigned int b1 = (num & 0x0000FF00) << 8;
//     unsigned int b2 = (num & 0x00FF0000) >> 8;
//     unsigned int b3 = (num & 0xFF000000) >> 24;
//     return b0 | b1 | b2 | b3;
// }

//Generic byte swapping 
void swapEndian(void* data, size_t size) {
    char* bytes = (char*)data;
    for (size_t i = 0; i < size / 2; ++i) {
        char temp = bytes[i];
        bytes[i] = bytes[size - 1 - i];
        bytes[size - 1 - i] = temp;
    }
}



int main() {
    Endianness system = detectEndianness();
    std::cout << "System Endianness: " 
              << ((system == little_Endian) ? "Little" : "Big") << "\n";

    unsigned int num = 0x12345678;
    std::cout << "Original: 0x" << std::hex << num << "\n";

    swapEndian(&num, sizeof(num));
    std::cout << "Converted to Big Endian: 0x" << std::hex << num << "\n";

    swapEndian(&num, sizeof(num));
    std::cout << "Converted back to Little Endian: 0x" << std::hex << num << "\n";

    return 0;
}