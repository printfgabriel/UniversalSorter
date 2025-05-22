#include "../include/OrdenadorUniversal.hpp"
#include <iostream>  
#include <stdlib.h>  
#include <math.h>    
#include <string>
#include <cstring>
#include <chrono>
#include <vector>
#include <Eigen/Dense>
#include <limits>



using namespace std;

#define seed 42


// Regressão linear via decomposição QR
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
    cout << "[" << quantidade << "]Numero de quebras: " << ordenador->numeroQuebras(vet, VETSZ) << endl;

    delete ordenador;

}


void fitLinearRegressionNNLS(const double* X, const double* Y, int n, int p, Eigen::VectorXd& coefficients) {
    const double lambda = 10.0;
    const double lr = 5e-3;
    const int maxIters = 1000;

    std::vector<double> mean(p, 0.0), stddev(p, 0.0);
    for (int j = 0; j < p; ++j) {
        double sum = 0.0;
        for (int i = 0; i < n; ++i) sum += X[i * p + j];
        mean[j] = sum / n;

        double var = 0.0;
        for (int i = 0; i < n; ++i)
            var += std::pow(X[i * p + j] - mean[j], 2);
        stddev[j] = std::sqrt(var / n);

        if (stddev[j] < 1e-6) stddev[j] = 1.0;
    }

    Eigen::MatrixXd Xmat(n, p + 1);
    Eigen::VectorXd Yvec(n);
    for (int i = 0; i < n; ++i) {
        Xmat(i, 0) = 1.0;
        for (int j = 0; j < p; ++j) {
            double normVal = (X[i * p + j] - mean[j]) / stddev[j];
            Xmat(i, j + 1) = normVal;
        }
        Yvec(i) = Y[i];
    }

    int m = p + 1;
    coefficients = Eigen::VectorXd::Zero(m);

    for (int iter = 0; iter < maxIters; ++iter) {
        Eigen::VectorXd pred = Xmat * coefficients;
        Eigen::VectorXd grad = 2.0 * Xmat.transpose() * (pred - Yvec);

        for (int j = 0; j < m; ++j) {
            if (j == 0) {
                // Não penaliza o intercepto
            } else if (coefficients(j) > 0) {
                grad(j) += lambda;
            } else if (coefficients(j) < 0) {
                grad(j) -= lambda;
            }
        }

        coefficients -= lr * grad;

        for (int j = 0; j < m; ++j) {
            if (coefficients(j) < 0.0) {
                coefficients(j) = 0.0;
            }
        }

        if (coefficients.hasNaN()) {
            break;
        }
    }

    for (int j = 0; j < p; ++j) {
        coefficients(j + 1) /= stddev[j];
    }

    double intercept = coefficients(0);
    for (int j = 0; j < p; ++j) {
        intercept -= coefficients(j + 1) * mean[j];
    }
    coefficients(0) = intercept;
}




int main() {

    int i, j, p;
    srand48(1); 
    
    tipo *vetorRegressao = new tipo[1603];
    
    cout << "------ SETANDO AMOSTRAS --------" << endl << endl;
    
    
    
    int aux = 200;
    
    for (int i = 1; i < 5; i++){
        OrdenadorUniversal ordenadorAux(1,2,3,1);
        
        for (int k = 0; k < aux; k++) {
            for (p = 0; p < KEYSZ; ++p)
                vetorRegressao[i].key[p] = '0' + (int)(drand48() * 10);
            vetorRegressao[i].key[KEYSZ] = '\0';
            for (j = 0; j < PLSZ; j++)
                vetorRegressao[k].payload[j] = '0' + j % 10;
            vetorRegressao[k].payload[PLSZ] = '\0';
        }
        
        
        cout << "Amostra tamanho " << aux << endl;
        
        ordenadorAux.quickSort3Ins(vetorRegressao, 0, aux-1, 50);

        ordenadorAux.resetStats();
        
        
        // ordenadorAux.addQuebras(vetorRegressao, 1599, (int)(0.2*aux));
        
        auto start = chrono::high_resolution_clock::now();
        ordenadorAux.quickSort3Ins(vetorRegressao, 0, aux-1, 50);
        auto stop = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
        
        ordenadorAux.registraEstatisticas(0, -1);
        ordenadorAux.imprimeEstatisticas(0);
        
        cout << "Tempo: " << duration.count() << " μs\n";    
        
        
        
        aux*=2;
    }
    
    
    
    delete[] vetorRegressao;
    
    tipo *vet = new tipo[VETSZ];
    
    
    
    for (i = 0; i < VETSZ; i++) {
        for (p = 0; p < KEYSZ; ++p) {
            vet[i].key[p] = '0' + (int)(drand48() * 10);
        }
        vet[i].key[KEYSZ - 1] = 0;
        for (j = 0; j < PLSZ; j++)
        vet[i].payload[j] = '0' + j % 10;
        vet[i].payload[PLSZ] = 0;
        
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
       595,  395,   10,    
       1588,  791,   22,    
       3973, 1585,   46,
       9547, 3175,  94
    };
    double Y[amostras] = {4, 10, 24, 60};
    
    cout << "amostras = " << amostras
    << ", variaveis = " << 3 << std::endl;
    
    Eigen::VectorXd coefs;
    fitLinearRegressionNNLS(X, Y, amostras, 3, coefs);
    
    // set_quebras(vet, (int)(0.2*VETSZ));
    
    cout << "Coeficientes calibrados:\n";
    cout << "intercepto: " << coefs[0] << endl;
    cout << "a (comparações): " << coefs[1] << " seg/op\n";
    cout << "b (movimentações): " << coefs[2] << " seg/op\n";
    cout << "c (chamadas): " << coefs[3] << " seg/op\n";
    
    double a = 0.00245996,  
           b = 0.0056141, 
           c = 0.184971, 
           limCusto = 10.0;
    
    OrdenadorUniversal *smartSorter = new OrdenadorUniversal(a, b, c, seed);
    
    cout << "size " << VETSZ << " seed " << seed << " breaks " << smartSorter->numeroQuebras(vet, VETSZ) << endl;
    
    tipo *backup = new tipo[VETSZ];

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
    smartSorter->registraEstatisticas(0, 0);
    smartSorter->imprimeEstatisticas(0);
    cout << "Tempo: " << duration.count() << " μs\n";    
    
    delete[] backup;
    delete[] vet;
}