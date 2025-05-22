#ifndef ORDENADOR_HPP
#define ORDENADOR_HPP

#include <cstring>

using namespace std;

typedef struct tipo {
    char key[KEYSZ+1];
    char payload[PLSZ+1];

    bool operator<(const tipo& other) const {
        return strcmp(key, other.key) < 0;
    }
    bool operator>(const tipo& other) const {
        return strcmp(key, other.key) > 0;
    }
    bool operator<=(const tipo& other) const {
        return strcmp(key, other.key) <= 0;
    }
    bool operator>=(const tipo& other) const {
        return strcmp(key, other.key) >= 0;
    }
    bool operator==(const tipo& other) const {
        return strcmp(key, other.key) == 0;
    }
    bool operator!=(const tipo& other) const {
        return !(*this == other);
    }
} tipo;


class OrdenadorUniversal
{
    struct Estatisticas{
        unsigned cmp, move, calls, limTestado;
        long double valorCusto;
    };
    

    private:
        double a, b, c;
        unsigned cmp, move, calls, seed;

        Estatisticas *custos;
        
    public:
        OrdenadorUniversal(double a, double b, double c, unsigned seed);
        ~OrdenadorUniversal();  
    
        // limiarCusto
        int determinaLimiarQuebras(tipo *V, int tam, double limiarCusto, int limParticao);
        void calculaNovaFaixaQuebra(int limQuebras, int &minQuebras, int &maxQuebras, int &passoQuebras, int numQuebras, float &lqdiff);

        // limiarParticao
        void ordenador(tipo *V, int tam, int minTamParticao, int limiarQuebras);

        int determinaLimiarParticao(tipo *V, int tam, double limiarCusto);
        void calculaNovaFaixaParticao(int limParticao, int &minMPS, int &maxMPS, int &passoMPS, int numMPS, float &mpsdiff);


        void registraEstatisticas(int numMPS, unsigned t);
        void imprimeEstatisticas(int numMPS);

        // Auxiliary methods
        int numeroQuebras(tipo *V, int tam);
        void addQuebras(tipo *V, int tamanho, int quantidade);
        int menorCustoParticao(int tam);
        int menorCustoQuebras(int num);
        void resetStats();
        void resetCustos();
        void swap(tipo &a, tipo &b);

        // Quicksort methods
        tipo median(tipo& a, tipo& b, tipo& c);
        void partition3(tipo * A, int l, int r, int *i, int *j);
        void quickSort3Ins(tipo * A, int l, int r, int partition);

        // InsertionSort methods
        void insertionSort(tipo V[], int l, int r);

        void quickSort3(tipo * A, int l, int r);


    };
    
    
#endif