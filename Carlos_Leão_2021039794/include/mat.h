#ifndef MATRIZH
#define MATRIZH

#include "memlog.h"

typedef struct matriz{
    double **m;
    int tamx, tamy;
    int id;
} mat_tipo;

void criaMatriz(mat_tipo * mat, int tx, int ty, int id);
void criaMatrizTxt(mat_tipo * mat, char nomeArq[], int id);
void inicializaMatrizNula(mat_tipo * mat);
void inicializaMatrizAleatoria(mat_tipo * mat);
double acessaMatriz(mat_tipo * mat);
void imprimeMatriz(mat_tipo * mat);
void escreveArquivoTxt(mat_tipo * mat, char nomeArq[]);
void escreveElemento(mat_tipo * mat, int x, int y, double v);
double leElemento(mat_tipo * mat, int x, int y);
void copiaMatriz(mat_tipo * src, mat_tipo * dst, int dst_id);
void somaMatrizes(mat_tipo * a, mat_tipo * b, mat_tipo *c);
void multiplicaMatrizes(mat_tipo * a, mat_tipo * b, mat_tipo *c);
void transpoeMatriz(mat_tipo * a);
void destroiMatriz(mat_tipo *a);

#endif