#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include"funcoes.h"
//gcc main.c funcoes.c -o main.exe
/*Considerei que o arquivo '.\\docs' estaria na mesma pasta do projeto*/
int menu();
int sub_menu(char*texto);

int main(){
    int tam_idc, tam_tup, opc = -1, quant_arq = 20;
    lst lista = ler_arquivo_texto("stop_words.txt");
    idc*vetor_idc = ler_arquivo_bin("indice.bin", &tam_idc);
    tup*vetor_tup = ler_arquivo_bin_tup("registros.bin", &tam_tup, &quant_arq);
    char arquivo[60][60], **ptr, palavra[50];
    char*url = ".\\docs\\doc";

    if(vetor_idc == NULL || vetor_tup == NULL){
        ptr = (char**)malloc(quant_arq*sizeof(char*));

        for(int i=1; i<=quant_arq; i++){
            snprintf(arquivo[i-1], 60, "%s%d%s", url, i, ".txt");
            ptr[i-1] = arquivo[i-1];
        }
        
        tam_idc = tamanho_lista_palavras(lista);
        tam_tup = tam_idc*quant_arq;
        vetor_idc = gera_indice(lista);
        vetor_tup = gera_tupla(vetor_idc, ptr, tam_tup, quant_arq);
        
        criar_arq_bin(vetor_idc, tam_idc);
        criar_arq_bin_tup(vetor_tup, tam_tup, quant_arq);
    }
    else{
        ptr = vetor_arquivos(vetor_tup, quant_arq);
    }
    system("pause");
    system("cls");

    while(opc != 0){
        printf("quant arq: %d\n", quant_arq);
        opc = menu("1)buscar termo\n2)buscar 2 termos\n3)adicionar arquivo texto\n4)imprimir arquivos registrados\n0)sair\n");
        system("cls");
        if(opc == 1){//Buscar 1 termo
            imprimir_lista(lista);
            printf("\ntermo: ");
            scanf("%s", palavra);
            imprimir_do_arquivo("indice.bin", "registros.bin", quant_arq, palavra);
        }
        else if(opc == 2){//Buscar 2 termos
            int opc2 = menu("\n1)Busca E\n2)Busca OU\n");
            char palavra2[50];
            printf("termo 1 e 2: ");
            scanf("%s %s", palavra, palavra2);
            if(opc2 == 1){
                busca_eh_arq("indice.bin", "registros.bin", palavra, palavra2, quant_arq);
            }
            else if(opc2 == 2){
                busca_ou_arq("indice.bin", "registros.bin", palavra, palavra2, quant_arq);
            }
            else{
                printf("opcao invalida\n");
            }
        }
        else if(opc == 3){//Adicionar arquivo
            char**ptr2 = (char**)malloc((quant_arq+1)*sizeof(char*));
            printf("arquivo: ");
            scanf("%s", palavra);
            ptr2[quant_arq] = palavra;
            for(int i=0; i<quant_arq; i++){
                ptr2[i] = ptr[i];
            }
            quant_arq += 1;
            tam_tup = tam_idc*quant_arq;
            vetor_tup = gera_tupla(vetor_idc, ptr2, tam_tup, quant_arq);
            criar_arq_bin_tup(vetor_tup, tam_tup, quant_arq);
            ptr = ptr2;
        }
        else if(opc == 4){//Imprimir arquivos registrados
            for(int i=0; i<quant_arq; i++){
                printf("%s\n", ptr[i]);
            }
        }
        else{
            break;
        }
        system("pause");
        system("cls");
    }
    lista = destruir_lst(lista);
}

