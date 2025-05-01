#include "./../include/OrdenadorUniversal.hpp"

#include <iostream>

using namespace std;

OrdenadorUniversal::OrdenadorUniversal(int tam){
    resetStats();
    custos = new Estatisticas[6];
}

OrdenadorUniversal::~OrdenadorUniversal(){}


void OrdenadorUniversal::ordenador(int *V, int tam, int minTamParticao, int limiarQuebras) {
    int numQuebras = numeroQuebras(V, tam);

    if (numQuebras < limiarQuebras) 
        insertionSort(V, 0, tam-1);   
    else if (tam > minTamParticao) 
        quickSort3Ins(V, 0, tam-1) ;
    else 
        insertionSort(V, 0, tam-1);
}

int OrdenadorUniversal::determinaLimiarQuebras(int *V, int tam, int limiarCusto){
    int limiarQuebras,
        minQuebras = 0, 
        maxQuebras = tam,
        numQuebras,
        index = 0,
        passoQuebra = (maxQuebras-minQuebras)/5;
    
    long double diffCusto = 0;
    
    do {
        numQuebras = 0;
        cout << "iter " << index << endl;

        for (int lq = minQuebras; lq <= maxQuebras; lq += passoQuebra) {
            // QuickSort
            ordenador(V, tam, 0, lq); // 'tam' como minTamParticao para forçar a escolha baseada no limiar de quebras
            registraEstatisticas(numQuebras, lq); // Registra as estatísticas para este limiar de quebras
            cout << "qs lq " << lq << " ";
            imprimeEstatisticas(numQuebras);
            resetStats();

            // InsertionSort
            ordenador(V, tam, tam+1, lq); // 'tam' como minTamParticao para forçar a escolha baseada no limiar de quebras
            registraEstatisticas(numQuebras, lq); // Registra as estatísticas para este limiar de quebras
            cout << "in lq " << lq << " ";
            imprimeEstatisticas(numQuebras);
            resetStats();


            numQuebras++;
        }

        limiarQuebras = menorCusto(tam); // Encontra o limiar de quebras com menor custo registrado
        calculaNovaFaixaQuebras(limiarQuebras, minQuebras, maxQuebras, passoQuebra, numQuebras);
        diffCusto = (custos[minQuebras].valorCusto - custos[maxQuebras].valorCusto) < 0 ?
                   -(custos[minQuebras].valorCusto - custos[maxQuebras].valorCusto) :
                    (custos[minQuebras].valorCusto - custos[maxQuebras].valorCusto);
        index++;
        resetCustos();
        cout << endl;
    } while ((diffCusto > limiarCusto) && (numQuebras >= 5));

    cout << "numq " << index - 1 << " limQuebras " << limiarQuebras << " qdiff " ; // ARRUMAR - o que é qdiff
    return limiarQuebras;
}



int OrdenadorUniversal::determinaLimiarParticao(int *V, int tam, int limiarCusto) {
    int minMPS = 2, 
        numMPS, 
        maxMPS = tam,
        passoMPS = (maxMPS - minMPS) / 5,
        limParticao,
        index = 0;
    long double diffCusto = 0;
        
    do {
        numMPS = 0;
        cout << "iter " << index << endl;
        
        for (int t = minMPS; t <= maxMPS; t += passoMPS) {
            ordenador(V, tam, t, tam);
            registraEstatisticas(numMPS, t);
            cout << "mps " << t << " ";
            imprimeEstatisticas(numMPS);
            resetStats(); 
            numMPS++;
        }
        
        limParticao = menorCusto(tam);
        calculaNovaFaixa(limParticao, minMPS, maxMPS, passoMPS, numMPS);
        diffCusto = (custos[minMPS].valorCusto - custos[maxMPS].valorCusto) < 0 ?
                   -(custos[minMPS].valorCusto - custos[maxMPS].valorCusto) : 
                    (custos[minMPS].valorCusto - custos[maxMPS].valorCusto);
        index++;
        resetCustos();
        cout << endl;
    } while ((diffCusto > limiarCusto) && (numMPS >= 5));
    cout << "nummps " << index-1 << " limParticao " << limParticao << " mpsdiff " ; // ARRUMAR - oq é mpsdiff
    return limParticao;
}

void OrdenadorUniversal::calculaNovaFaixa(int limParticao, int minMPS, int maxMPS, int passoMPS, int numMPS){
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

    passoMPS = (int)(maxMPS - minMPS) / 5;
    
    if (passoMPS == 0) {
        passoMPS++;
    }
}


int OrdenadorUniversal::menorCusto(int tam) {
    int menorIdx = 0;
    for (int i = 1; i < tam; i++) {
        if (custos[i].valorCusto < custos[menorIdx].valorCusto)
            menorIdx = i;
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

int OrdenadorUniversal::numeroQuebras(int *V, int tam) {
    int quebras = 0;

    for (int i = 1; i < tam; i++) {
        cmp++; // será?
        if (V[i] < V[i-1]) 
            quebras++;
    }
    return quebras;
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


void OrdenadorUniversal::swap(int &a, int &b){
    int temp = a;
    a=b;
    b=temp;
    move+=3;
    
}


int OrdenadorUniversal::median(int a, int b, int c){
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
void OrdenadorUniversal::partition3(int * A, int l, int r, int *i, int *j){
    // using the median(start, middle, end) as the pivot, we avoid the worst scenarios.
    // maybe I can swap the pivot and the last element and just do the partition normally. --> NO, it is not what the want!!!!
    
    *i = l;
    *j = r;
  
  
    int meio = l + (r - l) / 2;
  
    
    int pivot_val = median(A[l], A[meio], A[r]);
  
    int pivot_index;
    if (pivot_val == A[l]) pivot_index = l;
    else if (pivot_val == A[meio]) pivot_index = meio;
    else pivot_index = r;
  
    int pivot = A[pivot_index];
  
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
void OrdenadorUniversal::quickSort3Ins(int * A, int l, int r) { // ARRUMAR - acho q é sem insertio, visto q o insertion é em outra func
    calls += 2;

    int i, j;
    partition3(A, l, r, &i, &j);
    
    if(j > l)
      if(j - l <= 50)
        insertionSort(A, l, j);  
      else
        quickSort3Ins(A, l, j);
  
    if(r > i)
      if(r - i <= 50)
        insertionSort(A, i, r);  
      else
        quickSort3Ins(A, i, r);
}
  

void OrdenadorUniversal::insertionSort(int V[], int l, int r){
    // with an element i, we consider we have a i-1 ordered array. So we compare the element i
    // with the previous ones and swap it until it is not smaller than an element N. At this point we stop

    calls++;
    int i, j, key;

    for (i = l + 1; i <= r; i++){
        key = V[i];
        move++;
        j = i - 1;
        int did_break = 0;

        while (j >= l){
            cmp++;
            if (key >= V[j])
            {
                did_break = 1;
                break;
            }

            V[j + 1] = V[j];
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


