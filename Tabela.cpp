#include<iostream>
#include<cstring>
#include<string>
#include"Tabela.hpp"
using namespace std;

struct lista_palavras{
    char palavra[100];
    struct lista_palavras*prox;
};

struct tupla{
    unsigned int id; //posiçao na lista_palavras
    unsigned long int freq;
    char arquivo[100];
};

struct indice{
    char palavra[60];
    unsigned int prim_pos_tup;
};

//=======================CHAR FUNCOES===================//
char*minusculo(char palavra[]){
    int tam = strlen(palavra);
    int*min = (int*)malloc(tam*sizeof(int));
    char*result = (char*)malloc((tam+1)*sizeof(char));
    for(int i=0; i<tam; i++){
        min[i] = tolower(palavra[i]);
        result[i] = min[i];
        // result[i] = tolower(palavra[i]);
    }
    result[tam] = '\0';
    return result;
}

char*Tabela::lst_pos(lst lista, int pos){
    for(lst no=lista; no!=NULL; no=no->prox){
        if(pos == 0)
            return no->palavra;
        pos--;
    }
    return NULL;
}

//=======================LST FUNCOES===================//
lst Tabela::inserir_lst(lst no, char palavra[]){
    if(no == NULL){
        no = (struct lista_palavras*)malloc(sizeof(struct lista_palavras));
        strcpy(no->palavra, palavra);
        no->prox = NULL;
    }
    else{
        no->prox = inserir_lst(no->prox, palavra);
    }
    return no;
}

lst Tabela::destruir_lst(lst no){
    if(no != NULL){
        no->prox = destruir_lst(no->prox);
        free(no);
        no = NULL;
    }
    return no;
}

lst Tabela::tratamento_repeticao(lst lista){
    lst atualizada = NULL;
    for(lst aux=lista; aux!=NULL; aux=aux->prox){
        if(!existe(atualizada, minusculo(aux->palavra)))
            atualizada = inserir_lst(atualizada, minusculo(aux->palavra));
    }
    lista = destruir_lst(lista);
    return atualizada;
}

lst Tabela::ler_txt(char arquivo[]){
    FILE*arq = fopen(arquivo, "r");
    if(arq == NULL)
        return NULL;
    lst lista = NULL;
    int c, qtd = quant_caracter(arquivo);
    char*texto = (char*)malloc((qtd+1)*sizeof(char));
    char*pch;

    for(int i=0; i<qtd; i++){
        c = fgetc(arq);
        texto[i] = c;
    }
    texto[qtd] = '\0';
    pch = strtok(texto, " ,.;:-!?\n\t[]()/");
    while(pch != NULL){
        lista = inserir_lst(lista, minusculo(pch));
        pch = strtok(NULL, " ,.;:-!?\n\t[]()/");
    }
    fclose(arq);
    return lista;
}

lst Tabela::remove_stop_words(lst palavras, lst stop_words){
    lst lista = NULL;
    for(lst pl=palavras; pl!=NULL; pl=pl->prox){
        if(!existe(stop_words, pl->palavra))
            lista = inserir_lst(lista, pl->palavra);
    }
    palavras = destruir_lst(palavras);
    return lista;
}

lst Tabela::conjunto_listas(lst lista1, lst lista2){
    for(lst pl=lista2; pl!=NULL; pl=pl->prox){
        lista1 = inserir_lst(lista1, minusculo(pl->palavra));
    }
    return lista1;
}


//=======================IDC FUNCOES===================//
idc Tabela::gera_indice(lst lista){
    cout<<"gerando indice"<<endl;
    int tam = tamanho_lst(lista);
    int pos=0;
    idc vetor = (struct indice*)malloc(tam*sizeof(struct indice));
    for(lst pl=lista; pl!=NULL; pl=pl->prox){
        strcpy(vetor[pos].palavra, minusculo(pl->palavra));
        vetor[pos].prim_pos_tup = 0;
        pos++;
    }
    cout<<"indice gerado com sucesso"<<endl;
    return vetor;
}

//=======================TUP FUNCOES====================//
tup Tabela::gera_tupla(idc indice, lst arquivos, int tam_tup, int quant_arq){
    cout<<"gerando tupla"<<endl;
    int prim_pos, pos=0;
    tup aux = (struct tupla*)malloc(tam_tup*sizeof(struct tupla));

    for(int i=0; i<tam_tup; i+=quant_arq){
        aux[i].id = pos;
        system("cls");
        cout<<"processo "<<(i*100)/(tam_tup)<<"% "<<"completo"<<endl;
        for(int n=0; n<quant_arq; n++){
            aux[i+n].id = pos;
            strcpy(aux[i+n].arquivo, this->lst_pos(arquivos, n));
            aux[i+n].freq = frequencia(ler_txt(this->lst_pos(arquivos, n)), indice[pos].palavra);
        }
        indice[pos].prim_pos_tup = i;
        pos++;
    }
    cout<<"tupla gerada com sucesso"<<endl;
    return aux;
}

