// To-Do
// BASICS - 
// Implement my own DynamicArray<T>:
//  push_back, pop_back, insert, erase.
//  Auto-resize using your allocator.
// Implement a function to reverse an array in-place.
// Implement your own binary search (iterative + recursive).
// ADVANCE A bit - 
// Design a fixed-capacity circular buffer
// Lock-free single producer/consumer with atomic head/tail.
// Implement an arena allocator
// Use it to back your array and reset every frame.
// SIMD accelerate array sum
// Use AVX2 intrinsics to add floats in 256-bit chunks.
// Cache-Line Benchmark
// Compare sequential vs. random access of 1 GB array; measure L1/L2 miss rates.
// Memory Reclamation Stress
// Spawn threads doing push/pop at high rate, find false sharing & fix with padding.
#include <iostream>
#include <memory>
#include <utility>
#include <stdexcept>

template <typename T>

class DynamicArray{
    private:
        T *data_;
        size_t size_;
        size_t capacity_;

        T *allocate(size_t n){
            return static_cast<T*>(::operator new(sizeof(T) * n));
        }

        void deallocate(T *ptr, size_t n){
            ::operator delete(ptr, sizeof(T) * n);
        }

        void grow_if_needBe(){
            if(size_<capacity_) return;
            size_t newCap = capacity_ == 0? 1 : capacity_ * 2;
            T *newData = allocate(newCap);

            for(size_t i = 0; i < size_; i++){
                new(newData +1) T(std::move(data_[i]));
                data_[i].~T();
            }
            deallocate(data_, capacity_);
            data_ = newData;
            capacity_ = newCap;
        }
    public:
        DynamicArray() : data_(nullptr), size_(0), capacity_(0) {}
        ~DynamicArray() {
            clear();
            deallocate(data_, capacity_);
        }
        void push_back(const T& value){
            grow_if_needBe();
            new (data_+size_) T(value);
            ++size_;
        }

        void pop_back(){
            if(size_ == 0) throw std::out_of_range("empty");
            --size_;
            data_[size_].~T();
        }

        void insert(size_t idx, const T& value){
            if(idx > size_) throw std::out_of_range("index");
            grow_if_needBe();
            for(size_t i = size_; i>idx; --i){
                new (data_ + i) T(std::move(data_[i-1]));
                data_[i-1].~T();
            }
            new(data_ + idx) T(value);
            ++size_;
        }

        void erase(size_t idx){
            if (idx >= size_) throw std::out_of_range("index");
            data_[idx].~T();
            for (size_t i = idx; i < size_ - 1; ++i) {
                new (data_ + i) T(std::move(data_[i + 1]));
                data_[i + 1].~T();
            }
            --size_;
        }

        void clear(){
            for (size_t i = 0; i < size_; ++i) data_[i].~T();
            size_ = 0;
        }

        T& operator[](size_t i) {return data_[i]; }
        const T& operator[](size_t i) const {return data_[i]; }

        size_t size() const { return size_; }
        size_t capacity() const { return capacity_; }
};



int main() {
    DynamicArray<int> arr;
    arr.push_back(10);
    arr.push_back(20);
    arr.insert(1, 15);

    for (size_t i = 0; i < arr.size(); ++i)
        std::cout << arr[i] << " ";
    std::cout << "\n";
}
