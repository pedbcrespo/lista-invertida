#include<string>
using namespace std;

typedef struct lista_palavras*lst;

typedef struct tupla*tup;

typedef struct indice*idc;

class Tabela{
private:
    char const*arq_idc = ".\\indice.bin";
    char const*arq_tup = ".\\registros.bin";
    const char*stop_words;
    lst lista_arquivos = NULL;

    lst inserir_lst(lst no, char palavra[]);
    lst destruir_lst(lst no);
    lst tratamento_repeticao(lst lista);
    lst ler_txt(char arquivo[]);
    lst remove_stop_words(lst palavras, lst stop_words);
    lst conjunto_listas(lst lista1, lst lista2);

    idc gera_indice(lst lista);

    tup gera_tupla(idc indice, lst arquivos, int tam_tup, int quant_arq);

    int existe(lst lista, char palavra[]);
    int quant_caracter(char arquivo[]);
    int tamanho_lst(lst lista);
    int frequencia(lst lista, char palavra[]);

    char*lst_pos(lst lista, int pos);

    void salvar_idc(idc indice, int tam);
    void salvar_tup(tup tupla, int tam);


public:

    void insere_stop_words(char stop_words[]);
    void adiciona_arquivo(char arquivo[]);
    void gerar();
    void atualizar_tabela();
    void busca_simples(char palavra[]);
    void busca_eh(char palavra1[], char palavra2[]);
    void busca_ou(char palavra1[], char palavra2[]);
    void imprimir_lista_arquivos();
}; 