//=======================INT FUNCOES====================//
int Tabela::existe(lst lista, char palavra[]){
    for(lst no=lista; no!=NULL; no=no->prox){
        if(strcmp(no->palavra, palavra)==0)
            return 1;
    }
    return 0;
}

int Tabela::quant_caracter(char arquivo[]){
    FILE*arq = fopen(arquivo, "r");
    int c, quant=0;
    while((c=fgetc(arq))!=EOF){
        quant++;
    }
    fclose(arq);
    return quant;
}

int Tabela::tamanho_lst(lst lista){
    int tam=0;
    for(lst no=lista; no!=NULL; no=no->prox){
        tam++;
    }
    return tam;
}

int Tabela::frequencia(lst lista, char palavra[]){
    int freq = 0;
    for(lst no=lista; no!=NULL; no=no->prox){
        if(strcmp(no->palavra, minusculo(palavra))==0)
            freq++;
    }
    return freq;
}

//=======================VOID FUNCOES===================//
void imprimir_indice(idc indice, int tam){
    for(int i=0; i<tam; i++){
        cout<<indice[i].palavra<<": "<<indice[i].prim_pos_tup<<endl;
    }
}

void imprimir_tupla(tup tupla, int tam){
    for(int i=0; i<tam; i++){
        cout<<tupla[i].id<<":"<<tupla[i].arquivo<<": "<<tupla[i].freq<<endl;
    }
}

void imprimir_lista(lst lista){
    for(lst pl=lista; pl!=NULL; pl=pl->prox){
        cout<<pl->palavra<<endl;
    }
}

void Tabela::salvar_tup(tup tupla, int tam){
    cout<<"salvando tupla"<<endl;
    FILE*arq = fopen(this->arq_tup, "wb");
    for(int i=0; i<tam; i++){
        fwrite(&tupla[i], sizeof(struct tupla), 1, arq);
    }
    fclose(arq);
    cout<<"salvo com sucesso"<<endl;
}

void Tabela::salvar_idc(idc indice, int tam){
    cout<<"salvando indice"<<endl;
    FILE*arq = fopen(this->arq_idc, "wb");
    for(int i=0; i<tam; i++){
        fwrite(&indice[i], sizeof(struct indice), 1, arq);
    }
    fclose(arq);
    cout<<"salvo com sucesso"<<endl;
}

void Tabela::adiciona_arquivo(char arquivo[]){
    this->lista_arquivos = inserir_lst(this->lista_arquivos, arquivo);
}

void Tabela::gerar(){
    lst palavras = NULL;
    idc indice = NULL;
    tup tupla = NULL;
    int tam_idc, tam_tup;
    FILE*arq = fopen(this->arq_idc, "rb");
    if(arq == NULL){
        for(lst elem=this->lista_arquivos; elem!=NULL; elem=elem->prox){
            palavras = conjunto_listas(palavras, ler_txt(elem->palavra));
        }
    
        palavras = tratamento_repeticao(palavras);
        tam_idc = tamanho_lst(palavras);
        tam_tup = tam_idc*tamanho_lst(this->lista_arquivos);
        indice = gera_indice(palavras);
        tupla = gera_tupla(indice, this->lista_arquivos, tam_tup, tamanho_lst(this->lista_arquivos));
        
        this->salvar_idc(indice, tam_idc);
        this->salvar_tup(tupla, tam_tup);

        free(indice);
        free(tupla);
    }
    fclose(arq);
}

void Tabela::atualizar_tabela(){
    remove(this->arq_idc);
    this->gerar();
}

void Tabela::insere_stop_words(char stop_words[]){
    this->stop_words = stop_words;
}

void Tabela::imprimir_lista_arquivos(){
    for(lst arq=this->lista_arquivos; arq!=NULL; arq=arq->prox){
        cout<<arq->palavra<<endl;
    }
}

void Tabela::busca_simples(char palavra[]){
    idc aux_idc = (struct indice*)malloc(sizeof(struct indice));
    int flag = 1, pos=0, i=0;
    FILE*arq_indice = fopen(this->arq_idc,"rb");
    fread(aux_idc, sizeof(struct indice), 1, arq_indice);
    while(strcmp(aux_idc->palavra, palavra) != 0){
        if(fread(aux_idc, sizeof(struct indice), 1, arq_indice) == 0){
            flag = 0;
            break;
        }
        pos++;
    }
    fclose(arq_indice);
    if(flag){
        FILE*arq_registro = fopen(this->arq_tup, "rb");
        tup aux_registro = (struct tupla*)malloc(sizeof(struct tupla));
        fseek(arq_registro, aux_idc->prim_pos_tup*sizeof(struct tupla), SEEK_SET);
        fread(aux_registro, sizeof(struct tupla), 1, arq_registro);
        while(aux_registro->id == pos){
            if(aux_registro->freq > 0)
                cout<<aux_registro->arquivo<<": "<<aux_registro->freq<<endl;
            fread(aux_registro, sizeof(struct tupla), 1, arq_registro);
        }
        free(aux_registro);
        fclose(arq_registro);
    }
    else{
        cout<<"palavra inexistente"<<endl;
    }
    free(aux_idc);
}

