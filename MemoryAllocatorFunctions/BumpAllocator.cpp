#include <cstddef>
#include <cstdint>
#include <cassert>
#include <iostream>
#include <new>

class BumpAllocator{
        char *start_;
        char *ptr_;
        char *end_;
    public:
        BumpAllocator(void *mem, size_t size){
            start_ = static_cast<char *> (mem);
            ptr_ = start_;
            end_ = start_ + size;
        }    

        void *allocate(size_t bytes, size_t alignment = alignof(std::max_align_t)){
            uintptr_t cur = reinterpret_cast<uintptr_t>(ptr_);
            uintptr_t aligned = (cur+alignment-1) & ~(alignment - 1);
            char *out = reinterpret_cast<char*>(aligned);
            if(out + bytes > end_) throw std::bad_alloc{};
            ptr_ = out+bytes;
            return out;
        }
        
        void reset() {
            ptr_ = start_;
        }
};


int main() {
    constexpr size_t ARENA = 1024;
    static char arena_buffer[ARENA];
    BumpAllocator alloc(arena_buffer, ARENA);

    int *a = (int*)alloc.allocate(sizeof(int));
    *a = 1243;
    double *d = (double*)alloc.allocate(sizeof(double),alignof(double));
    *d = 1290.023;
    std::cout << "arena_buf=" << (void*)arena_buffer
          << " a=" << (void*)a
          << " d=" << (void*)d << "\n";
    alloc.reset();
    std::cout << "arena_buf=" << (void*)arena_buffer
          << " a=" << (void*)a
          << " d=" << (void*)d << "\n";
    return 0;
}