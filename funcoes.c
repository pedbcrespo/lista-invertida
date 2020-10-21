#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include"funcoes.h"

//=======================TUP FUNCOES====================//
tup*gera_tupla(tup*vetor, idc*indice, int tam_idc, char**arquivos, int *tam_tup, int quant_arq){
    printf("gerando tupla\n");
    int prim_pos, pos=0;
    vetor = (tup*)malloc(tam_idc*quant_arq*sizeof(tup));
    
    for(int i=0; i<tam_idc*quant_arq; i+=quant_arq){
        vetor[i].id = pos;
        system("cls");
        printf("lendo arquivos\n%d %%\n", (i*100)/(tam_idc*quant_arq));
        for(int n=0; n<quant_arq; n++){
            vetor[i+n].id = pos;
            strcpy(vetor[i+n].arquivo, arquivos[n]);
            vetor[i+n].freq = cont_freq_arq(ler_arquivo_texto(arquivos[n]), indice[pos].palavra);
        }
        indice[pos].prim_pos_tup = i;
        pos++;
    }
    *tam_tup = tam_idc*quant_arq;
    printf("tupla gerada com sucesso\n");
    return vetor;
}
tup*ler_arquivo_bin_tup(char*arquivo, int*tam, int*quant_arq){
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
    FILE*arq2 = fopen("quant_arq.bin", "rb");
    if(arq2 == NULL){
        *quant_arq = 20;
    }
    fread(quant_arq, sizeof(int), 1, arq);
    fclose(arq2);
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
lst conjunto_listas(lst lista1, lst lista2){
    for(lst ax = lista2; ax != NULL; ax = ax->prox){
        lista1 = inserir_lst(lista1, ax->palavra);
    }
    return tratamento_repeticao(lista1);
}
lst destruir_lst(lst no){
	if(no != NULL){
		no->prox = destruir_lst(no->prox);
		free(no);
		no = NULL;
	}
	return no;
}
lst remover_lst(lst no, char*elem){
    if(no!=NULL){
        if(strcmp(no->palavra, elem) == 0){
            lst ax = no->prox;
            free(no);
            return ax;
        }
        else{
            no->prox = remover_lst(no->prox, elem);
        }
    }
    return no;
}
lst tratamento_repeticao(lst lista){
    lst lista_atualizada = NULL;
    for(lst aux = lista; aux != NULL; aux = aux->prox){
        if(strlen(aux->palavra) >=6 && !existe(lista_atualizada, aux->palavra)){
            lista_atualizada = inserir_lst(lista_atualizada, aux->palavra);
        }
    }
    lista = destruir_lst(lista);
    return lista_atualizada;
}
lst ler_arquivo_texto(char*arquivo){
    // printf("lendo arquivo %s\n", arquivo);
    FILE*arq = fopen(arquivo,"r");
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
        pch = strtok(texto, " ,.:;!?()[]\n\t");
        while(pch != NULL){
            lista = inserir_lst(lista, converte_minusculo(pch));
            pch = strtok(NULL, " ,.:;!?()[]\n\t");
        }

        fclose(arq);
    }
    // printf("arquivo lido com sucesso\n");
    return lista;
}
lst mantem_mais_repetidas(lst lista, char**arquivos, int qtd_arq){
    printf("filtrando palavras mais repetidas\n");
    int tam = tamanho_lista_palavras(lista);
    lst nova_lista = NULL;
    int cont, pos=0;
    for(lst pl=lista; pl!=NULL; pl=pl->prox){
        cont=0;
        system("cls");
        printf("filtrando palavras mais repetidas\n");
        printf("%d%%\n", (pos*100)/tam);
        for(int i=0; i<qtd_arq; i++)
            cont += cont_freq_arq(ler_arquivo_texto(arquivos[i]), pl->palavra);
        if(cont>1)
            nova_lista = inserir_lst(nova_lista, pl->palavra);
        pos++;
    }
    printf("completo\n");
    return nova_lista;
}

