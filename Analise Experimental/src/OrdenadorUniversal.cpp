#include "./../include/OrdenadorUniversal.hpp"

#include <iostream>
#include <iomanip>
#include <cstring>

#define tamVetCustos 15

using namespace std;

OrdenadorUniversal::OrdenadorUniversal(double a, double b, double c, unsigned seed) 
    : a(a), b(b), c(c), seed(seed){
    resetStats();
    custos = new Estatisticas[tamVetCustos];
}

OrdenadorUniversal::~OrdenadorUniversal(){
    delete[] custos;
}


void OrdenadorUniversal::ordenador(tipo *V, int tam, int minTamParticao, int limiarQuebras) {
    // Programação Defensiva.
    if (V == nullptr){
        cerr << "Erro: Array de entrada V é nulo em OrdenadorUniversal::ordenador." << endl;
        return;
    }
    int numQuebras = numeroQuebras(V, tam);

    // Analise Experimental
    resetCustos();

    if (numQuebras < limiarQuebras){
        insertionSort(V, 0, tam-1);   
    }
    else if (tam > minTamParticao) {
        quickSort3Ins(V, 0, tam-1, minTamParticao) ;
    }
    else {
        insertionSort(V, 0, tam-1);
    }
    
    // Analise Experimental
    registraEstatisticas(0, tamVetCustos);
    imprimeEstatisticas(0);
}

/*
    Determina a partir de qual quantidade de quebras vale a pena trocar os algoritmos
    de ordenação.
*/
int OrdenadorUniversal::determinaLimiarQuebras(tipo *V, int tam, double limiarCusto, int limParticao){
    // Programação Defensiva.
    if (V == nullptr){
        cerr << "Erro: Array de entrada V é nulo em OrdenadorUniversal::determinaLimiarQuebras." << endl;
        return -1;
    }
    if(custos == nullptr){
        cerr << "Erro: Array de custos é nulo em OrdenadorUniversal::determinaLimiarQuebras." << endl;
        return -1;
    }


    // Para não perder o vetor V original
    insertionSort(V, 0, tam-1);
    resetStats();
    
    
    int limiarQuebras,
        minQuebras = 1,         
        maxQuebras = tam/2,
        numQuebras,
        index = 0,
        passoQuebra = (int)(maxQuebras - minQuebras)/5;
    
    float lqdiff;
    
    
    do {
        cout << fixed << setprecision(9);
        numQuebras = 0;
        cout << endl <<  "iter " << index << endl;
        for (int lq = minQuebras; lq <= maxQuebras; lq += passoQuebra) {
            
    
            addQuebras(V, tam,lq);

            // QuickSort somente
            quickSort3Ins(V, 0, tam-1, limParticao);
            // ordenador(V, tam, limParticao, -1); // 'tam' como minTamParticao para forçar a escolha baseada no limiar de quebras
            registraEstatisticas(2*numQuebras, lq); // Registra as estatísticas para este limiar de quebras
            cout << "qs lq " << lq << " ";
            imprimeEstatisticas(2*numQuebras);
            resetStats();


            addQuebras(V, tam, lq);

            // InsertionSort somente - sobreescrevendo as stats do quick --> a que importa é a do insertion
            // ordenador(V, tam, 0, tam); // 'tam' como minTamParticao para forçar a escolha baseada no limiar de quebras
            insertionSort(V, 0, tam-1);
            registraEstatisticas(2*numQuebras+1, lq); // Registra as estatísticas para este limiar de quebras
            cout << "in lq " << lq << " ";
            imprimeEstatisticas(2*numQuebras+1);
            resetStats();


            numQuebras++;
        }
        // Indice com menor custo (do Insertion puro)
        limiarQuebras = menorCustoQuebras(2*numQuebras); // Encontra o limiar de quebras com menor custo registrado
        calculaNovaFaixaQuebra(limiarQuebras, minQuebras, maxQuebras, passoQuebra, numQuebras, lqdiff);


        index++;
        resetCustos();
        cout << fixed << setprecision(6);
        cout << "numlq " << numQuebras << " limQuebras " << custos[2*limiarQuebras].limTestado << " lqdiff " << lqdiff << endl; // ARRUMAR - o que é qdiff
    } while ((lqdiff > limiarCusto) && (numQuebras >= 5));
    
        // } while ((lqdiff > limiarCusto) && (numQuebras >= 10));

    return custos[limiarQuebras].limTestado; 
}

