# Hash Funkcija v0.2.1

Mano sukurta 32 bitų hash funkcija, skirta eksperimentuoti su hash funkcijų savybėmis: greičiu, kolizijomis, lavinos efektu ir brute-force paieška.

Projektas sukurtas C++ kalba. Hash funkcija apdoroja įvestį baitų lygiu, todėl gali būti naudojama su įvairiais UTF-8 tekstais.

> **v0.2.1** – nauja hash funkcijos versija, kurioje pakeistas pats hash algoritmas ir atlikti jo savybių eksperimentai.

---

# Algoritmas

Hash funkcija grąžina **32 bitų** reikšmę, kuri gali būti pateikiama kaip 8 simbolių šešioliktainis skaičius.

Pradinė `state` reikšmė:

```cpp
uint32_t state = 0x9E3779B9;
```

Ši konstanta naudojama kaip pradinis seed.

## 1. Įvesties ilgio įtraukimas

Pirmiausia į hash būseną įtraukiamas įvesties ilgis:

```cpp
state ^= (uint32_t)input.size() * 0x85EBCA6B;
```

Tokiu būdu hash reikšmė priklauso ne tik nuo įvesties baitų, bet ir nuo jų skaičiaus.

---

## 2. Kiekvieno baito apdorojimas

Kiekvienas įvesties baitas konvertuojamas į `uint8_t`:

```cpp
uint32_t current = static_cast<uint8_t>(input[i]);
```

Toliau atliekamos trys pagrindinės operacijos.

### XOR su konstanta

Baito reikšmė padauginama iš konstantos ir XOR operacija sumaišoma su dabartine būsena:

```cpp
state ^= current * 0x5D6FEBB8;
```

### Bitų rotacija

Toliau atliekama kairinė 32 bitų rotacija:

```cpp
state = rotateLeft(state, (7 + i) % 32);
```

Rotacijos dydis priklauso nuo apdorojamo baito pozicijos.

Pati rotacijos funkcija:

```cpp
uint32_t rotateLeft(uint32_t x, int bits) {
    return (x << bits) | (x >> (32 - bits));
}
```

### XOR-shift

Po rotacijos atliekamas papildomas bitų maišymas:

```cpp
state ^= state >> 11;
```

Taip kiekvieno baito apdorojimo metu informacija paskleidžiama tarp skirtingų `state` bitų.

---

# Finalizavimas

Apdorojus visus įvesties baitus atliekamas papildomas finalizavimo etapas.

```cpp
state ^= state >> 15;
state *= 0x57EFBCA6;

state ^= state >> 13;
state *= 0xCBED548A;

state ^= state >> 11;
state *= 0x9E3779B9;

state ^= state >> 16;
```

Finalizavimo metu naudojamos XOR-shift ir daugybos operacijos.

Pagrindinis tikslas – galutinai išmaišyti `state` bitus ir sumažinti tiesioginę priklausomybę tarp įvesties ir išvesties.

---

# Pilna hash funkcija

```cpp
#include "hash.h"

uint32_t rotateLeft(uint32_t x, int bits) {
    return (x << bits) | (x >> (32 - bits));
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
```

---

# Hash funkcijos savybės

* **Išvesties dydis:** 32 bitai
* **Išvesties formatas:** 8 šešioliktainiai simboliai
* **Seed:** `0x9E3779B9`
* **Deterministinė:** ta pati įvestis visada duoda tą pačią hash reikšmę
* **UTF-8:** įvestis apdorojama baitais
* **Įvesties ilgis:** įtraukiamas į pradinę hash būseną
* **Operacijos:** XOR, bitų poslinkiai, bitų rotacija ir daugyba
* **Finalizavimas:** 4 XOR-shift ir 3 daugybos operacijos

---

# Eksperimentų rezultatai

## 1. Sparta

Hash funkcija buvo testuojama su skirtingo dydžio įvestimis.

| Baitai | Vidurkis (ns) | Min (ns) | Max (ns) | Sklaida |
| -----: | ------------: | -------: | -------: | ------: |
|      1 |           100 |      100 |      100 |    0.00 |
|      2 |            80 |        0 |      100 |   40.00 |
|      4 |           140 |      100 |      200 |   48.99 |
|      8 |           220 |      200 |      300 |   40.00 |
|     16 |           360 |      300 |      400 |   48.99 |
|     32 |           600 |      600 |      600 |    0.00 |
|     64 |          1340 |     1300 |     1400 |   48.99 |
|    128 |          2420 |     2400 |     2500 |   40.00 |
|    256 |          4520 |     4000 |     4700 |  271.29 |

