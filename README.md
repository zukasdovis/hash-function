# Hash Funkcijos v0.2 – Finali Versija

## Algoritmas

### Seed: Golden Ratio (0x9E3779B9)
- Universalūs seed skaičius
- Naudojamas MurmurHash, SipHash

### Pagrindinės Operacijos

1. **Ilgio garantija**: `state ^= size * 0x85EBCA6B`
2. **Kiekvienas baitas**:
   - Pozicijos XOR: `state ^= (current << (i%4)) | (current >> (8-i%4))`
   - bitSplit: `state ^= bitSplit(current, next)`
   - Suma: `state += key * 0x5D6FEBB8`
   - Rotacija: `state = rotateLeft(state, (7+i)%32)`
   - **XOR-shift**: `state ^= state >> 11`
3. **Finalizavimas** (6 operacijų):
state ^= state >> 15
state *= 0x57EFBCA6
state ^= state >> 13
state *= 0xCBED548A
state ^= state >> 11
state *= 0x9E3779B9
state ^= state >> 16


## Eksperimentų Rezultatai

### 1. Sparta
| Baitai | Vidurkis (ns) | Min | Max | Sklaida |
|--------|---------------|----|-----|---------|
| 1 | 120 | 100 | 200 | 40.00 |
| 8 | 420 | 400 | 500 | 40.00 |
| 32 | 1320 | 1300 | 1400 | 40.00 |
| 256 | 10240 | 10200 | 10300 | 48.99 |

**Išvada:** Lineinis augimas ~120 ns/B

### 2. Kolizijos (100,000 testas)
| Ilgis | Kolizijos | % |
|-------|-----------|-----|
| 10 B | 2 | 0.0020% |
| 100 B | 8 | 0.0080% |
| 500 B | 5 | 0.0050% |
| 1000 B | 3 | 0.0030% |

**Vidurkis: 0.0045%** ✅

### 3. Lavanos Efektas
- **Nuliai: 364 (0.364%)**
- **Vidurkis: 15.96 bitų (49.86%)**
- **Max: 27 bitai**
0 b: (364) ← Nuliai (0.364%)
...
16 b: (14029) ← Piko reikšmė
...
27 b: (1)

### 4. Spėjimas (Brute Force)
- **Be druskos**: 4730 bandymų, 1423 μs
- **Su druska (ABC)**: 4730 bandymų, 3235 μs
- **Kandidatai**: 4729 ✓

### 5. v0.1 vs v0.2 Palyginimas

| Parametras | v0.1 | v0.2 | Gerinimas |
|-----------|------|------|-----------|
| Nuliai | 402 | 364 | **-9.5%** |
| Lavana | 48.26% | 49.86% | **+1.6%** |
| Kolizijos | 0.008% | 0.0045% | **-44%** |
| Greitis | 110 ns | 120 ns | **-9%** |

## Išvados

### Pagerėjimai v0.2
1. **Avalanche Effect**: Greit artėjimas prie 50% idealo
2. **Nuliai**: Sumažinti 38 atvejų (-9.5%)
3. **Kolizijos**: Dar rečiau
4. **Determinizmas**: ✓ Fiksuoti seed

### Savybės
- **Ilgis**: Fiksuotas 32 bitai (8 hex skaičiai)
- **Vienodumas**: Panaši bitų distribucija visuose ilgiuose
- **Greitis**: ~120 ns/B – geras praktiniu atžvilgiu
- **UTF-8**: Baitai traktuojami jų numerinėmis reikšmėmis

### Ryšys su Paskaita

1. **Avalanche Effect**: 50% yra standartinė riba (SHA-1, MD5)
2. **XOR operacijos**: Fundamentalios bitų sklidimui
3. **Determinizmas**: Reikalingas hash reproducijai
4. **Seed**: Apsaugoja nuo predictability

## Naudojimas

```cpp
#include "hash.h"

uint32_t hash = hashFunction("hello");
printf("%08x\n", hash);  // Išveda 8 hex skaitmenų
```

## Versijos

- **v0.1**: Pradinė versija (110 ns/B, 48.26% lavana)
- **v0.2**: Optimizuota (120 ns/B, 49.86% lavana)

## Dirbtinio Intelekto Pagalba
- Padėjo suprasti, kaip veikia hash funkcijos
- Padėjo realizuti mano v0.1 idėją
- Padarė v0.2
- Padėjo paruošti README.md