/*
    Estreita a quantidade de quebras a serem testadas ao determinar o limiar de quebras.
*/
void OrdenadorUniversal::calculaNovaFaixaQuebra(int limQuebras, int &minQuebras, int &maxQuebras, int &passoQuebras, int numQuebras, float &lqdiff){
    int newMin, newMax;
    
    if (limQuebras == 0) {
        newMin = 0;
        newMax = 2;
    } else if (limQuebras == numQuebras - 1) {
        newMin = numQuebras - 3;
        newMax = numQuebras - 1;
    } else {
        newMin = limQuebras - 1;
        newMax = limQuebras + 1;
    }
    
    minQuebras = custos[2*newMin].limTestado;
    maxQuebras = custos[2*newMax].limTestado;
    
    lqdiff = custos[2*newMax +1].valorCusto - custos[2*newMin +1].valorCusto;
    lqdiff = lqdiff >= 0 ? lqdiff : -lqdiff;

    passoQuebras = (int)(maxQuebras - minQuebras) / 5;
    
    if (passoQuebras == 0) {
        passoQuebras++;
    }
}


/*
    Determina a partir de qual tamanho de partição vale a pena trocar os algoritmos
    de ordenação.
*/
int OrdenadorUniversal::determinaLimiarParticao(tipo *V, int tam, double limiarCusto) {
    // Programação Defensiva.
    if (V == nullptr){
        cerr << "Erro: Array de entrada V é nulo em OrdenadorUniversal::determinaLimiarParticao." << endl;
        return -1;
    }
    if(custos == nullptr){
        cerr << "Erro: Array de custos é nulo em OrdenadorUniversal::determinaLimiarParticao." << endl;
        return -1;
    }

    // Vetor de backup do vetor desordenado original 
    tipo *backup = new tipo[tam];
    for (int i = 0; i < tam; i++) {
        backup[i] = V[i];
    }

    int minMPS_valor = 2, 
        numMPS, 
        maxMPS_valor = tam,
        passoMPS = (int) (maxMPS_valor - minMPS_valor)/ 5,
        limParticao,
        index = 0;    
    float mpsdiff;
        
    do {
        cout << fixed << setprecision(9);
        numMPS = 0;
        cout << endl << "iter " << index << endl;
        
        for (int t = minMPS_valor; t <= maxMPS_valor; t += passoMPS) {
            // Para restaurar o V original
            for (int i = 0; i < tam; i++) {
                V[i] = backup[i];
            }
            // ordenador(V, tam, t, -1);
            
            // Testar com o t atual
            quickSort3Ins(V, 0, tam-1, t); // r é inclusivo, entao é tam-1 --> TESTADO!
            registraEstatisticas(numMPS, t);
            cout << "mps " << t << " ";
            imprimeEstatisticas(numMPS);
            resetStats(); 

            numMPS++;
        }

        // Acha índice de menor custo e calcula novos valores
        limParticao = menorCustoParticao(numMPS);
        calculaNovaFaixaParticao(limParticao, minMPS_valor, maxMPS_valor, passoMPS, numMPS, mpsdiff);
        index++;
        resetCustos();

        cout << fixed << setprecision(6);
        cout << "nummps " << numMPS << " limParticao " << custos[limParticao].limTestado << " mpsdiff " << mpsdiff << endl; // ARRUMAR - oq é mpsdiff

    } while ((mpsdiff > limiarCusto) && (numMPS >= 5));

    delete[] backup;
    return custos[limParticao].limTestado;
}

