#include <bits/stdc++.h>
#include "hash.h"
#include "functions.h"
#include "tests.h"
using namespace std;

// Main
int main(int argc, char* argv[]){
    
    int x;

    cout<<"Ivedimo metodas: 1-is failo, 2-ranka, 3-testai: \n";
    
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
    }else if(x==3){
        testPerformance();
        testCollisions();
        testAvalanche();
        testBruteForce();
        return 0;
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
