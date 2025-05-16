#include <iostream>
#include "../include/OrdenadorUniversal.hpp"
using namespace std;

int main() {
    int tam;
    double a, b, c, limCusto;
    unsigned seed;

    cin >> seed;


    cin >> limCusto >> a >> b >> c >> tam;

    tipo *V = new tipo[tam];

    for (int i = 0; i < tam; i++)
        cin >> V[i];    
    
    


    OrdenadorUniversal *smartSorter = new OrdenadorUniversal(a, b, c, seed);
    
    cout << "size " << tam << " seed " << seed << " breaks " << smartSorter->numeroQuebras(V, tam) << endl;

    int limPart = smartSorter->determinaLimiarParticao(V, tam, limCusto);

    int limQuebra = smartSorter->determinaLimiarQuebras(V, tam, limCusto, limPart);

    
    delete[] V;

    return 0;
}
