#include "../include/OrdenadorUniversal.hpp"
#include <iostream>  
#include <stdlib.h>  
#include <math.h>    
#include <string>
#include <cstring>
#include <chrono>


using namespace std;

#define seed 42

tipo vet[VETSZ];


int main() {

    double a = 0.0121560, 
           b = -0.0063780, 
           c = 0.0172897, 
           limCusto = 10.0;
    int i, j, p;
    long mult = (long)pow(10, KEYSZ - 1);
    srand48(1); 
    
    for (i = 0; i < VETSZ; i++) {
        for (j = (int)(drand48() * mult), p = KEYSZ - 2; p >= 0; j /= 10, p--)
            vet[i].key[p] = '0' + j % 10;
        vet[i].key[KEYSZ - 1] = 0;
        for (j = 0; j < PLSZ - 1; j++)
            vet[i].payload[j] = '0' + j % 10;
        vet[i].payload[PLSZ - 1] = 0;
    }
    // for (i = 0; i < VETSZ; i++) {
    //     cout << "key=" << vet[i].key 
    //               << "  payload=" << vet[i].payload << endl;
    // }

    OrdenadorUniversal *smartSorter = new OrdenadorUniversal(a, b, c, seed);

    cout << "size " << VETSZ << " seed " << seed << " breaks " << smartSorter->numeroQuebras(vet, VETSZ) << endl;

    int limPart = smartSorter->determinaLimiarParticao(vet, VETSZ, limCusto);
    int limQuebra = smartSorter->determinaLimiarQuebras(vet, VETSZ, limCusto, limPart);


    auto start = chrono::high_resolution_clock::now();
    
    smartSorter->ordenador(vet, VETSZ, limPart, limQuebra);

    // Para timer
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);

    cout << "Tempo: " << duration.count() << " μs\n";    
   

}