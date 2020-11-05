#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include"funcoes.h"

//=======================TUP FUNCOES====================//
tup*gera_tupla(idc*indice, char**arquivos, int tam_tup, int quant_arq){
    printf("gerando tupla\n");
    int prim_pos, pos=0;
    tup*aux = (tup*)malloc(tam_tup*sizeof(tup));
    
    for(int i=0; i<tam_tup; i+=quant_arq){
        aux[i].id = pos;
        system("cls");
        printf("lendo arquivos\n%d %%\n", (i*100)/(tam_tup));
        for(int n=0; n<quant_arq; n++){
            aux[i+n].id = pos;
            strcpy(aux[i+n].arquivo, arquivos[n]);
            aux[i+n].freq = cont_freq_arq(ler_arquivo_texto(arquivos[n]), indice[pos].palavra);
        }
        indice[pos].prim_pos_tup = i;
        pos++;
    }
    printf("tupla gerada com sucesso\n");
    return aux;
}
tup*ler_arquivo_bin_tup(char*arquivo, int*tam){
    printf("lendo %s\n", arquivo);
    FILE*arq=fopen(arquivo, "rb");
    if(arq == NULL){
        printf("arquivo inexistente\n");
        return NULL;
    }
    fseek(arq, 0, SEEK_END);
    *tam = ftell(arq)/sizeof(tup);
    fseek(arq, 0, SEEK_SET);

    tup*vetor = (tup*)malloc(*tam*sizeof(tup));
    int pos=0;
    while(fread(&vetor[pos], sizeof(tup), 1, arq)>0){
        pos++;
    }
    fclose(arq);
    printf("arquivo bin lido com sucesso\n");
    return vetor;
}

//=======================LST FUNCOES====================//
lst inserir_lst(lst no, char*palavra){
    if(no==NULL){
        no = (struct lista_palavras*)malloc(sizeof(struct lista_palavras));
        no->palavra = palavra;
        no->prox = NULL;
    }else{
        no->prox = inserir_lst(no->prox, palavra);
    } 
    return no;
}
lst remover(lst no, char*palavra){
    if(no != NULL){
        if(strcmp(no->palavra, palavra)){
            lst aux = no->prox;
            free(no);
            return aux;
        }
    }
    return no;
}
lst destruir_lst(lst no){
	if(no != NULL){
		no->prox = destruir_lst(no->prox);
		free(no);
		no = NULL;
	}
	return no;
}
lst tratamento_repeticao(lst lista){
    lst lista_atualizada = NULL;
    for(lst aux = lista; aux != NULL; aux = aux->prox){
        if(!existe(lista_atualizada, converte_minusculo(aux->palavra))){
            lista_atualizada = inserir_lst(lista_atualizada, converte_minusculo(aux->palavra));
        }
    }
    lista = destruir_lst(lista);
    return lista_atualizada;
}
lst ler_arquivo_texto(char*arquivo){
    // printf("lendo arquivo %s\n", arquivo);
    FILE*arq = fopen(arquivo,"r");
    if(arq == NULL){
        return NULL;
    }
    lst lista = NULL;
    if(arq != NULL){
        int c, t = quant_caracter(arquivo);
        char *texto = (char*)malloc(t+1*sizeof(char));
        char *pch;

        for(int i=0; i<t; i++){
            c = fgetc(arq);
            texto[i] = c;
        }
        texto[t] = '\0';
        pch = strtok(texto, " ,.;:-!?\n\t[]()/");
        while(pch != NULL){
            lista = inserir_lst(lista, converte_minusculo(pch));
            pch = strtok(NULL, " ,.;:-!?\n\t[]()/");
        }

        fclose(arq);
    }
    // printf("arquivo lido com sucesso\n");
    return lista;
}
lst remove_stop_words(lst palavras, lst stop_words){
    lst lista = NULL;
    for(lst pl = palavras; pl != NULL; pl=pl->prox){
        if(!existe(stop_words, pl->palavra))
            lista = inserir_lst(lista, pl->palavra);
    }
    palavras = destruir_lst(palavras);
    return lista;
}
lst conjunto_listas(lst lista1, lst lista2){
    for(lst pl=lista2; pl!=NULL; pl=pl->prox){
            lista1 = inserir_lst(lista1, converte_minusculo(pl->palavra));
    }
    return lista1;
}

//=======================IDC FUNCOES====================//
idc*ler_arquivo_bin(char*arquivo, int*tam){
    printf("lendo %s\n", arquivo);
    FILE*arq=fopen(arquivo, "rb");
    if(arq == NULL){
        printf("arquivo inexistente\n");
        return NULL;
    }
    fseek(arq, 0, SEEK_END);
    *tam = ftell(arq)/sizeof(idc);
    fseek(arq, 0, SEEK_SET);
    
    idc*vetor = (idc*)malloc(*tam*sizeof(idc));
    int pos=0;
    while(fread(&vetor[pos], sizeof(idc), 1, arq)>0){
        pos++;
    }
    fclose(arq);
    printf("arquivo bin lido com sucesso\n");
    return vetor;
}
idc*gera_indice(lst lista){
    printf("gerando indice\n");
    int tam = tamanho_lista_palavras(lista); 
    int pos=0;
    idc*vetor = (idc*)malloc(tam*sizeof(idc));
    for(lst pl=lista; pl!=NULL; pl=pl->prox){
        strcpy(vetor[pos].palavra, converte_minusculo(pl->palavra));
        vetor[pos].prim_pos_tup = -1;
        pos++;
    }
    printf("indice gerado com sucesso\n");
    return vetor;
}

