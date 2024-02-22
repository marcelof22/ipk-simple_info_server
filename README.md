# Dokumentácia k projektu č.1

# Autor: Marcel Feiler

**Stručný popis projektu:**

Projekt hinfosvc je implementovaný ako lightweight server v jazyku C, ktorý dokáže komunikovať pomocou protokolu HTTP. Tento server následne počúva požiadavky na uživateľom zadanom porte, následne bude podľa URL vracať informácie, o ktoré si sám uživateľ požiadal. Server je plne funkčný a spustiteľný v prostredí Linux Ubuntu 20.04 LTS.

Funkčnosť aplikácie spočíva v spracovaní nasledujúcich 3 typov dotazov na sever zaslané príkazom “GET” (alebo “curl”) :  

1. GET <http://servername:12345/hostname>

Vracia doménové meno, napr.: merlin.fit.vutbr.cz

2. GET <http://servername:12345/cpu-name>

Vracia základné informácie o procesore, napr.: Intel(R) Xeon(R) CPU e5-2640 0 @ 2.50GHz

3. GET <http://servername:12345/load>

Vracia informácie o aktuálnej záťaži v percentách, napr.: 25.55%

Pri implementácií som použil rôzne knižnice pracujúce so soketmi (sys/socket.h) a ďalšie ako stdio.h, stdlib.h, netinet/in.h, sys/socket.h, stdbool.h, string.h, unistd.h. 

**Spôsob spustenia projektu:**

1. Lokalizuj v termináli priečinok, kde sa nachádza daný kód spolu s príslušným Makefile.
2. Zadajte príkaz: make hinfosvc

Daný príkaz vytvorí preloží a vygeneruje spustiteľný program hinfosvc.

3. Pre spustenie servera zadajte príkaz: ./hinfosvc CISLOPORTU

kde CISLOPORTU označuje lokálny port (napr. 12345)

4. Pre prácu s vyššie uvedenými funkcionalitami si otvorte ďalší terminál a choďte do priečinka, kde sa nachádza daný program.
5. Následne v tomto druhom termináli zadajte jednu z troch vyššie uvedených príkazov, v prípade zlej požiadavky na príkaz dostanete výstup “Bad request”, v opačnom prípade dostanete správny výstup, teda buď hostname, cpu-name alebo load.
6. Pre ukončenie serveru stlačte v pôvodnom termináli (kde beží server) klávesy CTRL+C pre ukončenie servera.
7. Pre odstránenie spustiteľného súboru hinfosvc zadajte do terminálu príkaz: make clean 

**Príklady použitia projektu:**

![image](https://user-images.githubusercontent.com/99992982/154753320-3d7ceb3c-987e-4b32-b107-2b1093e097a1.png)

![image](https://user-images.githubusercontent.com/99992982/154753346-683b38bd-6541-4ae3-9a0b-b08950de4d42.png)

![image](https://user-images.githubusercontent.com/99992982/154753376-b77e5093-99ce-4a18-bead-0f5803801e69.png)
