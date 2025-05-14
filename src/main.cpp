#include <iostream>
#include "../include/OrdenadorUniversal.hpp"
using namespace std;

int main() {
    int tam;
    long double a, b, c, limCusto;
    unsigned seed;

    cin >> seed;


    cin >> limCusto >> a >> b >> c >> tam;

    tipo *V = new tipo[tam];

    for (int i = 0; i < tam; i++)
        cin >> V[i];    
    
    
    OrdenadorUniversal *smartSorter = new OrdenadorUniversal(a, b, c, seed);
    
    unsigned limPart = smartSorter->determinaLimiarParticao(V, tam, limCusto);

    unsigned limQuebra = smartSorter->determinaLimiarQuebras(V, tam, limCusto);

    
    delete[] V;

    return 0;
}
