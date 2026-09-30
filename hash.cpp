#include "hash.h"

uint32_t rotateLeft(uint32_t x, int bits){
    return (x<<bits) | (x>>(32-bits));
}

uint32_t hashFunction(const std::string& input) {
    uint32_t state = 0x9E3779B9;

    state ^= (uint32_t)input.size() * 0x85EBCA6B;

    for (size_t i = 0; i < input.size(); i++) {
        uint32_t current = static_cast<uint8_t>(input[i]);

        state ^= current * 0x5D6FEBB8;
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