Rezultatai rodo, kad didėjant įvesties dydžiui skaičiavimo laikas taip pat didėja.

Kadangi algoritmas kiekvieną įvesties baitą apdoroja vieną kartą, jo laiko sudėtingumas pagal įvesties dydį yra **O(n)**.

---

# 2. Kolizijos

Buvo atlikta po **100 000 testų** su skirtingo ilgio įvestimis.

|  Ilgis | Išmaišos | Kolizijos | Kolizijų % |
| -----: | -------: | --------: | ---------: |
|   10 B |   99 994 |         6 |    0.0060% |
|  100 B |   99 992 |         8 |    0.0080% |
|  500 B |   99 996 |         4 |    0.0040% |
| 1000 B |   99 995 |         5 |    0.0050% |

Gauti rezultatai skirtinguose testuose svyravo nuo **4 iki 8 kolizijų**.

Kadangi funkcijos išvestis yra tik 32 bitų, kolizijos teoriškai yra neišvengiamos, kai skirtingų įvesčių skaičius tampa pakankamai didelis.

---

# 3. Lavinos efektas

Lavinos efektui testuoti buvo analizuojamos **100 000 porų**, kuriose originali įvestis buvo pakeista pakeičiant vieną baitą.

Buvo matuojama, kokia dalis iš 32 hash išvesties bitų pasikeitė.

## Rezultatai

| Ilgis |    Min |    Max | Vidurkis |
| ----: | -----: | -----: | -------: |
|  10 B | 0.0000 | 0.8438 |   0.4985 |
|  50 B | 0.0000 | 0.8438 |   0.4966 |
| 100 B | 0.0000 | 0.8438 |   0.4978 |
| 500 B | 0.0000 | 0.8438 |   0.4984 |

Bendras rezultatas:

```text
Min = 0.0000
Max = 0.8438
Vidurkis = 0.4978
```

Tai reiškia, kad vidutiniškai pasikeitė:

**49.78% hash išvesties bitų.**

Idealiu atveju pakeitus vieną įvesties bitą ar nedidelę įvesties dalį būtų tikimasi maždaug 50% išvesties bitų pasikeitimo.

Gautas **49.78%** rezultatas yra labai arti šios reikšmės.

### Bitų skirtumo pasiskirstymas

```text
0 b:    (380)
4 b:    (2)
5 b:    (3)
6 b:    (23)
7 b:    (63)
8 b:    (233)
9 b:    (661)
10 b:   (1562)
11 b:   (2994)
12 b:   (5267)
13 b:   (7963)
14 b:   (10944)
15 b:   (13348)
16 b:   (13859)
17 b:   (13176)
18 b:   (10805)
19 b:   (8121)
20 b:   (5147)
21 b:   (2907)
22 b:   (1540)
23 b:   (663)
24 b:   (237)
25 b:   (83)
26 b:   (14)
27 b:   (5)
```

Didžiausia rezultatų koncentracija yra ties **16 bitų**, kas atitinka maždaug pusės 32 bitų hash išvesties pasikeitimą.

---

# 4. Tikslinės reikšmės paieška

Buvo atliktas brute-force eksperimentas, kurio metu buvo ieškoma įvesties, kurios hash reikšmė sutaptų su nustatyta tiksline reikšme.

Tikslas:

```text
4729
```

Buvo tikrinami kandidatai nuo `0000` iki `9999`.

## Be druskos

```text
Tikslas:                    4729
Pirmas sutapimas:           4729
Bandymų iki sutapimo:       4730
Laikas iki pirmo sutapimo:  1330 us
Visų 10000 bandymų laikas:  2456 us
Visi sutapę kandidatai:     4729
```

## Su vieša druska

Naudota druska:

```text
ABC
```

Rezultatai:

```text
Tikslas:                    4729
Druska:                     ABC
Pirmas sutapimas:           4729
Bandymų iki sutapimo:       4730
Laikas iki pirmo sutapimo:  2121 us
Visų 10000 bandymų laikas:  4702 us
Visi sutapę kandidatai:     4729
```

Šis eksperimentas parodo, kaip papildoma druska pakeičia hash skaičiavimo procesą ir padidina šio konkretaus testo vykdymo laiką.

Šis eksperimentas **neįrodo kriptografinio saugumo**. Hash funkcija nėra skirta slaptažodžių saugojimui ar kitoms kriptografinėms reikmėms.

---

# v0.1 ir v0.2.1 palyginimas