//=======================INT FUNCOES====================//
int arquivo_bin_existe(char*arquivo){
    FILE*arq=fopen(arquivo, "rb");
    if(arq == NULL)
        return 0;
    return 1;
}
int existe(lst lista, char*palavra){
    if(lista == NULL)
        return 0;
    else{
        return strcmp(converte_minusculo(lista->palavra),converte_minusculo(palavra)) == 0? 1: existe(lista->prox, palavra);
    }
}
int quant_caracter(char*arquivo){
    FILE*arq = fopen(arquivo,"r");
    int c;
    int i=0;
    while((c=fgetc(arq))!=EOF){
        i++;
    }
    fclose(arq);
    return i;
}
int tamanho_lista_palavras(lst lista){
    return lista != NULL? 1 + tamanho_lista_palavras(lista->prox) : 0; 
}
int cont_freq_arq(lst lista, char*palavra){
    return lista == NULL? 0: strcmp(lista->palavra, converte_minusculo(palavra)) == 0? 1 + cont_freq_arq(lista->prox, converte_minusculo(palavra)): cont_freq_arq(lista->prox, converte_minusculo(palavra));
}
int cont_quant_arq(char*arquivo){
    FILE*arq=fopen(arquivo, "rb");
    int cont=0, id;
    tup elem;
    fread(&elem, sizeof(tup), 1, arq);
    id = elem.id;
    while(elem.id == id){
        cont++;
        fread(&elem, sizeof(tup), 1, arq);
    }
    return cont;

}
int compara (const void * a, const void * b){
    tup *info1 = (tup *)a;
    tup *info2 = (tup *)b;
    return ( info2->freq - info1->freq );
}
int menu(char*texto){
    int opc;
    printf("%s", texto);
    scanf("%d", &opc);
    return opc;
}
int existe_idc(idc*vetor, int tam, char*palavra){
    for(int i=0; i<tam; i++){
        if(strcmp(vetor[i].palavra, palavra) == 0)
            return 1;
    }
    return 0;
}

