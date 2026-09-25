#include "hash.h"

// Algortimas
uint8_t bitSplit(uint8_t a, uint8_t b){
    uint8_t aHigh = a >> 4;
    uint8_t aLow = a & 0x0F;

    uint8_t bHigh = b >> 4;
    uint8_t bLow = b & 0x0F;

    uint8_t high = (aHigh + bHigh) & 0x0F;
    uint8_t low = (aLow + bLow) & 0x0F;

    return (high << 4) | low;
}
uint32_t rotateLeft(uint32_t x, int bits){
    return (x<<bits) | (x>>(32-bits));
}
uint32_t hashFunction(const string& input){
    
    uint32_t state =0x12345678; 

    for(size_t i=0; i < input.size(); i++){
        uint8_t current = static_cast<uint8_t>(input[i]);

        uint8_t next;

        if(i+1<input.size())
            next = static_cast<uint8_t>(input[i+1]);
        else next = 0;

        uint8_t key = bitSplit(current, next);

        state ^= key;

        state += key * 0x5D6FEBB8;

        state = rotateLeft(state,7);
    }

    state ^= state >> 16;  
    state *= 0x57EFBCA6;

    state ^= state >> 11;
    state *= 0xCBED548A;

    return state;
}