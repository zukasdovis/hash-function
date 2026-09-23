#include <bits/stdc++.h>

using namespace std;

//from Uint32 to Binary
string uint32ToBinary(uint32_t value)
{
    string binary;

    for (int i = 31; i >= 0; i--)
    {
        binary += ((value >> i) & 1) + '0';
    }

    return binary;
}

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
//Failas
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

//Main
int main(int argc, char* argv[]){
    
    int x;

    cout<<"Ivedimo metodas: 1-is failo, 2-ranka: \n";
    
    if (!(cin >> x)) {
        cout << "Neteisinga ivestis.\n";
        return 1;
    }

    vector<string>inputs;
    
    if(x == 1){
        if(argc < 2){
            cout << "Klaida: nenurodytas failas.\n";
            return 1;
        }
        cout<<"Naudojamas FAILO rezimas:\n";
        string input;

        if(!readFile(argv[1], input)){
            return 1;
        }

        inputs.push_back(input);

    }else if(x==2){
        string input;
        cout<<"Naudojamas RANKINIS rezimas:\n";
        cout<<"Ivesk kiek slaptazodziu vesi: ";
        int y;
        cin >> y;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        while(y--){
            getline(cin, input);
            inputs.push_back(input);
        } 
    }else{
        cout<<"Neteisingas pasirinkimas\n";
        return 1;
    }
    vector<uint32_t>results;
    for(const string& s:inputs){

        uint32_t result = hashFunction(s);
        results.push_back(result);
        cout<<"Input:\n"<<s<<endl;
        cout<<"Binary from hex: "<<uint32ToBinary(result)<<endl;
        cout << "Hash: "
        << hex << setw(8) << setfill('0') << result
        << dec << setfill(' ') << endl;

    }

    cout<<"Ar norite patikrinti avalanche: 1-taip, 0-ne: ";
    int y;
    cin>>y;
    if(y){

        cout<<"Pasirinkite dvi ivestis, kuriuos norit tikrint:\n";
        for(int i=1;i<=inputs.size();i++){
            cout<<i<<"."<<inputs[i-1]<<endl;
        }
        int first, second;
        cout<<"Iveskite du skaicius: ";
        cin>>first>>second;

        if(first < 1 || first > inputs.size() ||
            second < 1 || second > inputs.size()){
            cout << "Neteisingas pasirinkimas.\n";
            return 1;
        }
        cout<<compare(results[first-1], results[second-1]);
    }

    return 0;
}
