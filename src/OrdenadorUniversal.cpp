#include "./../include/OrdenadorUniversal.hpp"

OrdenadorUniversal::OrdenadorUniversal(){
    resetStats();
}

OrdenadorUniversal::~OrdenadorUniversal(){}


void OrdenadorUniversal::ordenador(int *V, int tam, int minTamParticao, int limiarQuebras, int numeroQuebras) {
    
    if (numeroQuebras < limiarQuebras) 
        insertionSort(V, 0, tam);   
    else if (tam > minTamParticao) 
            quickSort3Ins(V, 0, tam) ;
    else 
        insertionSort(V, 0, tam);
}


int OrdenadorUniversal::determinaLimiarParticao(int *V, int tam, int limiarCusto) {
    int minMPS = 2, numMPS, maxMPS = tam;
    int passoMPS = (maxMPS - minMPS) / 5;
    int diffCusto = 0;

    do {
        numMPS = 0;
        
        for (int t = minMPS; t <= maxMPS; t += passoMPS) {
            ordenador(V, tam, t, tam);
            registraEstatisticas(custo[numMPS]);
            imprimeEstatisticas(custo[numMPS]);
            numMPS++;
        }
        
        limParticao = menorCusto();
        calculaNovaFaixa(limParticao, minMPS, maxMPS, passoMPS);
        diffCusto = fabs(custo[minMPS] - custo[maxMPS]);
    } while ((diffCusto > limiarCusto) && (numMPS >= 5)) 
    
    return limParticao;
}








void OrdenadorUniversal::resetStats(){
    cmp = 0;
    move = 0;
    calls = 0;
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
void OrdenadorUniversal::quickSort3Ins(int * A, int l, int r) { 
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


