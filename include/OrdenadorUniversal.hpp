#ifndef ORDENADOR_HPP
#define ORDENADOR_HPP

class OrdenadorUniversal
{
    private:
        unsigned cmp;
        unsigned move;
        unsigned calls;
    public:
        OrdenadorUniversal();
        ~OrdenadorUniversal();  
    
        void ordenador(int *V, int tam, int minTamParticao, int limiarQuebras, int numeroQuebras);
        int determinaLimiarParticao(int *V, int tam, int limiarCusto);
        void calculaNovaFaixa(int limParticao, int minMPS, int maxMPS, int passoMPS);

        // Auxiliary methods
        void resetStats();
        void swap(int &a, int &b);

        // Quicksort methods
        int median(int a, int b, int c);
        void partition3(int * A, int l, int r, int *i, int *j);
        void quickSort3Ins(int * A, int l, int r);

        // InsertionSort methods
        void insertionSort(int V[], int l, int r);

    };

#endif