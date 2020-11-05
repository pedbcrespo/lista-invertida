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
    char arquivo[100];
} tup;

typedef struct indice{
    char palavra[60];
    int prim_pos_tup;
} idc;

tup*gera_tupla(idc*indice, char**arquivos, int tam_tup, int quant_arq);
tup*ler_arquivo_bin_tup(char*arquivo, int*tam);

lst inserir_lst(lst no, char*palavra);
lst remover(lst no, char*palavra);
lst destruir_lst(lst no);
lst ler_arquivo_texto(char*arquivo);
lst remove_stop_words(lst palavras, lst stop_words);
lst conjunto_listas(lst lista1, lst lista2);
lst tratamento_repeticao(lst lista);
lst remove_repeticao(lst lista);

idc*ler_arquivo_bin(char*arquivo, int*tam);
idc*gera_indice(lst lista);

int existe(lst lista, char*palavra);
int arquivo_bin_existe(char*arquivo);
int quant_caracter(char*arquivo);
int tamanho_lista_palavras(lst lista);
int cont_freq_arq(lst lista, char*palavra);
int cont_quant_arq(char*arquivo);
int compara (const void * a, const void * b);
int menu(char*texto);
int existe_idc(idc*vetor, int tam, char*palavra);

void criar_arq_bin(idc*vetor, int tam);
void criar_arq_bin_tup(tup*vetor, int tam, int quant_arq);
void imprimir_indice(idc*vetor, int tam);
void imprimir_tupla(tup*vetor, int tam);
void busca_eh_arq(char*arquivo_idc, char*arquivo_tup, char*palavra, char*palavra2, int qtd_arq);
void busca_ou_arq(char*arquivo_idc, char*arquivo_tup, char*palavra, char*palavra2, int qtd_arq);
void imprimir_lista(lst lista);
void imprimir_do_arquivo(char*arquivo_idc, char*arquivo_tup, int qtd_arq, char*palavra);

char*converte_minusculo(char*palavra);
char**vetor_arquivos(char*arquivo_tup, int quant_arq);