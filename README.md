# Hash Function

Nuosava maišos (hash) funkcija, sukurta C++ kalba. Projektas skirtas išbandyti hash funkcijos spartą, kolizijas, lavinos efektą ir atsparumą brute-force paieškai.

# Hash Function Idea
Mano hash funkcijos idėja yra kiekvieną įvesties baitą apdoroti kartu su kitu baitu. Abu baitai funkcijoje bitSplit() padalijami į aukštąją ir žemąją 4 bitų dalis, kurios atskirai sudedamos ir vėl sujungiamos į vieną 8 bitų key. Šis key įmaišomas į pradinę 32 bitų būseną naudojant XOR, sudėtį su pasirinkta konstanta 0x5D6FEBB8 ir 7 bitų rotaciją į kairę. Taip kiekvienas įvesties baitas keičia bendrą būseną, o ankstesnių baitų įtaka perduodama toliau per visą algoritmą. Apdorojus visą įvestį, papildomai atliekami XOR poslinkiai ir daugyba iš konstantų, kad galutinė 32 bitų reikšmė būtų dar labiau sumaišyta.

## Funkcijos savybės

* Veikia su tekstinėmis eilutėmis.
* Palaiko skirtingo ilgio įvestis.
* Grąžina fiksuoto dydžio hash reikšmę.
* Naudojamos bitinės operacijos ir maišymo operacijos.
* Atlikti spartos, kolizijų, lavinos efekto ir brute-force testai.

---

## Spartos testas

Hash funkcijos sparta buvo testuojama su skirtingo dydžio įvestimis.

| Baitai | Vidurkis (ns) | Min (ns) | Max (ns) | Sklaida |
| -----: | ------------: | -------: | -------: | ------: |
|      1 |           120 |      100 |      200 |   40.00 |
|      2 |           100 |      100 |      100 |    0.00 |
|      4 |           220 |      200 |      300 |   40.00 |
|      8 |           360 |      300 |      400 |   48.99 |
|     16 |           660 |      600 |      700 |   48.99 |
|     32 |          1400 |     1400 |     1400 |    0.00 |
|     64 |          2500 |     2400 |     2600 |   63.25 |
|    128 |          5320 |     5200 |     5400 |   74.83 |
|    256 |          9140 |     8500 |     9400 |  338.23 |

Didėjant įvesties dydžiui, hash apskaičiavimo laikas taip pat didėja.

---

## Kolizijų testas

Buvo sugeneruota apie 100 000 skirtingų įvesčių ir tikrinama, kiek jų gavo tokią pačią hash reikšmę.

| Įvesties ilgis | Išmaišos | Kolizijos | Kolizijų % |
| -------------: | -------: | --------: | ---------: |
|       10 baitų |   99 996 |         4 |    0.0040% |
|      100 baitų |   99 997 |         3 |    0.0030% |
|      500 baitų |   99 992 |         8 |    0.0080% |
|     1000 baitų |   99 992 |         8 |    0.0080% |

Testuose gautas nedidelis kolizijų skaičius.

---

## Lavinos efektas

Lavinos efektas tikrina, kiek hash reikšmės bitų pasikeičia pakeitus tik vieną įvesties baitą.

Buvo analizuojama 100 000 originalių ir pakeistų porų.

|           Ilgis |        Min |        Max |   Vidurkis |
| --------------: | ---------: | ---------: | ---------: |
|            10 B |     0.0000 |     0.8125 |     0.4823 |
|            50 B |     0.0000 |     0.8125 |     0.4823 |
|           100 B |     0.0000 |     0.8438 |     0.4832 |
|           500 B |     0.0000 |     0.8125 |     0.4827 |
| **Visos poros** | **0.0000** | **0.8438** | **0.4826** |

Vidutiniškai pasikeitė apie **48.26% hash bitų**, kai buvo pakeistas tik vienas įvesties baitas.

### Bitų skirtumo pasiskirstymas

```text
0 b:   408
4 b:     3
5 b:     8
6 b:    36
7 b:   112
8 b:   376
9 b:   885
10b:  2060
11b:  3895
12b:  6550
13b:  9546
14b: 12243
15b: 14038
16b: 13798
17b: 12457
18b:  9597
19b:  6551
20b:  3958
21b:  1984
22b:   959
23b:   367
24b:   118
25b:    45
26b:     5
27b:     1
```

---

## Brute-force testas

Taip pat patikrinta, kaip greitai galima rasti 4 skaitmenų reikšmę brute-force būdu.

### Be druskos

```text
Tikslas: 4729
Pirmas sutapimas: 4729
Bandymų iki pirmo sutapimo: 4730
Laikas iki pirmo sutapimo: 1123 us
Visų 10000 bandymų laikas: 2466 us
Visi sutapę kandidatai: 4729
```

### Su vieša druska

```text
Tikslas: 4729
Druska: ABC
Pirmas sutapimas: 4729
Bandymų iki pirmo sutapimo: 4730
Laikas iki pirmo sutapimo: 2718 us
Visų 10000 bandymų laikas: 7763 us
Visi sutapę kandidatai: 4729
```

Šiame teste druskos naudojimas padidino skaičiavimo laiką, tačiau pati druska buvo vieša, todėl ji nėra slapta saugumo priemonė.

---

## Naudojimas

Projektą galima sukompiliuoti naudojant C++ kompiliatorių:

```bash
g++ main.cpp -o hash
```

Paleidimas:

```bash
./hash
```

Windows sistemoje:

```powershell
.\hash.exe
```

---

## Projekto tikslas

Šio projekto tikslas – praktiškai išbandyti savo sukurtą hash funkciją ir įvertinti:

1. **Spartą** – kiek laiko užtrunka hash apskaičiavimas.
2. **Kolizijas** – kaip dažnai skirtingos įvestys gauna tą pačią hash reikšmę.
3. **Lavinos efektą** – kaip stipriai pasikeičia hash pakeitus nedidelę įvesties dalį.
4. **Brute-force atsparumą** – kaip lengvai galima rasti konkrečią įvestį bandant daug kandidatų.

> Ši hash funkcija yra mokomasis projektas ir nėra skirta slaptažodžių saugojimui ar kitoms kriptografinio saugumo reikmėms.

