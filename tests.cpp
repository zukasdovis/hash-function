#include <bits/stdc++.h>
#include "hash.h"
#include "tests.h"
#include "functions.h"

using namespace std;

void testPerformance() {
    cout << "\nTestuojama sparta\n";
    
    vector<int> sizes = {1, 2, 4, 8, 16, 32, 64, 128, 256};
    
    cout << left << setw(12) << "Baitai" << setw(15) << "Vidurkis (ns)" 
         << setw(15) << "Min (ns)" << setw(15) << "Max (ns)" 
         << setw(15) << "Sklaida\n";
    cout << string(72, '-') << "\n";
    
    for (int size : sizes) {
        string input(size, 'a');
        vector<long long> times;
        
        // Apšilimas
        for (int i = 0; i < 100; i++) hashFunction(input);
        
        // Matavimai
        for (int i = 0; i < 5; i++) {
            auto start = high_resolution_clock::now();
            hashFunction(input);
            auto end = high_resolution_clock::now();
            times.push_back(duration_cast<nanoseconds>(end - start).count());
        }
        
        double avg = accumulate(times.begin(), times.end(), 0.0) / times.size();
        double minTime = *min_element(times.begin(), times.end());
        double maxTime = *max_element(times.begin(), times.end());
        double variance = 0;
        for (auto t : times) variance += pow(t - avg, 2);
        variance /= times.size();
        
        cout << left << setw(12) << size << setw(15) << fixed << setprecision(2) << avg
             << setw(15) << minTime << setw(15) << maxTime << setw(15) << sqrt(variance) << "\n";
    }
}

void testCollisions() {
    cout << "\nTestuojamos kolizijos\n";
    
    vector<int> lengths = {10, 100, 500, 1000};
    mt19937 gen(chrono::system_clock::now().time_since_epoch().count());
    
    for (int len : lengths) {
        set<uint32_t> hashes;
        set<string> inputs;
        int collisions = 0;
        
        for (int i = 0; i < 100000; i++) {
            string input;
            for (int j = 0; j < len; j++) {
                input += char(gen() % 256);
            }
            
            if (inputs.count(input)) {
                i--;
                continue;
            }

            uint32_t hash = hashFunction(input);
            if (hashes.count(hash)) {
                collisions++;
            }
            hashes.insert(hash);
            inputs.insert(input);
        }
        
        cout << "Ilgis: " << len << " baitu | Ismaisos: " << hashes.size() 
             << " | Kolizijos: " << collisions 
             << " | Kolizijos %: " << fixed << setprecision(4) 
             << (100.0 * collisions / 100000) << "\n";
    }
}

void testAvalanche() {
    cout << "\nTestuojamas lavinos efektas\n";
    
    vector<int> lengths = {10, 50, 100, 500};
    mt19937 gen(chrono::system_clock::now().time_since_epoch().count());
    uniform_int_distribution<> dis(0, 255);
    
    map<int, vector<double>> bitDifferences;
    
    cout << "Analizuojamos 100000 poru: originalus vs. su vienu keistiniu baitu\n\n";
    
    for (int len : lengths) {
        vector<double> diffs;
        
        for (int i = 0; i < 100000 / lengths.size(); i++) {
            string input1;
            for (int j = 0; j < len; j++) {
                input1 += char(dis(gen));
            }
            
            string input2 = input1;
            int changePos = gen() % len;
            input2[changePos] = char(dis(gen));
            
            uint32_t hash1 = hashFunction(input1);
            uint32_t hash2 = hashFunction(input2);
            
            double bitDiff = compare(hash1, hash2);
            diffs.push_back(bitDiff);
        }
        
        bitDifferences[len] = diffs;
        
        double minDiff = *min_element(diffs.begin(), diffs.end());
        double maxDiff = *max_element(diffs.begin(), diffs.end());
        double avgDiff = accumulate(diffs.begin(), diffs.end(), 0.0) / diffs.size();
        
        cout << "Ilgis " << len << "b: Min=" << fixed << setprecision(4) << minDiff 
             << " | Max=" << maxDiff << " | Vidurkis=" << avgDiff << "\n";
    }
    
    // Visos poros
    vector<double> allDiffs;
    for (auto& [len, diffs] : bitDifferences) {
        allDiffs.insert(allDiffs.end(), diffs.begin(), diffs.end());
    }
    
    cout << "\nVISOS POROS: Min=" << *min_element(allDiffs.begin(), allDiffs.end())
         << " | Max=" << *max_element(allDiffs.begin(), allDiffs.end())
         << " | Vidurkis=" << accumulate(allDiffs.begin(), allDiffs.end(), 0.0) / allDiffs.size() << "\n";
    
    // Bitų skirtumo histograma
    cout << "\nBitu skirtumo histograma (visos poros):\n";
    map<int, int> histogram;
    for (double diff : allDiffs) {
        int bits = (int)(diff * 32 + 0.5);
        histogram[bits]++;
    }
    
    for (auto& [bits, count] : histogram) {
        int bars = count / 10000;
        cout << setw(2) << bits << "b: " << string(bars, '#') << " (" << count << ")\n";
    }
}

