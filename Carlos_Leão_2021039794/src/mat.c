#include <math.h>
#include "mat.h"
#include "memlog.h"
#include "msgassert.h"

// limite superior da inicializacao aleatoria
#define INITRANDOMRANGE 10
// Macro que realiza swap sem variavel auxiliar
#define ELEMSWAP(x,y) (x+=y,y=x-y,x-=y)

//cria matriz dinamicamente com dimensoes tx X ty
void criaMatriz(mat_tipo * mat, int tx, int ty, int id){
    //verifica se os valores de tx e ty são válidos
    erroAssert(tx>0, "Dimensao nula");
    erroAssert(ty>0, "Dimensao nula"); 
    int i;
    //inicializa as dimensões da matriz
    mat->tamx = tx;
    mat->tamy = ty;
    //inicializa o identificador da matriz, para rastreamento
    mat->id = id;
    //aloca a memóra para as linhas e colunas da matriz
    mat->m = (double **) malloc(mat->tamx * sizeof(double*));
    for(i=0; i<mat->tamx; i++){
        mat->m[i] = (double *)malloc(mat->tamy * sizeof(double));
    }
    erroAssert(mat->m != NULL, "Não foi possível alocar a matriz.");
    //outra forma de alocação
    //mat->m = (double*) malloc((mat->tamx * mat->tamy)*sizeof(double)); 
}

//inicializa mat com valores escritos em um arquivo txt
void criaMatrizTxt(mat_tipo * mat, char nomeArq[], int id){
    FILE *arq;
    int i=0, j=0;
    int tx=0, ty=0;
    //abertura do arquivo txt que contém a matriz
    arq = fopen(nomeArq, "r");
    erroAssert(arq != NULL, "Não foi possível acessar o arquivo.");
    //recebe as dimensões da matriz
    fscanf(arq, "%d %d", &tx, &ty);
    erroAssert(tx>0 && ty>0, "Arquivo com dimensoes incompativeis");
    //cria a matriz com as dimensoes escaneadas
    criaMatriz(mat, tx, ty, id);
    //preenche os elementos da matrizD
    for(i=0; i<mat->tamx; i++){
        for(j=0; j<mat->tamy; j++){
            fscanf(arq, "%lf", &mat->m[i][j]);
            ESCREVEMEMLOG((long int)(&(mat->m[i][j])), sizeof(double), mat->id);
        }
    }
    fclose(arq);
}

//inicializa mat com valores nulos
void inicializaMatrizNula(mat_tipo * mat){
    erroAssert(mat->m!=NULL, "Matriz não foi alocada corretamente");
    erroAssert(mat->tamx>0 && mat->tamy>0, "Matriz já foi destruída");
    int i, j;
    //inicializa todos os elementos da matriz com 0
    for(i=0; i<mat->tamx; i++){
        for(j=0; j<mat->tamy; j++){
            mat->m[i][j] = 0.0;
            ESCREVEMEMLOG((long int)(&(mat->m[i][j])), sizeof(double), mat->id);
        }
    }
}

//acessa mat para fins de registro de acesso
double acessaMatriz(mat_tipo * mat){
    erroAssert(mat->m!=NULL, "Matriz não foi alocada corretamente");
    erroAssert(mat->tamx>0 && mat->tamy>0, "Matriz já foi destruída");
    int i, j;
    double aux, s=0.0;
    for(i=0; i<mat->tamx; i++){
        for(j=0; j<mat->tamy; j++){
            aux = mat->m[i][j];
            s+=aux;
            LEMEMLOG((long int)(&(mat->m[i][j])), sizeof(double), mat->id);
        }
    }
    return s; // apenas para evitar que acesso seja eliminado
}

//escreve a matriz mat em um arquivo txt nomeArq
void escreveArquivoTxt(mat_tipo * mat, char nomeArq[]){
    FILE *arq;
    erroAssert(mat->m!=NULL, "Matriz não foi alocada corretamente");
    erroAssert(mat->tamx>0 && mat->tamy>0, "Matriz já foi destruída");
    int i=0, j=0;
    //abertura do arquivo txt que a matriz será escrita
    arq = fopen(nomeArq, "w");
    erroAssert(arq != NULL, "Não foi possível abrir o arquivo.");
    //escreve no txt os valores da matriz
    fprintf(arq, "%d %d \n", mat->tamx, mat->tamy);
    for(i=0; i<mat->tamx; i++){
        for(j=0; j<mat->tamy; j++){
            fprintf(arq, "%.2lf ", mat->m[i][j]);
            LEMEMLOG((long int)(&(mat->m[i][j])), sizeof(double), mat->id);
        }
        fprintf(arq, "\n");
    }
    fclose(arq);
}