| Parametras      |    v0.1 |                        v0.2.1 |     Pokytis |
| --------------- | ------: | ----------------------------: | ----------: |
| Nuliai          |     402 |                           380 |       -5.5% |
| Lavinos efektas |  48.26% |                        49.78% | +1.52 p. p. |
| Kolizijos       |  0.008% |                     0.00575%* |       ~-28% |
| Greitis         | ~110 ns | priklauso nuo įvesties dydžio |           — |

* Vidurkis apskaičiuotas iš naujausių keturių kolizijų testų:

```text
(0.0060 + 0.0080 + 0.0040 + 0.0050) / 4
= 0.00575%
```

v0.2.1 algoritmas yra paprastesnis už ankstesnę versiją: pašalinta `bitSplit()` operacija ir papildomas pozicijos XOR. Vietoje jų naudojamas tiesioginis baito maišymas, bitų rotacija ir XOR-shift.

---

# Ryšys su paskaitos medžiaga

## Avalanche Effect

Lavinos efektas apibūdina situaciją, kai mažas įvesties pakeitimas sukelia didelį hash išvesties pasikeitimą.

v0.2.1 teste gautas vidurkis:

```text
49.78%
```

Tai yra arti teorinės 50% reikšmės.

## XOR

XOR operacija naudojama pagrindiniame maišymo procese:

```cpp
state ^= current * 0x5D6FEBB8;
```

ir XOR-shift operacijose:

```cpp
state ^= state >> 11;
```

## Bitų rotacija

Rotacija naudojama informacijai paskleisti tarp skirtingų `state` bitų:

```cpp
state = rotateLeft(state, (7 + i) % 32);
```

## Determinizmas

Hash funkcija yra deterministinė:

```text
ta pati įvestis → ta pati hash reikšmė
```

## Fiksuotas išvesties dydis

Nepriklausomai nuo įvesties ilgio hash funkcijos rezultatas yra 32 bitų dydžio.

---

# Naudojimas

## Kompiliavimas

Projektą galima sukompiliuoti naudojant C++ kompiliatorių:

```bash
g++ main.cpp functions.cpp tests.cpp hash.cpp -o hash.exe
```

## Paleidimas

Windows PowerShell:

```powershell
.\hash.exe
```

Git Bash / Linux:

```bash
./hash.exe
```

---

# Projekto struktūra

```text
.
├── main.cpp
├── hash.cpp
├── hash.h
├── functions.cpp
├── functions.h
├── tests.cpp
├── tests.h
└── README.md
```

---

# Versijos

## v0.1

Pradinė mano sukurta hash funkcijos versija.

Buvo sukurta pagrindinė 32 bitų hash funkcijos struktūra ir atlikti pirmieji greičio, kolizijų bei lavinos efekto testai.

## v0.2

Algoritmas buvo tobulinamas siekiant pagerinti lavinos efektą ir hash reikšmių pasiskirstymą.

## v0.2.1

Sukurta nauja hash funkcijos versija.

Pagrindiniai v0.2.1 pakeitimai:

* naujas hash algoritmo variantas;
* įvesties ilgio įtraukimas į `state`;
* baito daugyba iš `0x5D6FEBB8`;
* pozicijai priklausanti bitų rotacija;
* XOR-shift po kiekvieno baito;
* naujas finalizavimo etapas;
* atlikti nauji spartos, kolizijų, lavinos efekto ir brute-force testai.

---

# Dirbtinio intelekto pagalba

Kuriant projektą buvo naudojamas dirbtinis intelektas kaip pagalbinė priemonė.

DI buvo naudojamas:

* geriau suprasti hash funkcijų veikimo principus;
* suprasti bitines operacijas;
* padėti realizuoti mano idėjas;
* padėti kurti ir tobulinti hash funkcijos algoritmą;
* analizuoti testų rezultatus;
* padėti paruošti `README.md`.

DI buvo naudojamas kaip pagalbinė priemonė mokymosi ir kūrimo procese.

---

# Išvados

v0.2.1 versijoje sukurta nauja 32 bitų hash funkcija, naudojanti XOR, daugybą, bitų rotaciją ir XOR-shift operacijas.

Atlikti eksperimentai parodė:

* algoritmo vykdymo laikas didėja didėjant įvesties dydžiui;
* 100 000 testų rinkiniuose buvo gautas nedidelis kolizijų skaičius;
* lavinos efekto vidurkis siekė **49.78%**;
* bitų skirtumo pasiskirstymo pikas buvo ties **16 pasikeitusių bitų**;
* funkcija yra deterministinė;
* nepriklausomai nuo įvesties dydžio rezultatas yra 32 bitų;
* UTF-8 įvestis apdorojama baitų lygiu.

Projektas skirtas **mokymuisi ir eksperimentams su hash funkcijų kūrimo principais**, o ne kriptografiniam naudojimui.