//=======================IDC FUNCOES====================//
idc*inserir_idc(idc*vetor, int*tam, char*palavra){
    printf("verificando indice\n");
    idc*novo_vet = (idc*)malloc((*tam+1)*sizeof(idc));
    if(vetor == NULL){
        vetor = (idc*)malloc(sizeof(idc));
        strcpy(vetor[0].palavra, palavra);
        *tam = 1;
    }
    else{
        if(existe_idc(vetor, *tam, palavra) != -1){
            for(int i=0; i<*tam; i++){
                strcpy(novo_vet[i].palavra, vetor[i].palavra);
            }
            strcpy(novo_vet[*tam].palavra, palavra);
            *tam += 1;
            free(vetor);
            return novo_vet;
        }else
            return vetor;
    }
    return vetor;
}
idc*ler_arquivo_bin(char*arquivo, int*tam){//SEM PROBLEMAS POR AQUI
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
        strcpy(vetor[pos].palavra, pl->palavra);
        pos++;
    }
    printf("indice gerado com sucesso\n");
    return vetor;
}

//=======================INT FUNCOES====================//
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
int existe(lst lista, char*elem){
    if(lista == NULL)
        return 0;
    else{
        return strcmp(converte_minusculo(lista->palavra),converte_minusculo(elem)) == 0? 1: existe(lista->prox, elem);
    }
}
int existe_idc(idc*vetor, int tam, char*elem){
    for(int i=0; i<tam; i++){
        if(strcmp(vetor[i].palavra, elem) == 0)
            return i;
    }
    return -1;
}
int tamanho_lista_palavras(lst lista){
    return lista != NULL? 1 + tamanho_lista_palavras(lista->prox) : 0; 
}
int cont_freq_arq(lst lista, char*palavra){
    int cont = 0;
    for(lst aux = lista; aux != NULL; aux = aux->prox){
        if(strcmp(aux->palavra, palavra) == 0)
            cont++;
    }
    return cont;
}
int cont_repet_lst(lst lista, char*palavra){
    int cont=0;
    for(lst pl=lista; pl!=NULL; pl=pl->prox){
        if(strcmp(pl->palavra, palavra)==0)
            cont++;
    }
    return cont;
}
int busca_pos(idc*vetor, int tam, char*palavra){
    for(int i=0; i<tam; i++){
        if(strcmp(vetor[i].palavra, palavra)==0){
            return i;
        }
    }
    
    return -1;
}
int compara (const void * a, const void * b){
    tup *info1 = (tup *)a;
    tup *info2 = (tup *)b;
    return ( info2->freq - info1->freq );
}