//=======================VOID FUNCOES====================//
void criar_arq_bin(idc*vetor, int tam){
    printf("criando arquivo indice.bin\n");
    FILE*arq = fopen("indice.bin", "wb");
    for(int i=0; i<tam; i++){
        fwrite(&vetor[i], sizeof(idc), 1, arq);
    }
    fclose(arq);
    printf("arquivo criado com sucesso\n");
}
void criar_arq_bin_tup(tup*vetor, int tam, int quant_arq){
    printf("criando arquivo tupla.bin\n");
    FILE*arq = fopen("registros.bin","wb");
    for(int i=0; i<tam; i++){
        fwrite(&vetor[i], sizeof(tup), 1, arq);
    }
    fclose(arq);
    // FILE*arq2 = fopen("quant_arq.bin", "wb");
    // fwrite(&quant_arq, sizeof(int), 1, arq2);
    // fclose(arq2);
    printf("arquivo criado com sucesso\n");
}
void imprimir_indice(idc*vetor, int tam){
    for(int i=0; i<tam; i++){
        printf("%s\n", vetor[i].palavra);
    }
}
void imprimir_tupla(tup*vetor, int tam){
    for(int i=0; i<tam; i++){
        if(vetor[i].freq>0)
            printf("%d %s %d\n", vetor[i].id, vetor[i].arquivo, vetor[i].freq);
    }
}
void busca_eh_arq(char*arquivo_idc, char*arquivo_tup, char*palavra, char*palavra2, int qtd_arq){
    FILE*arq = fopen(arquivo_idc, "rb");
    fseek(arq, 0, SEEK_END);
    int tam1 = ftell(arq)/sizeof(idc), pos = 0, pos_p1, pos_p2;
    fseek(arq, 0, SEEK_SET);
    idc elem1, elem2;
    while(pos<tam1){
        fread(&elem1, sizeof(idc), 1, arq);
        if(strcmp(elem1.palavra, palavra) == 0){
            pos_p1 = pos;
            break;
        }
        pos++;
    }
    pos = 0;
    fseek(arq, 0, SEEK_SET);
    while(pos<tam1){
        fread(&elem2, sizeof(idc), 1, arq);
        if(strcmp(elem2.palavra, palavra2) == 0){
            pos_p2 = pos;
            break;
        }
        pos++;
    }
    fclose(arq);
    
    FILE*arq2 = fopen(arquivo_tup, "rb");
    tup*vetor1 = (tup*)malloc(qtd_arq*sizeof(tup));
    tup*vetor2 = (tup*)malloc(qtd_arq*sizeof(tup));
    fseek(arq2, elem1.prim_pos_tup*sizeof(tup), SEEK_SET);
    fread(vetor1, sizeof(tup), qtd_arq, arq2);
    fseek(arq2, elem2.prim_pos_tup*sizeof(tup), SEEK_SET);
    fread(vetor2, sizeof(tup), qtd_arq, arq2);
    
    for(int i=0; i<qtd_arq; i++){
        if(vetor1[i].freq >0 && vetor2[i].freq > 0)
            printf("%s [p1: %d] [p2: %d]\n", vetor1[i].arquivo, vetor1[i].freq, vetor2[i].freq);
    }
    fclose(arq2);

}
void busca_ou_arq(char*arquivo_idc, char*arquivo_tup, char*palavra, char*palavra2, int qtd_arq){
    FILE*arq = fopen(arquivo_idc, "rb");
    fseek(arq, 0, SEEK_END);
    int tam1 = ftell(arq)/sizeof(idc), pos = 0, pos_p1, pos_p2;
    fseek(arq, 0, SEEK_SET);
    idc elem1, elem2;
    while(pos<tam1){
        fread(&elem1, sizeof(idc), 1, arq);
        if(strcmp(elem1.palavra, palavra) == 0){
            pos_p1 = pos;
            break;
        }
        pos++;
    }
    pos = 0;
    fseek(arq, 0, SEEK_SET);
    while(pos<tam1){
        fread(&elem2, sizeof(idc), 1, arq);
        if(strcmp(elem2.palavra, palavra2) == 0){
            pos_p2 = pos;
            break;
        }
        pos++;
    }
    fclose(arq);
    
    FILE*arq2 = fopen(arquivo_tup, "rb");
    tup*vetor1 = (tup*)malloc(qtd_arq*sizeof(tup));
    tup*vetor2 = (tup*)malloc(qtd_arq*sizeof(tup));
    fseek(arq2, elem1.prim_pos_tup*sizeof(tup), SEEK_SET);
    fread(vetor1, sizeof(tup), qtd_arq, arq2);
    fseek(arq2, elem2.prim_pos_tup*sizeof(tup), SEEK_SET);
    fread(vetor2, sizeof(tup), qtd_arq, arq2);
    
    for(int i=0; i<qtd_arq; i++){
        if(vetor1[i].freq > 0 || vetor2[i].freq > 0)
            printf("%s [p1: %d] [p2: %d]\n", vetor1[i].arquivo, vetor1[i].freq, vetor2[i].freq);
    }
    fclose(arq2);

}
void imprimir_lista(lst lista){
    for(lst pl = lista; pl!=NULL; pl=pl->prox){
        printf("%s\n", pl->palavra);
    }
}
void imprimir_do_arquivo(char*arquivo_idc, char*arquivo_tup, int qtd_arq, char*palavra){
    FILE*arq = fopen(arquivo_idc, "rb");
    if(arq == NULL)
        printf("arquivo inexistente\n");
    fseek(arq, 0, SEEK_END);
    int tam1 = ftell(arq)/sizeof(idc), pos = 0;
    fseek(arq, 0, SEEK_SET);
    idc elem;
    while(pos < tam1){
        fread(&elem, sizeof(idc), 1, arq);
        if(strcmp(elem.palavra, palavra) == 0)
            break;
        pos++;
    }
    fclose(arq);
    if(pos < tam1){
        // printf("termo: %s\n", elem.palavra);
        FILE*arq2 = fopen(arquivo_tup, "rb");
        tup*elem2 = (tup*)malloc(qtd_arq*sizeof(tup));
        fseek(arq, elem.prim_pos_tup*sizeof(tup), SEEK_SET);
        fread(elem2, sizeof(tup), qtd_arq, arq2);

        qsort(elem2, qtd_arq, sizeof(tup), compara);

        for(int i=0; i<qtd_arq; i++){
            if(elem2[i].freq >0)
                printf("%s : %d\n",elem2[i].arquivo, elem2[i].freq);
        }
        fclose(arq2);
    }
    else{
        printf("termo inexistente\n");
    }
}

//=======================CHAR FUNCOES====================//
char*converte_minusculo(char*palavra){
    int tam = strlen(palavra);
    int *min = (int*)malloc(tam*sizeof(int));
    char *result = (char*)malloc((tam+1)*sizeof(char));

    for(int i=0; i<tam; i++){
        min[i] = tolower(palavra[i]);
        result[i] = min[i];
    }
    result[tam] = '\0';
    return result;
}
char**vetor_arquivos(char*arquivo_tup, int quant_arq){
    char**ptr = (char**)malloc(quant_arq*sizeof(char*));
    FILE*arq = fopen(arquivo_tup, "rb");
    tup elem;
    for(int i=0; i<quant_arq; i++){
        fread(&elem, sizeof(tup), 1, arq);
        ptr[i] = elem.arquivo;
    }
    fclose(arq);
    return ptr;
}