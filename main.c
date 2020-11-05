#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include"funcoes.h"
//gcc main.c funcoes.c -o main.exe
/*Considerei que o arquivo '.\\docs' estaria na mesma pasta do projeto*/
int main(){
    char*url = ".\\docs\\doc", **ptr, arquivo[60][60], termo[60];
    int opc = -1, qtd_arq = 20, tam_tup, tam_idc;
    lst lista = NULL;
    lst stop_words = ler_arquivo_texto("stop_words.txt");
    idc*indice = ler_arquivo_bin("indice.bin", &tam_idc);
    tup*tupla = ler_arquivo_bin_tup("registros.bin", &tam_tup);
    if(!arquivo_bin_existe("indice.bin")){
        ptr = (char**)malloc(20*sizeof(char*));
        for(int i=1; i<=20; i++){
            snprintf(arquivo[i-1], 60, "%s%d%s", url, i, ".txt");
            ptr[i-1] = arquivo[i-1];
            lista = conjunto_listas(lista, ler_arquivo_texto(arquivo[i-1]));
            lista = remove_stop_words(lista, stop_words);
        }
        lista = tratamento_repeticao(lista);
        
        indice = gera_indice(lista);
        tam_idc = tamanho_lista_palavras(lista);
        tam_tup = tam_idc*qtd_arq;
        tupla = gera_tupla(indice, ptr, tam_tup, qtd_arq);

        criar_arq_bin(indice, tam_idc);
        criar_arq_bin_tup(tupla, tam_tup, qtd_arq);
    }else{
        qtd_arq = cont_quant_arq("registros.bin");
        ptr = vetor_arquivos("registros.bin", qtd_arq);
    }
    system("cls");
    while(opc != 0){
        // imprimir_indice(indice, tam_idc);
        opc = menu("1)buscar termo\n2)busca E/OU\n3)adicionar arquivo txt\n0)sair\n");
        system("cls");
        if(opc == 1){
            printf("termo: ");
            scanf("%s", termo);
            imprimir_do_arquivo("indice.bin", "registros.bin", qtd_arq, termo);
        }
        else if(opc == 2){
            int opc2 = menu("1)busca E \n2)busca OU\n");
            char termo2[60];
            printf("termo 1 e termo 2: ");
            scanf("%s %s", termo, termo2);
            if(opc2 == 1){
                busca_eh_arq("indice.bin", "registros.bin", termo, termo2, qtd_arq);
            }
            else if(opc2 == 2){
                busca_ou_arq("indice.bin", "registros.bin", termo, termo2, qtd_arq);
            }
            else{
                printf("opcao invalida\n");
            }
        }
        else if(opc == 3){
            char novo_arquivo[60];
            char**ptr2 = (char**)malloc((qtd_arq+1)*sizeof(char*));
            printf("arquivo: ");
            scanf("%s", novo_arquivo);
            ptr2[qtd_arq] = novo_arquivo;
            for(int i=0; i<qtd_arq; i++){
                ptr2[i] = ptr[i];
            }
            qtd_arq += 1;
            indice = ler_arquivo_bin("indice.bin", &tam_idc);
            tam_tup = tam_idc*qtd_arq;
            tupla = gera_tupla(indice, ptr2, tam_tup, qtd_arq);
            criar_arq_bin(indice, tam_idc);
            criar_arq_bin_tup(tupla, tam_tup, qtd_arq);
            ptr = ptr2;
        }
        else if(opc == 4){
            // imprimir_indice(ler_arquivo_bin("indice.bin", &tam_idc), tam_idc);
            scanf("%s", termo);
            existe_idc(ler_arquivo_bin("indice.bin", &tam_idc), tam_idc, termo)? printf("termo existe\n") : printf("termo inexistente\n");

        }
        else{
            break;
        }
        system("pause");
        system("cls");
    }

}

