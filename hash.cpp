#include "hash.h"

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

uint32_t hashFunction(const std::string& input) {
    uint32_t state = 0x9E3779B9;
    
    state ^= (uint32_t)input.size() * 0x85EBCA6B;
    
    for(size_t i = 0; i < input.size(); i++) {
        uint8_t current = static_cast<uint8_t>(input[i]);
        uint8_t next = (i + 1 < input.size()) ? static_cast<uint8_t>(input[i+1]) : 0xFF;
        uint8_t key = bitSplit(current, next);
        
        state ^= ((current << (i % 4)) | (current >> (8 - i % 4)));
        state ^= key;
        
        state += key * 0x5D6FEBB8;
        state = rotateLeft(state, (7 + i) % 32);

        state ^= state >> 11;
    }
    
    state ^= state >> 15;
    state *= 0x57EFBCA6;
    state ^= state >> 13;
    state *= 0xCBED548A;
    state ^= state >> 11;
    state *= 0x9E3779B9;
    state ^= state >> 16;
    
    return state;
}