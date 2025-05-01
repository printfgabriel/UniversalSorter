#ifndef ORDENADOR_HPP
#define ORDENADOR_HPP

class OrdenadorUniversal
{
    struct Estatisticas{
        unsigned cmp, move, calls, mps;
        long double valorCusto;
    };
    

    private:
        unsigned cmp, move, calls;
        double a, b, c;
        Estatisticas *custos;
        
    public:
        OrdenadorUniversal(int tam);
        ~OrdenadorUniversal();  
    
        // limiarCusto
        int determinaLimiarQuebras(int *V, int tam, int limiarCusto);

        // limiarParticao
        void ordenador(int *V, int tam, int minTamParticao, int limiarQuebras);
        int determinaLimiarParticao(int *V, int tam, int limiarCusto);
        void calculaNovaFaixa(int limParticao, int minMPS, int maxMPS, int passoMPS, int numMPS);
        void registraEstatisticas(int numMPS, unsigned t);
        void imprimeEstatisticas(int numMPS);

        // Auxiliary methods
        int numeroQuebras(int *V, int tam);
        int menorCusto(int tam);
        void resetStats();
        void resetCustos();
        void swap(int &a, int &b);

        // Quicksort methods
        int median(int a, int b, int c);
        void partition3(int * A, int l, int r, int *i, int *j);
        void quickSort3Ins(int * A, int l, int r);

        // InsertionSort methods
        void insertionSort(int V[], int l, int r);

    };

#endif