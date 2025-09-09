#include <iostream>

//Good resource on memory Allocation Techniques in Modern C++ -> https://learnmoderncpp.com/2024/08/05/memory-allocation-techniques-in-modern-c/

//Basic function to swap two numbers with pointers
void swap(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Swapping two numbers with function pointer
void (*SwapPtr)(int *a, int *b);

int main() {

    int a =1;
    int b =4;
    //Basic number swapping
    swap (&a,&b);
    std::cout<<"Number 1 : "<<a<<"\nNumber 2 : "<<b<<"\n";

    //function pointer swapping
    SwapPtr = &swap;
    SwapPtr(&a,&b);
    std::cout << a<<"\n"<<b;
    return 0;
}