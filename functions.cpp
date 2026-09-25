#include "functions.h"

// from Uint32 to Binary
string uint32ToBinary(uint32_t value)
{
    string binary;

    for (int i = 31; i >= 0; i--)
    {
        binary += ((value >> i) & 1) + '0';
    }

    return binary;
}

// Avalanche tikrinimas
int countSetBits(uint32_t x)
{
    int count = 0;

    while (x > 0)
    {
        count += x & 1;
        x >>= 1;
    }

    return count;
}

double compare(uint32_t a, uint32_t b)
{
    uint32_t diff = a ^ b;

    int differentBits = countSetBits(diff);

    int totalBits = 32;

    return double(differentBits)/totalBits;
}
// Failas
bool readFile(const string& filename, string& content) {
    ifstream file(filename, ios::binary);

    if (!file) {
        cout << "Klaida: nepavyko atidaryti failo.\n";
        return false;
    }

    content.assign(
        (istreambuf_iterator<char>(file)),
        istreambuf_iterator<char>()
    );

    return true;
}