#ifndef ORDENADOR_HPP
#define ORDENADOR_HPP

typedef int tipo;

class OrdenadorUniversal
{
    struct Estatisticas{
        unsigned cmp, move, calls, mps;
        long double valorCusto;
    };
    

    private:
        unsigned cmp, move, calls, seed;
        long double a, b, c;

        Estatisticas *custos;
        
    public:
        OrdenadorUniversal(long double a, long double b, long double c, unsigned seed);
        ~OrdenadorUniversal();  
    
        // limiarCusto
        int determinaLimiarQuebras(tipo *V, int tam, long double limiarCusto);
        void calculaNovaFaixaQuebra(int limQuebras, int &minQuebras, int &maxQuebras, int &passoQuebras, int numQuebras, long double &lqdiff);

        // limiarParticao
        void ordenador(tipo *V, int tam, int minTamParticao, int limiarQuebras);
        int determinaLimiarParticao(tipo *V, int tam, long double limiarCusto);
        void calculaNovaFaixa(int limParticao, int &minMPS, int &maxMPS, int &passoMPS, int numMPS, long double &mpsdiff);
        void registraEstatisticas(int numMPS, unsigned t);
        void imprimeEstatisticas(int numMPS);

        // Auxiliary methods
        int numeroQuebras(tipo *V, int tam);
        void addQuebras(tipo *V, int tamanho, int quantidade);
        int menorCusto(int tam);
        int menorCustoQuebras(int num);
        void resetStats();
        void resetCustos();
        void swap(tipo &a, tipo &b);

        // Quicksort methods
        int median(tipo a, tipo b, tipo c);
        void partition3(tipo * A, int l, int r, int *i, int *j);
        void quickSort3(tipo * A, int l, int r);
        void quickSort3Ins(tipo * A, int l, int r, int partition);

        // InsertionSort methods
        void insertionSort(tipo V[], int l, int r);

    };

#endif