void Tabela::busca_eh(char palavra1[], char palavra2[]){
    idc pl1 = (struct indice*)malloc(sizeof(struct indice)); 
    idc pl2 = (struct indice*)malloc(sizeof(struct indice));
    int pos_pl1 = 0, pos_pl2 = 0, flag1 = 0, flag2 = 0;
    FILE*arq_indice = fopen(this->arq_idc, "rb");

    while(fread(pl1, sizeof(struct indice), 1, arq_indice) != 0){

        if(strcmp(pl1->palavra, palavra1) == 0){
            flag1 = 1;
            break;
        }
        else   
            pos_pl1++;
    }
    fseek(arq_indice, 0, SEEK_SET);

    while(fread(pl2, sizeof(struct indice), 1, arq_indice) != 0){

        if(strcmp(pl2->palavra, palavra2) == 0){
            flag2 = 1;
            break;
        }
        else   
            pos_pl2++;
    }

    //Esta encontrando as palavras

    fclose(arq_indice);
    if(flag1 && flag2){
        FILE*arq_tupla = fopen(this->arq_tup, "rb");
        FILE*aux = fopen(this->arq_tup, "rb");
        tup reg1 = (struct tupla*)malloc(sizeof(struct tupla));
        tup reg2 = (struct tupla*)malloc(sizeof(struct tupla));

        fseek(arq_tupla, pl1->prim_pos_tup*sizeof(struct tupla), SEEK_SET);
        fseek(aux, pl2->prim_pos_tup*sizeof(struct tupla), SEEK_SET);

        fread(reg1, sizeof(struct tupla), 1, arq_tupla);
        fread(reg2, sizeof(struct tupla), 1, aux);

        while(reg1->id == pos_pl1 && reg2->id == pos_pl2){
            if(reg1->freq > 0 && reg2->freq > 0){
                cout<<reg1->arquivo<<"|| termo 1: "<<reg1->freq<<" termo 2: "<<reg2->freq<<endl;
            }

            fread(reg1, sizeof(struct tupla), 1, arq_tupla);
            fread(reg2, sizeof(struct tupla), 1, aux);
        }
        free(pl1);
        free(pl2);

        fclose(arq_tupla);
        fclose(aux);
    }
    else{
        cout<<"nao encontrado"<<endl;
        cout<<"um dos termos nao existe na lista"<<endl;
    }
}

void Tabela::busca_ou(char palavra1[], char palavra2[]){
    idc pl1 = (struct indice*)malloc(sizeof(struct indice)); 
    idc pl2 = (struct indice*)malloc(sizeof(struct indice));
    int pos_pl1 = 0, pos_pl2 = 0, flag1 = 0, flag2 = 0;
    FILE*arq_indice = fopen(this->arq_idc, "rb");

    while(fread(pl1, sizeof(struct indice), 1, arq_indice) != 0){

        if(strcmp(pl1->palavra, palavra1) == 0){
            flag1 = 1;
            break;
        }
        else   
            pos_pl1++;
    }
    fseek(arq_indice, 0, SEEK_SET);

    while(fread(pl2, sizeof(struct indice), 1, arq_indice) != 0){

        if(strcmp(pl2->palavra, palavra2) == 0){
            flag2 = 1;
            break;
        }
        else   
            pos_pl2++;
    }

    //Esta encontrando as palavras

    fclose(arq_indice);
    if(flag1 && flag2){
        FILE*arq_tupla = fopen(this->arq_tup, "rb");
        FILE*aux = fopen(this->arq_tup, "rb");
        tup reg1 = (struct tupla*)malloc(sizeof(struct tupla));
        tup reg2 = (struct tupla*)malloc(sizeof(struct tupla));

        fseek(arq_tupla, pl1->prim_pos_tup*sizeof(struct tupla), SEEK_SET);
        fseek(aux, pl2->prim_pos_tup*sizeof(struct tupla), SEEK_SET);

        fread(reg1, sizeof(struct tupla), 1, arq_tupla);
        fread(reg2, sizeof(struct tupla), 1, aux);

        while(reg1->id == pos_pl1 && reg2->id == pos_pl2){
            if(reg1->freq > 0 || reg2->freq > 0){
                cout<<reg1->arquivo<<"|| termo 1: "<<reg1->freq<<" termo 2: "<<reg2->freq<<endl;
            }

            fread(reg1, sizeof(struct tupla), 1, arq_tupla);
            fread(reg2, sizeof(struct tupla), 1, aux);
        }
        free(pl1);
        free(pl2);

        fclose(arq_tupla);
        fclose(aux);
    }
    else{
        cout<<"nao encontrado"<<endl;
        cout<<"um dos termos nao existe na lista"<<endl;
    }
}