/*
    Estreita a faixa de partições a serem testadas ao determinar o limiar de partição.
*/
void OrdenadorUniversal::calculaNovaFaixaParticao(int limParticao, int &minMPS, int &maxMPS, int &passoMPS, int numMPS, float &mpsdiff){
    int newMin, newMax;
    
    if (limParticao == 0) {
        newMin = 0;
        newMax = 2;
    } else if (limParticao == numMPS - 1) {
        newMin = numMPS - 3;
        newMax = numMPS - 1;
    } else {
        newMin = limParticao - 1;
        newMax = limParticao + 1;
    }    
      
    minMPS = custos[newMin].limTestado;
    maxMPS = custos[newMax].limTestado;

    mpsdiff = custos[newMax].valorCusto - custos[newMin].valorCusto;
    mpsdiff = mpsdiff >= 0 ? mpsdiff : -mpsdiff;

    passoMPS = (int)(maxMPS - minMPS) / 5;
    
    if (passoMPS == 0) {
        passoMPS++;
    }
}

/*
    Calcula o menor custo ao comparar no vetor de custos para determinar limParticao
*/
int OrdenadorUniversal::menorCustoParticao(int num) {
    int menorIdx = 0;
    for (int i = 1; i < num; i++) {
        if ( (custos[i].valorCusto >=0)  && (custos[i].valorCusto < custos[menorIdx].valorCusto) )
            menorIdx = i;
    }
    return menorIdx;
}

/*
    Calcula o menor custo ao comparar no vetor de custos para determinar limQuebras
    Aqui, as posições 2k e 2k+1 possuem, respectivamente, a
    o custo do QuickSort e do InsertionSort do teste k
*/
int OrdenadorUniversal::menorCustoQuebras(int num) {
    int menorIdx = 0;
    double diffAtual = custos[1].valorCusto - custos[0].valorCusto;
    diffAtual = diffAtual < 0 ? -diffAtual : diffAtual; 


    for (int i = 2; i < num; i+=2) {
        double diffAux = custos[i+1].valorCusto - custos[i].valorCusto;
        diffAux = diffAux < 0 ? -diffAux : diffAux;

        if(diffAux < diffAtual){
            diffAtual = diffAux;
            menorIdx = i;
        }

    }
    return menorIdx/2;
}

void OrdenadorUniversal::registraEstatisticas(int index, unsigned t){
    if(index >=0 && index < tamVetCustos){
        custos[index].limTestado = t;
        custos[index].calls = calls;
        custos[index].cmp = cmp;
        custos[index].move = move;
        custos[index].valorCusto = a*cmp + b*move + c*calls;
    }
    else{
        cerr << "ERRO: tentativa de registrar estatisticas em posição inválida do vetor custos";
    }
}

void OrdenadorUniversal::imprimeEstatisticas(int index){
    if(index >=0 && index < tamVetCustos){
    cout << "cost " << custos[index].valorCusto << " "
         << "cmp " << custos[index].cmp <<  " "
         << "move " << custos[index].move << " "
         << "calls " << custos[index].calls << endl;
    }
    else{
        cerr << "ERRO: tentativa de imprimir estatisticas de posição inválida do vetor custos";
    }
}

int OrdenadorUniversal::numeroQuebras(tipo *V, int tam) {
    if(V == nullptr){
        cerr << "ERRO: vetor V é nulo em OrdenadorUniversal::numeroQuebras";
        return -1;
    }

    int quebras = 0;

    for (int i = 1; i < tam; i++) {
        // cmp++; // será?
        if (V[i] < V[i-1]) 
            quebras++;
    }
    return quebras;
}

