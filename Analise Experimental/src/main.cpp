#include "../include/OrdenadorUniversal.hpp"
#include <iostream>  
#include <stdlib.h>  
#include <math.h>    
#include <string>
#include <cstring>
#include <chrono>
#include <vector>
#include <Eigen/Dense>


using namespace std;

#define seed 42

tipo vet[VETSZ];




// Regressão linear via decomposição QR (mais robusta)
void fitLinearRegression(const double* X, const double* Y, int n, int p, Eigen::VectorXd& coefficients) {
    Eigen::MatrixXd Xmat(n, p + 1);
    Eigen::VectorXd Yvec(n);

    for (int i = 0; i < n; ++i) {
        Xmat(i, 0) = 1.0;
        for (int j = 0; j < p; ++j) {
            Xmat(i, j + 1) = X[i * p + j];
        }
        Yvec(i) = Y[i];
    }

    // Resolve min ||Xmat * coef - Yvec|| via QR decomposition
    coefficients = Xmat.colPivHouseholderQr().solve(Yvec);
}


void set_quebras(tipo *vet, int quantidade){
    OrdenadorUniversal *ordenador = new OrdenadorUniversal(1,2 , 3, 1);
    
    ordenador->insertionSort(vet, 0, VETSZ-1);
    cout << "Numero de quebras: " << ordenador->numeroQuebras(vet, VETSZ) << endl;
    ordenador->addQuebras(vet, VETSZ-1, quantidade);
    cout << "Numero de quebras: " << ordenador->numeroQuebras(vet, VETSZ) << endl;

    delete ordenador;

}



int main() {

    int i, j, p;
    long mult = (long)pow(10, KEYSZ - 1);
    srand48(1); 
    
    tipo vetorRegressao[1600];

    cout << "------ SETANDO AMOSTRAS --------" << endl << endl;
    
    
    
    int aux = 200;

    for (int i = 1; i < 5; i++){
        OrdenadorUniversal ordenadorAux(1,2,3,1);

        for (int k = 0; k < aux; k++) {
            for (j = (int)(drand48() * mult), p = KEYSZ - 2; p >= 0; j /= 10, p--)
            vetorRegressao[k].key[p] = '0' + j % 10;
            vetorRegressao[k].key[KEYSZ - 1] = 0;
            for (j = 0; j < PLSZ - 1; j++)
            vetorRegressao[k].payload[j] = '0' + j % 10;
            vetorRegressao[k].payload[PLSZ - 1] = 0;
        }

        cout << "Amostra tamanho " << aux << endl;

        ordenadorAux.quickSort3Ins(vetorRegressao, 0, aux-1, 50);
        ordenadorAux.resetStats();


        ordenadorAux.addQuebras(vetorRegressao, 1599, 200);


        auto start = chrono::high_resolution_clock::now();
        ordenadorAux.quickSort3Ins(vetorRegressao, 0, aux-1, 50);
        auto stop = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);

        ordenadorAux.registraEstatisticas(0, -1);
        ordenadorAux.imprimeEstatisticas(0);

        cout << "Tempo: " << duration.count() << " μs\n";    
        


        aux*=2;
    }
    






    for (i = 0; i < VETSZ; i++) {
        for (j = (int)(drand48() * mult), p = KEYSZ - 2; p >= 0; j /= 10, p--)
        vet[i].key[p] = '0' + j % 10;
        vet[i].key[KEYSZ - 1] = 0;
        for (j = 0; j < PLSZ - 1; j++)
        vet[i].payload[j] = '0' + j % 10;
        vet[i].payload[PLSZ - 1] = 0;
    }
    
    /*
    a --> comparações --> X1
    b --> movimentações --> X2
    c --> chamadas --> x3
    Y --> tempo
    Y = a*X1 + b*X2 + c*X3
    */
   
    
   
   int amostras = 4;
   double X[amostras * 3] = {
       1789,  1719,   16,    
       3817,  3245,   34,    
       10668, 9049,   79,
       14930, 8114,  139
    };
    double Y[amostras] = {15, 30, 80, 114};
    
    cout << "amostras = " << amostras
    << ", variaveis = " << 3 << std::endl;
    
    Eigen::VectorXd coefs;
    fitLinearRegression(X, Y, amostras, 3, coefs);
    
    set_quebras(vet, VETSZ-1);
    
    cout << "Coeficientes calibrados:\n";
    cout << "intercepto: " << coefs[0] << endl;
    cout << "a (comparações): " << coefs[1] << " seg/op\n";
    cout << "b (movimentações): " << coefs[2] << " seg/op\n";
    cout << "c (chamadas): " << coefs[3] << " seg/op\n";
    
    double a = 0.00754801, 
           b = -0.000474109, 
           c = 0.0231183, 
           limCusto = 10.0;
    
    
    OrdenadorUniversal *smartSorter = new OrdenadorUniversal(a, b, c, seed);
    
    cout << "size " << VETSZ << " seed " << seed << " breaks " << smartSorter->numeroQuebras(vet, VETSZ) << endl;
    
    tipo backup[VETSZ];
    for (int i = 0; i < VETSZ; i++){
        backup[i] = vet[i];
    }
    

    int limPart = smartSorter->determinaLimiarParticao(vet, VETSZ, limCusto);
    int limQuebra = smartSorter->determinaLimiarQuebras(vet, VETSZ, limCusto, limPart);
    
    smartSorter->resetCustos();
    smartSorter->resetStats();
    for (int i = 0; i < VETSZ; i++){
        vet[i] = backup[i];
    }

    cout << endl << endl << endl << "TAD - ";
    auto start = chrono::high_resolution_clock::now();
    smartSorter->ordenador(vet, VETSZ, limPart, limQuebra);
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);

    
    cout << "Tempo: " << duration.count() << " μs\n";    

    for (int i = 0; i < VETSZ; i++){
        vet[i] = backup[i];
    }
    smartSorter->resetStats();
    smartSorter->resetCustos();

    cout << endl << "QuickSort puro - ";
    start = chrono::high_resolution_clock::now();
    // smartSorter->quickSort3(vet, 0, VETSZ-1);
    smartSorter->quickSort3Ins(vet, 0, VETSZ-1,0);

    stop = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    smartSorter->registraEstatisticas(0, 11);
    smartSorter->imprimeEstatisticas(0);
    cout << "Tempo: " << duration.count() << " μs\n";    
    
    for (int i = 0; i < VETSZ; i++){
        vet[i] = backup[i];
    }
    smartSorter->resetStats();
    smartSorter->resetCustos();

    cout << endl << "Insertion Sort - ";
    start = chrono::high_resolution_clock::now();
    smartSorter->insertionSort(vet, 0, VETSZ-1);
    stop = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    smartSorter->registraEstatisticas(0, 11);
    smartSorter->imprimeEstatisticas(0);
    cout << "Tempo: " << duration.count() << " μs\n";    
    

}