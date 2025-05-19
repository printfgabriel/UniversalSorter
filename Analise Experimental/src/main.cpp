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


#include <Eigen/Dense>

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
        std::cout << "Xmat:\n" << Xmat << "\n\n";
        std::cout << "Yvec:\n" << Yvec.transpose() << "\n\n";

    }

    // Resolve min ||Xmat * coef - Yvec|| via QR decomposition
    coefficients = Xmat.colPivHouseholderQr().solve(Yvec);
}




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
   
    /*
        a --> comparações --> X1
        b --> movimentações --> X2
        c --> chamadas --> x3

        Y = a*X1 + b*X2 + c*X3
    */

    int amostras = 4;
    double X[amostras * 3] = {
        10,  5,   2,    
        20,  7,   4,    
        30, 11,   6,
        40, 17,  10
    };

    std::cout << "amostras = " << amostras
          << ", variaveis = " << 3 << std::endl;


    double Y[amostras] = {31, 51, 75, 109};

    Eigen::VectorXd coefs;
    fitLinearRegression(X, Y, amostras, 3, coefs);

    cout << "Coeficientes calibrados:\n";
    cout << "intercepto: " << coefs[0] << endl;
    cout << "a (comparações): " << coefs[1] << " seg/op\n";
    cout << "b (movimentações): " << coefs[2] << " seg/op\n";
    cout << "c (chamadas): " << coefs[3] << " seg/op\n";



    // OrdenadorUniversal *smartSorter = new OrdenadorUniversal(a, b, c, seed);

    // cout << "size " << VETSZ << " seed " << seed << " breaks " << smartSorter->numeroQuebras(vet, VETSZ) << endl;

    // int limPart = smartSorter->determinaLimiarParticao(vet, VETSZ, limCusto);
    // int limQuebra = smartSorter->determinaLimiarQuebras(vet, VETSZ, limCusto, limPart);


    // auto start = chrono::high_resolution_clock::now();
    
    // smartSorter->ordenador(vet, VETSZ, limPart, limQuebra);

    // // Para timer
    // auto stop = chrono::high_resolution_clock::now();
    // auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);

    // cout << "Tempo: " << duration.count() << " μs\n";    
   

}