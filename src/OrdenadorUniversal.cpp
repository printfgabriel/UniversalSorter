#include "./../include/OrdenadorUniversal.hpp"

#include <iostream>


using namespace std;

OrdenadorUniversal::OrdenadorUniversal(long double a, long double b, long double c, unsigned seed) : a(a), b(b), c(c), seed(seed){
    resetStats();
    custos = new Estatisticas[20];
}

OrdenadorUniversal::~OrdenadorUniversal(){
    delete[] custos;
}


void OrdenadorUniversal::ordenador(tipo *V, int tam, int minTamParticao, int limiarQuebras) {
    int numQuebras = numeroQuebras(V, tam);
    // cout << "lq = " << limiarQuebras << "  |  numquebras = " << numQuebras << endl;

    if(limiarQuebras == -1) // caso especial
        quickSort3Ins(V, 0, tam-1, minTamParticao);

    else if (numQuebras < limiarQuebras){
        insertionSort(V, 0, tam-1);   
    }
    else if (tam > minTamParticao) {
        quickSort3(V, 0, tam-1) ;
    }
    else {
        insertionSort(V, 0, tam-1);
    }
    
}

int OrdenadorUniversal::determinaLimiarQuebras(tipo *V, int tam, long double limiarCusto){
    // Para não perder o vetor V original
    int *backup = new int[tam];
    for (int i = 0; i < tam; i++) {
        backup[i] = V[i];
    }

    insertionSort(backup, 0, tam);
    resetStats();
    
    
    int limiarQuebras,
    minQuebras = 1, 
    maxQuebras = tam-1, // só pode ter tam-1 quebra, matematicamente
    numQuebras,
    index = 0,
    passoQuebra = (int)(maxQuebras - minQuebras)/5;
    
    long double lqdiff;
    
    srand48(seed);
    do {
        numQuebras = 0;
        cout << endl <<  "iter " << index << endl;
        for (int lq = minQuebras; lq <= maxQuebras; lq += passoQuebra) {
            
            // voltar ao ordenado e adicionar lq quebras
            for (int i = 0; i < tam; i++) {
                V[i] = backup[i];
            }
            addQuebras(V, tam,lq);

            // QuickSort somente
            ordenador(V, tam, 0, 0); // 'tam' como minTamParticao para forçar a escolha baseada no limiar de quebras
            registraEstatisticas(numQuebras, lq); // Registra as estatísticas para este limiar de quebras
            cout << "qs lq " << lq << " ";
            imprimeEstatisticas(numQuebras);
            resetStats();

            numQuebras++;

            // voltar ao ordenado e adicionar lq quebras
            for (int i = 0; i < tam; i++) {
                V[i] = backup[i];
            }
            addQuebras(V, tam, lq);

            // InsertionSort somente - sobreescrevendo as stats do quick --> a que importa é a do insertion
            ordenador(V, tam, 0, tam); // 'tam' como minTamParticao para forçar a escolha baseada no limiar de quebras
            registraEstatisticas(numQuebras, lq); // Registra as estatísticas para este limiar de quebras
            cout << "in lq " << lq << " ";
            imprimeEstatisticas(numQuebras);
            resetStats();


            numQuebras++;
        }
        // Indice com menor custo (do Insertion puro)
        limiarQuebras = menorCustoQuebras(numQuebras); // Encontra o limiar de quebras com menor custo registrado
        calculaNovaFaixaQuebra(limiarQuebras, minQuebras, maxQuebras, passoQuebra, numQuebras, lqdiff);
        // diffCusto = custos[0].valorCusto - custos[numQuebras-1].valorCusto;
        // diffCusto = diffCusto < 0? -diffCusto : diffCusto;

        index++;
        resetCustos();
        cout << endl << "numq " << index - 1 << " limQuebras " << custos[limiarQuebras].mps << " lqdiff " << lqdiff << endl; // ARRUMAR - o que é qdiff
    } while ((lqdiff > limiarCusto) && (numQuebras >= 10));

    delete[] backup;
    return custos[limiarQuebras].mps; 
}


