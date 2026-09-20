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


int main(){

    ifstream fd("input.txt");
    ofstream fr("result.txt");

    string input;

    int x;
    cin>>x;
    if(x==0){
        getline(fd,input);
    }else if(x==1){
        getline(cin,input);
    }
    while(compare()){


    }


    return 0;
}