void testBruteForce() {
    cout << "\nTestuojama tiksline maisa\n";

    // Be druskos
    string target = "4729";
    uint32_t targetHash = hashFunction(target);

    cout << "\n--- Be druskos ---\n";
    cout << "Tikslas: " << target << "\n";

    auto start = high_resolution_clock::now();

    int attempts = 0;
    vector<string> found;
    bool firstFound = false;

    for (int i = 0; i < 10000; i++) {
        string candidate = to_string(i);

        while (candidate.length() < 4)
            candidate = "0" + candidate;

        attempts++;

        if (hashFunction(candidate) == targetHash) {

            found.push_back(candidate);

            if (!firstFound) {
                auto foundTime = high_resolution_clock::now();

                auto timeToFind =
                    duration_cast<microseconds>(foundTime - start).count();

                cout << "Pirmas sutapimas: " << candidate << "\n";
                cout << "Bandymu iki pirmo sutapimo: "
                     << attempts << "\n";
                cout << "Laikas iki pirmo sutapimo: "
                     << timeToFind << " us\n";

                firstFound = true;
            }
        }
    }

    auto end = high_resolution_clock::now();

    cout << "Visu 10000 bandymu laikas: "
         << duration_cast<microseconds>(end - start).count()
         << " us\n";

    cout << "Visi sutape kandidatai: ";

    for (const auto& candidate : found)
        cout << candidate << " ";

    cout << "\n";


    // Su vieša druska
    cout << "\n--- Su viesa druska ---\n";

    string salt = "ABC";
    target = "4729";
    targetHash = hashFunction(target + salt);

    cout << "Tikslas: " << target << "\n";
    cout << "Druska: " << salt << "\n";

    start = high_resolution_clock::now();

    attempts = 0;
    found.clear();
    firstFound = false;

    for (int i = 0; i < 10000; i++) {
        string candidate = to_string(i);

        while (candidate.length() < 4)
            candidate = "0" + candidate;

        attempts++;

        if (hashFunction(candidate + salt) == targetHash) {

            found.push_back(candidate);

            if (!firstFound) {
                auto foundTime = high_resolution_clock::now();

                auto timeToFind =
                    duration_cast<microseconds>(foundTime - start).count();

                cout << "Pirmas sutapimas: " << candidate << "\n";
                cout << "Bandymu iki pirmo sutapimo: "
                     << attempts << "\n";
                cout << "Laikas iki pirmo sutapimo: "
                     << timeToFind << " us\n";

                firstFound = true;
            }
        }
    }

    end = high_resolution_clock::now();

    cout << "Visu 10000 bandymu laikas: "
         << duration_cast<microseconds>(end - start).count()
         << " us\n";

    cout << "Visi sutape kandidatai: ";

    for (const auto& candidate : found)
        cout << candidate << " ";

    cout << "\n";
}
