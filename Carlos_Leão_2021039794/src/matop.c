#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <getopt.h>
#include <string.h>
#include "mat.h"
#include "memlog.h" 
#include "msgassert.h" 

//definicoes de operacoes a serem testadas
#define OPSOMA 1
#define OPMULTIPLICA 2
#define OPTRANSPOR 3

typedef struct opt{
    int opescolhida;
    char lognome[100];
    char arq1[100];
    char arq2[100];
    char arq3[100];
    int tam, regmem;
} opt_tipo;

//imprime as opcoes de uso
void uso(){
    fprintf(stderr,"matop\n");
    fprintf(stderr,"\t-s \t\t(soma matrizes) \n");
    fprintf(stderr,"\t-m \t\t(multiplica matrizes) \n");
    fprintf(stderr,"\t-t \t\t(transpoe matriz 1)\n");
    fprintf(stderr,"\t-1 m1.txt\t(matriz 1)\n");
    fprintf(stderr,"\t-2 m2.txt\t(matriz 2)\n");
    fprintf(stderr,"\t-o res.out\t(matriz resultante)\n");
    fprintf(stderr,"\t-p log.out\t(registro de desempenho)\n");
    fprintf(stderr,"\t-l \t\t(padrao de acesso e localidade)\n");
}

//le as opcoes da linha de comando e inicializa variaveis
void parse_args(int argc,char ** argv, opt_tipo * opt){
    // variaveis externas do getopt
    extern char * optarg;
    extern int optind;
    // variavel auxiliar
    int c;

    // inicializacao variaveis globais para opcoes
    opt->opescolhida = -1;
    opt->tam = -1;
    opt->regmem = 0;
    opt->lognome[0] = 0;
    opt->arq1[0] = 0;
    opt->arq2[0] = 0;
    opt->arq3[0] = 0;
    
    // getopt - letra indica a opcao, : junto a letra indica parametro
    // no caso de escolher mais de uma operacao, vale a ultima
    while ((c = getopt(argc, argv, "mst1:2:o:p:lh")) != EOF)
    switch(c) {
        case 'm':
            avisoAssert(opt->opescolhida==-1, "Mais de uma operacao escolhida");
            opt->opescolhida = OPMULTIPLICA;
            break;
        case 's':
            avisoAssert(opt->opescolhida==-1, "Mais de uma operacao escolhida");
            opt->opescolhida = OPSOMA;
            break;
        case 't':
            avisoAssert(opt->opescolhida==-1, "Mais de uma operacao escolhida");
            opt->opescolhida = OPTRANSPOR;
            break;
        case '1':
            strcpy(opt->arq1, optarg);
            break;
        case '2':
            strcpy(opt->arq2, optarg);
            break;
        case 'o':
            strcpy(opt->arq3, optarg);
            break;
        case 'p': 
            strcpy(opt->lognome, optarg);
            break;
        case 'd': 
            opt->tam = atoi(optarg);
            break;
        case 'l': 
            opt->regmem = 1;
            break;
        case 'h':
        default:
            uso();
            exit(1);
    }
    // verificacao da consistencia das opcoes
    erroAssert(opt->opescolhida>0,"matop - necessario escolher operacao");
    erroAssert(strlen(opt->lognome)>0,
        "matop - nome de arquivo de registro de acesso tem que ser definido");
    //erroAssert(opt->tam>0, "matop - tamanho da matriz tem que ser positivo");
}

//programa principal para execucao de operacoes de matrizes 
int main(int argc, char ** argv){
    // ate 3 matrizes sao utilizadas, dependendo da operacao
    mat_tipo a, b, c;
    opt_tipo opt;
    // avaliar linha de comando
    parse_args(argc,argv,&opt);

    // iniciar registro de acesso
    iniciaMemLog(opt.lognome);

    // ativar ou nao o registro de acesso
    if (opt.regmem){ 
        ativaMemLog();
    }
    else{
        desativaMemLog();
    }

    // execucao dependente da operacao escolhida
    switch (opt.opescolhida){
        case OPSOMA:
        // le dois arquivos txt para a formacao das matrizes a e b
        // as matrizes a e b sao somadas e formam a matriz c
        // matriz c é escrita em um arquivo txt e todos as matrizes sao destruidas
        defineFaseMemLog(0);
            criaMatrizTxt(&a, opt.arq1, 0);
            criaMatrizTxt(&b, opt.arq2, 1);
            criaMatriz(&c, a.tamx, a.tamy, 2);
            inicializaMatrizNula(&c);
        defineFaseMemLog(1);
            acessaMatriz(&a);
            acessaMatriz(&b);
            acessaMatriz(&c);
            somaMatrizes(&a,&b,&c);
        defineFaseMemLog(2);
            acessaMatriz(&c);
            escreveArquivoTxt(&c, opt.arq3);
            destroiMatriz(&a);
            destroiMatriz(&b);
            destroiMatriz(&c);
        break;

        case OPMULTIPLICA:
        // le dois arquivos txt para a formacao das matrizes a e b
        // as matrizes a e b sao multiplicadas e formam a matriz c
        // matriz c é impressa e todas as matrizes sao destruidas
        defineFaseMemLog(0);
            criaMatrizTxt(&a, opt.arq1, 0);
            criaMatrizTxt(&b, opt.arq2, 1);
            criaMatriz(&c, a.tamx, b.tamy, 2);
            inicializaMatrizNula(&c);
        defineFaseMemLog(1);
            acessaMatriz(&a);
            acessaMatriz(&b);
            acessaMatriz(&c);
            multiplicaMatrizes(&a,&b,&c);
        defineFaseMemLog(2);
            acessaMatriz(&c);
            escreveArquivoTxt(&c, opt.arq3);
            destroiMatriz(&a);
            destroiMatriz(&b);
            destroiMatriz(&c);
        break;
        case OPTRANSPOR:
        // le um arquivo txt para a formacao da matriz a
        // a matriz a eh transposta, impressa e destruida
        defineFaseMemLog(0);
            criaMatrizTxt(&a, opt.arq1, 0);
        defineFaseMemLog(1);
            acessaMatriz(&a);
            transpoeMatriz(&a);
        //if (opt.regmem) escreveArquivoTxt(&a, "resTrans.txt");
        defineFaseMemLog(2);
            acessaMatriz(&a);
            escreveArquivoTxt(&a, opt.arq3);
            destroiMatriz(&a);
        break;
    default:
            // nao deve ser executado, pois ha um erroAssert em parse_args
            uso();
        exit(1);
    }

    // conclui registro de acesso
    return finalizaMemLog();
}