void OrdenadorUniversal::addQuebras(tipo *V, int tamanho, int quantidade){
    if(V == nullptr){
        cerr << "ERRO: vetor V é nulo em OrdenadorUniversal::addQuebras";
        return;
    }

    int p1 = 0, p2 = 0;
    tipo temp;
    srand48(seed);

    for (int t = 0; t < quantidade; t++) {

        while (p1 == p2) {
            p1 = (int)(drand48() * tamanho);
            p2 = (int)(drand48() * tamanho);
        }
        
        /* Realiza a troca para introduzir uma quebra */
        temp = V[p1];
        V[p1] = V[p2];
        V[p2] = temp;
        p1 = p2 = 0;
    }
}

void OrdenadorUniversal::resetStats(){
    cmp = 0;
    move = 0;
    calls = 0;
}

/*
    Reseta o vetor de custos
*/
void OrdenadorUniversal::resetCustos(){
    for (int i = 0; i < tamVetCustos; i++)
        custos[i].valorCusto = -1;
}

/*
    Troca dois itens de um vetor de posição
*/
void OrdenadorUniversal::swap(tipo &a, tipo &b){
    tipo temp = a;
    a=b;
    b=temp;
    move+=3;
    
}

/*
    Encontra a mediana de três números
*/
tipo OrdenadorUniversal::median(tipo a, tipo b, tipo c){
    if(a >= b)  
        if (b>=c)
            return b;
        else
            return a>=c? c : a;
    else
        if (a>=c)
            return a;
        else
            return b>=c? c : b;
        
}

/*
    Função auxiliar do QuickSort. Calcula sua partição.
*/
void OrdenadorUniversal::partition3(tipo * V, int l, int r, int *i, int *j){
    *i = l;
    *j = r;  
  
    int meio = l + (r - l) / 2;
    
    tipo pivot_val = median(V[l], V[meio], V[r]);
  
    int pivot_index;
    if (pivot_val == V[l]) pivot_index = l;
    else if (pivot_val == V[meio]) pivot_index = meio;
    else pivot_index = r;
  
    tipo pivot = V[pivot_index];
  
    do {
        while (pivot > V[*i]) {
            cmp++;
            (*i)++;
        }
        while (pivot < V[*j]) {
            cmp++;
            (*j)--;
        }
       
        cmp += 2;
  
        if (*i <= *j) {
            swap(V[*i], V[*j]);
            (*i)++;
            (*j)--;
        }
    } while (*i <= *j);
}



/*
    QuickSort com Insertion Sort para certos tamanhos de partições.
*/
void OrdenadorUniversal::quickSort3Ins(tipo * V, int l, int r, int partition) { 
    // Programação Defensiva.
    if (V == nullptr){
        cerr << "Erro: Array de entrada V é nulo em OrdenadorUniversal::quickSort3Ins." << endl;
        return;
    }

    calls+=2; // para partição e pro quickSort3Ins
    int i, j;
    partition3(V, l, r, &i, &j);

    if(j > l){
      if(j - l < partition){
        insertionSort(V, l, j);
      }
      else{
        quickSort3Ins(V, l, j, partition);
      }
    }
    if(r > i){
      if(r - i < partition){
        insertionSort(V, i, r);  
      }
      else{
        quickSort3Ins(V, i, r, partition);
      }
    }
  }
  

  /*
    Algoritmo de ordenação Insertion Sort
  */
void OrdenadorUniversal::insertionSort(tipo V[], int l, int r){
    // Programação Defensiva.
    if (V == nullptr){
        cerr << "Erro: Array de entrada V é nulo em OrdenadorUniversal::insertionSort" << endl;
        return;
    }

    calls++;
    int i, j;
    tipo key;

    for (i = l + 1; i <= r; i++){
        key = V[i];
        move++;
        j = i - 1;
        int did_break = 0;

        while (j >= l){
            cmp++;
            if (key >= V[j]){
                did_break = 1;
                break;
            }
            V[j+1] = V[j];
            move++;
            j--;
        }

        if (!did_break)
                cmp++;

        V[j + 1] = key;
        
        move++;
    }

    return;
}