//soma as matrizes a e b e armazena o resultado em c
void somaMatrizes(mat_tipo * a, mat_tipo * b, mat_tipo * c){
    erroAssert(a->m!=NULL, "Matriz A não foi alocada corretamente");
    erroAssert(b->m!=NULL, "Matriz B não foi alocada corretamente");
    erroAssert(a->tamx>0 && a->tamy>0, "Matriz A já foi destruída");
    erroAssert(b->tamx>0 && b->tamy>0, "Matriz B já foi destruída");
    int i=0, j=0;
    //veriffica se as dimensoes das matrizes a e b sao as mesmas
    erroAssert(a->tamx==b->tamx, "Dimensoes incompativeis.");
    erroAssert(a->tamy==b->tamy, "Dimensoes incompativeis.");
    //inicializa a matriz c garantindo a compatibilidade das dimensões
    criaMatriz(c, a->tamx, a->tamy, c->id);
    inicializaMatrizNula(c);
    //faz a soma elemento a elemento
    for(i=0; i<a->tamx; i++){
        for(j=0; j<a->tamy; j++){
            c->m[i][j] = a->m[i][j]+b->m[i][j];
            LEMEMLOG((long int)(&(a->m[i][j])),sizeof(double),a->id);
            LEMEMLOG((long int)(&(b->m[i][j])),sizeof(double),b->id);
            ESCREVEMEMLOG((long int)(&(c->m[i][j])),sizeof(double),c->id);
        }
    }
}

//multiplica as mattrizes a e bb e armazena o resultado em c
void multiplicaMatrizes(mat_tipo * a, mat_tipo * b, mat_tipo * c){
    erroAssert(a->m!=NULL, "Matriz A não foi alocada corretamente");
    erroAssert(b->m!=NULL, "Matriz B não foi alocada corretamente");
    erroAssert(a->tamx>0 && a->tamy>0, "Matriz A já foi destruída");
    erroAssert(b->tamx>0 && b->tamy>0, "Matriz B já foi destruída");
    int i=0, j=0, k=0;
    //verifica a compatibilidade das dimensoes
    erroAssert(a->tamy==b->tamx, "Dimensoes incompativeis.");
    //cria e inicializa a matriz c garantindo a compattibilidade das dimensões
    criaMatriz(c, a->tamx, b->tamy, c->id);
    inicializaMatrizNula(c);
    //realiza a multiplicação de matrizes
    for(i=0; i<c->tamx; i++){
        for(j=0; j<c->tamy; j++){
            for(k=0; k<a->tamy; k++){
                c->m[i][j] += a->m[i][k] * b->m[k][j];
                LEMEMLOG((long int)(&(a->m[i][k])),sizeof(double),a->id);
                LEMEMLOG((long int)(&(b->m[k][j])),sizeof(double),b->id);
                ESCREVEMEMLOG((long int)(&(c->m[i][j])),sizeof(double),c->id);
            }
        }
    }
}

//transpoe a matriz a
void transpoeMatriz(mat_tipo * a){
    erroAssert(a->m!=NULL, "Matriz não foi alocada corretamente");
    erroAssert(a->tamx>0 && a->tamy>0, "Matriz já foi destruída");
    int i=0, j=0;
    mat_tipo aux;
    criaMatriz(&aux, a->tamy, a->tamx, a->id);
    for (i=0; i<a->tamx; i++){
        for (j=0; j<a->tamy; j++){
            aux.m[j][i] = a->m[i][j];
            LEMEMLOG((long int)(&(a->m[i][j])),sizeof(double),a->id);
            ESCREVEMEMLOG((long int)(&(aux.m[j][i])),sizeof(double),aux.id);
        }
    }
    (*a) = aux;
}

//destroi a matriz a, que se torna inacessivel
void destroiMatriz(mat_tipo * a){
    //apenas um aviso se a matriz for destruida mais de uma vez
    avisoAssert((a->tamx>0)&&(a->tamy>0), "Matriz ja foi destruida.");
    for(int i=0; i<a->tamx; i++){
        free(a->m[i]);
    }
    free(a->m);
    //torna as dimensoes invalidas
    a->tamx = a->tamy = -1;
}