//=======================VOID FUNCOES====================//
void criar_arq_bin(idc*vetor, int tam){
    printf("criando arquivo indice.bin\n");
    FILE*arq = fopen("indice.bin", "ab");
    for(int i=0; i<tam; i++){
        fwrite(&vetor[i], sizeof(idc), 1, arq);
    }
    fclose(arq);
    printf("arquivo criado com sucesso\n");
}
void criar_arq_bin_tup(tup*vetor, int tam, int quant_arq){
    printf("criando arquivo tupla.bin\n");
    FILE*arq = fopen("registros.bin","ab");
    for(int i=0; i<tam; i++){
        fwrite(&vetor[i], sizeof(tup), 1, arq);
    }
    fclose(arq);
    FILE*arq2 = fopen("quant_arq.bin", "wb");
    fwrite(&quant_arq, sizeof(int), 1, arq2);
    fclose(arq2);
    printf("arquivo criado com sucesso\n");
}
void imprimir_indice(idc*vetor, int tam){
    for(int i=0; i<tam; i++){
        printf("%s\n", vetor[i].palavra);
    }
}
void imprimir_termo_buscado(idc*vetor, int tam, tup*vetor_tup, char*palavra){
    for(int i=0; i<tam; i++){
        if(strcmp(vetor[i].palavra, palavra)==0){
            int pos = vetor[i].prim_pos_tup;
            printf("termo: %s\n", palavra);
            while(vetor_tup[pos].id == i){
                if(vetor_tup[pos].freq>0)
                    printf("%s : %d\n", vetor_tup[pos].arquivo, vetor_tup[pos].freq);
                pos++;
            }
            break;
        }
    }

}
void imprimir_termo_buscado_alt(idc*vetor, int tam, tup*vetor_tup, int quant_arq, char*palavra){
    int pos, cont=0;
    for(int i=0; i<tam; i++){
        if(strcmp(vetor[i].palavra, palavra)==0){
            pos = vetor[i].prim_pos_tup;
            break;
        }
    }
    tup*aux = (tup*)malloc(quant_arq*sizeof(tup));
    for(int i=0; i<quant_arq; i++){
        aux[i].freq = vetor_tup[pos+i].freq;
        strcpy(aux[i].arquivo, vetor_tup[pos+i].arquivo);
    }
    
    imprimir_ordenado(aux, quant_arq, palavra);

}
void imprimir_tupla(tup*vetor, int tam){
    for(int i=0; i<tam; i++){
        if(vetor[i].freq>0)
            printf("%d %s %d\n", vetor[i].id, vetor[i].arquivo, vetor[i].freq);
    }
}
void busca_eh(idc*vetor, int tam, tup*vetor_tup, int quant_arq, char*palavra1, char*palavra2){
    char **vet_char = (char**)malloc(quant_arq*sizeof(char*));
    int n=0, pos1, pos2;
    for(int i=0; i<tam; i++){
        if(strcmp(vetor[i].palavra, palavra1)==0){
            pos1 = vetor[i].prim_pos_tup;
            int pos = vetor[i].prim_pos_tup;
            while(vetor_tup[pos].id == i){
                if(vetor_tup[pos].freq>0){
                    vet_char[n] = vetor_tup[pos].arquivo;
                    n++;
                }    
                pos++;
            }
            break;
        }
    }
    for(int i=0; i<tam; i++){
        if(strcmp(vetor[i].palavra, palavra2)==0){
            pos2 = vetor[i].prim_pos_tup;
            printf("termos: %s e %s\n", palavra1, palavra2);
            while(vetor_tup[pos2].id == i){
                if(vetor_tup[pos2].freq>0){
                    for(int x=0; x<n; x++){
                        if(strcmp(vetor_tup[pos2].arquivo, vet_char[x])==0)
                            printf("%s: p1: %d p2: %d\n", vet_char[x], vetor_tup[pos1].freq, vetor_tup[pos2].freq);
                    }
                }
                pos1++;
                pos2++;
            }
            break;
        }
    }
}
void busca_ou(idc*vetor, int tam, tup*vetor_tup, int quant_arq, char*palavra1, char*palavra2){
    int pos1, pos2, cont=0;
    for(int i=0; i<tam; i++){
        if(strcmp(vetor[i].palavra, palavra1)==0 || strcmp(vetor[i].palavra, palavra2)==0){
            if(strcmp(vetor[i].palavra, palavra1)==0){
                pos1 = vetor[i].prim_pos_tup;
                cont++;
            }    
            if(strcmp(vetor[i].palavra, palavra2)==0){
                pos2 = vetor[i].prim_pos_tup;
                cont++;
            }
            if(cont>=2)
                break;
        }
    }   

    printf("termos: %s ou %s\n", palavra1, palavra2);
    for(int i=0; i<quant_arq; i++){
        if((vetor_tup[pos1 + i].freq > 0) || (vetor_tup[pos2 + i].freq > 0)){
            printf("%s: p1: %d p2: %d\n", vetor_tup[pos1+i].arquivo, vetor_tup[pos1+i].freq, vetor_tup[pos2+i].freq);
        }
    }
}
void imprimir_lista(lst lista){
    for(lst pl = lista; pl!=NULL; pl=pl->prox){
        printf("%s\n", pl->palavra);
    }
}
void imprimir_ordenado(tup*vetor, int tam, char*palavra){
    int cont=0;
    char arq_maior[20];
    
    qsort(vetor, tam, sizeof(tup), compara);

    printf("termo: %s\n", palavra);
    for(int i=0; i<tam; i++){
        if(vetor[i].freq>0)
            printf("%s : %d\n", vetor[i].arquivo, vetor[i].freq);
    }
}
//=======================CHAR FUNCOES====================//
char*converte_minusculo(char*palavra){//SEM PROBLEMAS POR AQUI
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