#ifndef ORDENADOR_HPP
#define ORDENADOR_HPP

class OrdenadorUniversal
{
    private:
        void ordenador(int &V, int tam, int minTamParticao, int limiarQuebras);
        int determinaLimiarParticao(int &V, int tam, int limiarCusto);
        void calculaNovaFaixa(int limParticao, int minMPS, int maxMPS, int passoMPS);
        
    public:
        OrdenadorUniversal(/* args */);
        ~OrdenadorUniversal();  
};

#endif