void OrdenadorUniversal::calculaNovaFaixaQuebra(int limQuebras, int &minQuebras, int &maxQuebras, int &passoQuebras, int numQuebras, long double &lqdiff){
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
    
    minQuebras = custos[2*newMin].mps;
    maxQuebras = custos[2*newMax].mps;

    lqdiff = custos[2*newMax +1].valorCusto - custos[2*newMin +1].valorCusto;
    lqdiff = lqdiff >= 0 ? lqdiff : -lqdiff;

    passoQuebras = (int)(maxQuebras - minQuebras) / 5;
    
    if (passoQuebras == 0) {
        passoQuebras++;
    }
}



int OrdenadorUniversal::determinaLimiarParticao(tipo *V, int tam, long double limiarCusto) {
    // Para não perder o vetor V original
    int *backup = new int[tam];
    for (int i = 0; i < tam; i++) {
        backup[i] = V[i];
    }

    int minMPS_valor = 2, 
        numMPS, 
        maxMPS_valor = tam,
        passoMPS = (int) (maxMPS_valor - minMPS_valor)/ 5,
        limParticao,
        index = 0;
    long double mpsdiff;
        
    do {
        numMPS = 0;
        cout << endl << "iter " << index << endl;
        
        for (int t = minMPS_valor; t <= maxMPS_valor; t += passoMPS) {
            // Para restaurar o V original
            for (int i = 0; i < tam; i++) {
                V[i] = backup[i];
            }

            // Testar com o t atual
            // ordenador(V, tam, t, -1);

            quickSort3Ins(V, 0, tam-1, t); // r é inclusivo, entao é tam-1 --> TESTADO!

            registraEstatisticas(numMPS, t);
            cout << "mps " << t << " ";
            imprimeEstatisticas(numMPS);
            resetStats(); 
            numMPS++;
        }
        // Acha índice de menor custo e calcula novos valores
        limParticao = menorCusto(numMPS);
        calculaNovaFaixa(limParticao, minMPS_valor, maxMPS_valor, passoMPS, numMPS, mpsdiff);
        // diffCusto = custos[0].valorCusto - custos[numMPS-1].valorCusto;
        // diffCusto = diffCusto < 0 ? -diffCusto : diffCusto;
        index++;
        resetCustos();
        
        cout << endl << "nummps " << numMPS << " limParticao " << custos[limParticao].mps << " mpsdiff " << mpsdiff << endl; // ARRUMAR - oq é mpsdiff

        // cout << endl << diffCusto << " > " << limiarCusto << " && " << numMPS << " >= 5" << endl;
    } while ((mpsdiff > limiarCusto) && (numMPS >= 5));
    // return limParticao;

    delete[] backup;
    return custos[limParticao].mps;
}

void OrdenadorUniversal::calculaNovaFaixa(int limParticao, int &minMPS, int &maxMPS, int &passoMPS, int numMPS, long double &mpsdiff){
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
      // getMPS(index) virou custos[index].mps
    minMPS = custos[newMin].mps;
    maxMPS = custos[newMax].mps;

    mpsdiff = custos[newMax].valorCusto - custos[newMin].valorCusto;
    mpsdiff = mpsdiff >= 0 ? mpsdiff : -mpsdiff;

    passoMPS = (int)(maxMPS - minMPS) / 5;
    
    if (passoMPS == 0) {
        passoMPS++;
    }
}


int OrdenadorUniversal::menorCusto(int num) {
    int menorIdx = 0;
    for (int i = 1; i < num; i++) {
        if ( (custos[i].valorCusto >=0)  && (custos[i].valorCusto < custos[menorIdx].valorCusto) )
            menorIdx = i;
    }
    return menorIdx;
}

int OrdenadorUniversal::menorCustoQuebras(int num) {
    int menorIdx = 0;
    long double diffAtual = custos[1].valorCusto - custos[0].valorCusto;
    diffAtual = diffAtual < 0 ? -diffAtual : diffAtual; 


    for (int i = 2; i < num; i+=2) {
        long double diffAux = custos[i+1].valorCusto - custos[i].valorCusto;
        diffAux = diffAux < 0 ? -diffAux : diffAux;

        if(diffAux < diffAtual){
            diffAtual = diffAux;
            menorIdx = i;
        }

    }
    return menorIdx;
}

