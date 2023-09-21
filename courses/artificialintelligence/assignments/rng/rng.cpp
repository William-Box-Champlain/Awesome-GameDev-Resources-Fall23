#include <iostream>
#include <cstdint>
// add your imports here
int xorShift(int input,int seed,int min,int max);

int main(){
    uint32_t seed, outputs, min, max;
    std::cin >> seed >> outputs >> min >> max;
    xorShift(seed,outputs,min,max);
}

int xorShift(int seed,int outputs,int min,int max)
{
    int leftOne = 13;
    int rightOne = 17;
    int leftTwo = 5;
    int random = seed;
    for(int i = 0; i < outputs; i++)
    {
        //generate random
        random ^= random << leftOne;
        random ^= random >> rightOne;
        random ^= random << leftTwo;
        abs(random);
        //clamp random
        int clampedValue = min + (random % (max - min + 1));
        std::cout << clampedValue << std::endl;
    }
    return random;
}