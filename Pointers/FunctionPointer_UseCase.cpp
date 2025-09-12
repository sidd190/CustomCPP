#include <iostream>
#define MAX_COLORS  256

//From a thread on function pointers, awesome resource - https://stackoverflow.com/questions/840501/how-do-function-pointers-in-c-work

typedef struct {
    const char *name;
    int red;
    int blue;
    int green;
} Color;

Color Colors[MAX_COLORS];

void EachColor(void (*fp)(Color *c)){
    for(int i =0; i<MAX_COLORS; i++){
        (*fp)(&Colors[i]);
    }
}

void printColor(Color *c){
    if(c->name){
        std::cout<<c->name<<"\t"<< c->red<<"\t" << c->green<<"\t"<<c->blue<<"\n";
    }
}

int main() {
    Colors[0].name="red";
    Colors[0].red=255;
    Colors[1].name="blue";
    Colors[1].blue=255;
    Colors[2].name="black";
    EachColor(printColor);
    return 0;
}