void OrdenadorUniversal::registraEstatisticas(int index, unsigned t){
    custos[index].mps = t;
    custos[index].calls = calls;
    custos[index].cmp = cmp;
    custos[index].move = move;
    custos[index].valorCusto = a*cmp + b*move + c*calls;
}

void OrdenadorUniversal::imprimeEstatisticas(int index){
    cout << "cost " << custos[index].valorCusto << " "
         << "cmp " << custos[index].cmp <<  " "
         << "move " << custos[index].move << " "
         << "calls " << custos[index].calls << endl;
}

int OrdenadorUniversal::numeroQuebras(tipo *V, int tam) {
    int quebras = 0;

    for (int i = 1; i < tam; i++) {
        // cmp++; // será?
        if (V[i] < V[i-1]) 
            quebras++;
    }
    return quebras;
}

// void OrdenadorUniversal::addQuebras(tipo *V, int quantidade){
//     quantidade++;

//     for (int i = 0; i < quantidade / 2; i++) {
//         int aux = V[i];
//         V[i] = V[quantidade - 1 - i];
//         V[quantidade - 1 - i] = aux;

//     }
// }

void OrdenadorUniversal::addQuebras(tipo *V, int tamanho, int quantidade){
    int p1 = 0, p2 = 0, temp;

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

void OrdenadorUniversal::resetCustos(){
    for (int i = 0; i < 6; i++)
        custos[i].valorCusto = -1;
}


void OrdenadorUniversal::swap(tipo &a, int &b){
    tipo temp = a;
    a=b;
    b=temp;
    move+=3;
    
}


int OrdenadorUniversal::median(tipo a, tipo b, tipo c){
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

// quicksort partition using median of 3
void OrdenadorUniversal::partition3(tipo * A, int l, int r, int *i, int *j){
    // using the median(start, middle, end) as the pivot, we avoid the worst scenarios.
    // maybe I can swap the pivot and the last element and just do the partition normally. --> NO, it is not what the want!!!!

    *i = l;
    *j = r;
  
  
    int meio = l + (r - l) / 2;
  
    
    tipo pivot_val = median(A[l], A[meio], A[r]);
  
    int pivot_index;
    if (pivot_val == A[l]) pivot_index = l;
    else if (pivot_val == A[meio]) pivot_index = meio;
    else pivot_index = r;
  
    tipo pivot = A[pivot_index];
  
    do {
        while (pivot > A[*i]) {
            cmp++;
            (*i)++;
        }
        while (pivot < A[*j]) {
            cmp++;
            (*j)--;
        }
       
        cmp += 2;
  
        if (*i <= *j) {
            swap(A[*i], A[*j]);
            (*i)++;
            (*j)--;
        }
    } while (*i <= *j);
}

// quicksort with insertion for small partitions and median of 3
void OrdenadorUniversal::quickSort3(tipo * A, int l, int r) { // ARRUMAR - acho q é sem insertio, visto q o insertion é em outra func
    if(l >= r)
    return;

    int i, j;

    partition3(A, l, r, &i, &j);

    calls++;
    quickSort3(A, l, j);

    calls++;
    quickSort3(A, i, r);
}

// quicksort with insertion for small partitions and median of 3
void OrdenadorUniversal::quickSort3Ins(tipo * A, int l, int r, int partition) { 
    calls+=2; // para partição e pro quickSort3Ins
    int i, j;
    partition3(A, l, r, &i, &j);

    if(j > l)
      if(j - l <= partition)
        insertionSort(A, l, j);  
      else
        quickSort3Ins(A, l, j, partition);
  
    if(r > i)
      if(r - i <= partition)
        insertionSort(A, i, r);  
      else
        quickSort3Ins(A, i, r, partition);
  }
  

void OrdenadorUniversal::insertionSort(tipo V[], int l, int r){
    // with an element i, we consider we have a i-1 ordered array. So we compare the element i
    // with the previous ones and swap it until it is not smaller than an element N. At this point we stop
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


