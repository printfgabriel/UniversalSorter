#include <iostream>
#include <string>   
#include <random>
#include <chrono>

#include "../include/OrdenadorUniversal.hpp"
using namespace std;

#define seed 42

void gerarVetor(int v[], int tamanho, int config, int porcentagemDesordenado) {
    // Preenche sequencialmente
    for (int i = 0; i < tamanho; i++) {
        v[i] = i;
    }

    // Se for só ordenado, já está pronto
    if (config == 1) return;

    // Se for invertido, faz a inversão manual
    if (config == 2) {
        for (int i = 0; i < tamanho / 2; i++) {
            int temp = v[i];
            v[i] = v[tamanho - 1 - i];
            v[tamanho - 1 - i] = temp;
        }
        return;
    }

    // Se for parcialmente desordenado, faz trocas aleatórias
    if (config == 3) {
        mt19937 gerador(seed); // Gerador de números aleatórios
        int numTrocas = (porcentagemDesordenado * tamanho) / 100;

        for (int i = 0; i < numTrocas; i++) {
            int pos1 = gerador() % tamanho; // Posição aleatória 1
            int pos2 = gerador() % tamanho; // Posição aleatória 2

            // Troca os elementos
            int temp = v[pos1];
            v[pos1] = v[pos2];
            v[pos2] = temp;
        }
    }
}

// Imprime o vetor (limita a 10 elementos pra não poluir a tela)
void imprimirVetor(int v[], int tamanho, int limite = 10) {
    cout << "[";
    for (int i = 0; i < tamanho && i < limite; i++) {
        cout << v[i];
        if (i < tamanho - 1 && i < limite - 1) cout << ", ";
    }
    if (tamanho > limite) cout << ", ...";
    cout << "]" << endl;
}



int main(int argc, char *argv[]) {
    cout << "mainAE.cpp" << endl;
    double a = 0.0121560, 
           b = -0.0063780, 
           c = 0.0172897, 
           limCusto = 10.0;
    
    int tam = 1000;

    tipo *V = new int[tam];

    /*
        config 1 --> ordenado
               2 --> desordenado
               3 --> %
    */

    gerarVetor(V, tam, 2, 100);
    cout << "100% desordenado: ";
    imprimirVetor(V, tam);
    
    tipo *backup = new tipo[tam];
    for (int i = 0; i < tam; i++)
        backup[i] = V[i];
    
    
    OrdenadorUniversal *smartSorter = new OrdenadorUniversal(a, b, c, seed);
    cout << "size " << tam << " seed " << seed << " breaks " << smartSorter->numeroQuebras(V, tam) << endl;

    int limPart = smartSorter->determinaLimiarParticao(V, tam, limCusto);
    int limQuebra = smartSorter->determinaLimiarQuebras(V, tam, limCusto, limPart);

    for (int i = 0; i < tam; i++)
        V[i] = backup[i];


    // Inicia timer
    auto start = chrono::high_resolution_clock::now();
    
    cout << "\n[" << "100% Invertido." << "]\n";
    smartSorter->ordenador(V, tam, limPart, limQuebra);

    // Para timer
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);

    cout << "Tempo: " << duration.count() << " μs\n";    
   
    delete[] V;

    return 0;
}
