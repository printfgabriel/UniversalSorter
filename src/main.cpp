#include <iostream>
#include <fstream>  
#include <string>   

#include "../include/OrdenadorUniversal.hpp"
using namespace std;



int main(int argc, char *argv[]) {
    int tam;
    double a, b, c, limCusto;
    unsigned seed, tamVetCustos = 20;


    string nomeArquivo = argv[1];


    ifstream arquivo(nomeArquivo);  

    if (!arquivo.is_open()) {
        cerr << "ERRO: Não foi possível abrir o arquivo" << endl;
        return 1;  
    }

    arquivo >> seed >> limCusto >> a >> b >> c >> tam;  

    tipo *V = new tipo[tam];

    for (int i = 0; i < tam; i++)
        arquivo >> V[i];  // Lê os elementos do vetor

    arquivo.close();  // Fecha o arquivo


    OrdenadorUniversal *smartSorter = new OrdenadorUniversal(a, b, c, seed, tamVetCustos);
    
    cout << "size " << tam << " seed " << seed << " breaks " << smartSorter->numeroQuebras(V, tam) << endl;

    int limPart = smartSorter->determinaLimiarParticao(V, tam, limCusto);

   smartSorter->determinaLimiarQuebras(V, tam, limCusto, limPart);

    
    delete[] V;

    return 0;
}
