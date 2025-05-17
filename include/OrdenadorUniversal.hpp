#ifndef ORDENADOR_HPP
#define ORDENADOR_HPP

typedef int tipo;

class OrdenadorUniversal
{
    struct Estatisticas{
        unsigned cmp, move, calls, limTestado;
        long double valorCusto;
    };
    

    private:
        double a, b, c;
        unsigned cmp, move, calls, seed, tamVetCustos;

        Estatisticas *custos;
        
    public:
        OrdenadorUniversal(double a, double b, double c, unsigned seed, unsigned tamVetCustos);
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
        int median(tipo a, tipo b, tipo c);
        void partition3(tipo * A, int l, int r, int *i, int *j);
        void quickSort3Ins(tipo * A, int l, int r, int partition);

        // InsertionSort methods
        void insertionSort(tipo V[], int l, int r);

    };

#endif