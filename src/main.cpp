#include <iostream>
#include "../include/OrdenadorUniversal.hpp"
using namespace std;

int main() {
    int limCusto, tam;
    long double a, b, c;

    cin >> limCusto >> a >> b >> c >> tam;

    tipo *V = new tipo[tam];

    for (int i = 0; i < tam; i++)
        cin >> V[i];    
    
    
    OrdenadorUniversal *smartSorter = new OrdenadorUniversal(a, b, c);
    
    unsigned limPart = smartSorter->determinaLimiarParticao(V, tam, limCusto);

    unsigned limQuebra = smartSorter->determinaLimiarQuebras(V, tam, limCusto);

    
    delete[] V;

    return 0;
}
