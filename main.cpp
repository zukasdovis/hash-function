#include <bits/stdc++.h>

using namespace std;

int hexToInt(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';

    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;

    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;

    return -1;
}
int countSetBits(int x)
{
    int count = 0;

    while (x > 0)
    {
        count += x & 1;
        x >>= 1;
    }

    return count;
}

double compare(string a, string b)
{
    if (a.length() != b.length())
    {
        cerr << "Hashes must have the same length!" << endl;
        return -1;
    }

    int difBits = 0;

    for (int i = 0; i < a.length(); i++)
    {
        int x = hexToInt(a[i]);
        int y = hexToInt(b[i]);

        if (x == -1 || y == -1)
        {
            cerr << "Invalid hexadecimal character!" << endl;
            return -1;
        }

        int diff = x ^ y;

        difBits += countSetBits(diff);
    }

    int totalBits = a.length() * 4;

    return double(difBits) / totalBits;
}

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


int main(){

    ifstream fd("input.txt");
    ofstream fr("result.txt");

    string input;

    int x;
    cin>>x;
    vector<string>inputs;
    if(x==0){
        string input;
        while(getline(fd,input)){
            inputs.push_back(input);
        }
    }else if(x==1){
        cin>>input;
        inputs.push_back(input);
    }

    for(string s:inputs){

        uint32_t result = hashFunction(s);
        cout<<"Input: "<<s<<endl;
        cout<<"Hash: "<<hex<<result<<dec<<endl;

    }
    cin>>x;

    return 0;
}
