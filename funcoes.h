#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

typedef struct lista_palavras{
    char*palavra;
    struct lista_palavras*prox;
}*lst;

typedef struct tupla{
    int id; //posiçao na lista_palavras
    int freq;
    char arquivo[20];
} tup;

typedef struct indice{
    char palavra[50];
    int prim_pos_tup;
} idc;

tup*gera_tupla(tup*vetor, idc*indice, int tam_idc, char**arquivos, int *tam_tup, int quant_arq);
tup*ler_arquivo_bin_tup(char*arquivo, int*tam, int*quant_arq);

lst inserir_lst(lst no, char*palavra);
lst conjunto_listas(lst lista1, lst lista2);
lst destruir_lst(lst no);
lst remover_lst(lst no, char*elem);
lst tratamento_repeticao(lst lista);
lst ler_arquivo_texto(char*arquivo);
lst mantem_mais_repetidas(lst lista, char**arquivos, int qtd_arq);

idc*inserir_idc(idc*vetor, int*tam, char*palavra);
idc*ler_arquivo_bin(char*arquivo, int*tam);
idc*gera_indice(lst lista);

int quant_caracter(char*arquivo);
int existe(lst lista, char*elem);
int existe_idc(idc*vetor, int tam, char*elem);
int tamanho_lista_palavras(lst lista);
int cont_freq_arq(lst lista, char*palavra);
int cont_repet_lst(lst lista, char*palavra);
int busca_pos(idc*vetor, int tam, char*palavra);
int compara (const void * a, const void * b);

void criar_arq_bin(idc*vetor, int tam);
void criar_arq_bin_tup(tup*vetor, int tam, int quant_arq);
void imprimir_indice(idc*vetor, int tam);
void imprimir_termo_buscado(idc*vetor, int tam, tup*vetor_tup, char*palavra);
void imprimir_termo_buscado_alt(idc*vetor, int tam, tup*vetor_tup, int quant_arq, char*palavra);
void imprimir_tupla(tup*vetor, int tam);
void busca_eh(idc*vetor, int tam, tup*vetor_tup, int quant_arq, char*palavra1, char*palavra2);
void busca_ou(idc*vetor, int tam, tup*vetor_tup, int quant_arq, char*palavra1, char*palavra2);
void imprimir_lista(lst lista);
void imprimir_ordenado(tup*vetor, int tam, char*palavra);

char*converte_minusculo(char*palavra);