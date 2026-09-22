#include <bits/stdc++.h>

using namespace std;

//Avalanche tikrinimas

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
//Algortimas
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

//main
int main(){

    ifstream fd("input.txt");
    ofstream fr("result.txt");

    string input;

    int x;

    cout<<"Ivedimo metodas: 1-is failo, 2-ranka: \n";
    try{
        cin>>x;

        if (cin.fail()) {
            throw invalid_argument("Neteisinga ivestis");
        }
    }
    catch (const invalid_argument& e) {
        cout << e.what() << endl;
    }

    vector<string>inputs;
    
    if(x==1){
        string input;
        while(getline(fd,input)){
            inputs.push_back(input);
        }
    }else if(x==2){
        string input;

        cout<<"Ivesk kiek slaptazodziu vesi: ";
        int y;
        cin >> y;
        cin>>ws;
        while(y--){
            getline(cin,input);
            inputs.push_back(input);
        } 
    }
    vector<uint32_t>results;
    for(string s:inputs){

        uint32_t result = hashFunction(s);
        results.push_back(result);
        cout<<"Input: "<<s<<endl;
        cout<<"Hash: "<<hex<<result<<dec<<endl;

    }

    cout<<"Ar norite patikrinti avalanche: 1-taip, 0-ne: ";
    int y;
    cin>>y;
    if(y){
        //cout<<compare(results[0], results[1]);
    }

    return 0;
}
