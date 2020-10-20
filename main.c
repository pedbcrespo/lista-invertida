#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include"funcoes.h"
//para executar: gcc main.c funcoes.c -o main.exe
/*Por se tratar de um trabalho de faculdade, eu considero que o arquivo chamado "docs" esteja dentro da pasta do projeto
e que todo novo arquivo texto criado tambem esteja na mesma pasta*/

int menu();
int sub_menu();

int main(){
    int tam, tam_tup, opc = -1, quant_arq = 20;
    lst lista = NULL, lista_arquivos = NULL, copia = NULL;
    idc*vetor = ler_arquivo_bin("indice.bin", &tam);
    tup*vetor_tup = ler_arquivo_bin_tup("registros.bin", &tam_tup);
    char arquivo[20][20], **ptr, palavra[20];
    char*url = ".\\docs\\doc";

    if(vetor == NULL || vetor_tup == NULL){
        ptr = (char**)malloc(quant_arq*sizeof(char*));

        for(int i=1; i<=quant_arq; i++){
            snprintf(arquivo[i-1], 20, "%s%d%s", url, i, ".txt");
            ptr[i-1] = arquivo[i-1];
            lista_arquivos = inserir_lst(lista_arquivos, arquivo[i-1]);
            lista = conjunto_listas(lista, ler_arquivo_texto(arquivo[i-1]));
        }
        lista = mantem_mais_repetidas(lista, ptr, quant_arq);

        tam = tamanho_lista_palavras(lista);
        printf("tam: %d\n", tam);
        vetor = gera_indice(lista);
        vetor_tup = gera_tupla(vetor_tup, vetor, tam, ptr, &tam_tup, quant_arq);

        criar_arq_bin(vetor, tam);
        criar_arq_bin_tup(vetor_tup, tam_tup);       
    }
    system("pause");
    while(opc != 0){
    system("cls");
        opc = menu();
        if(opc == 1){//Buscar uma palavra;
            // imprimir_indice(vetor, tam);
            imprimir_lista(lista);
            printf("\n\ntermo buscado: ");
            scanf("%s", palavra);        
            imprimir_termo_buscado(vetor, tam, vetor_tup, palavra);
        }
        else if(opc == 2){//Buscar 2 palavras
            opc = sub_menu();
            char palavra1[20], palavra2[20];
            if(opc == 1){
                printf("termo 1 e 2: ");
                scanf("%s %s", palavra1, palavra2);
                busca_eh(vetor, tam, vetor_tup, quant_arq, palavra1, palavra2);
            }
            else if(opc == 2){
                printf("termo 1 ou 2: ");
                scanf("%s %s", palavra1, palavra2);
                busca_ou(vetor, tam, vetor_tup, quant_arq, palavra1, palavra2);
            }
            else{
                printf("opcao invalida\n");
                system("pause");
            }
        }
        else if(opc == 3){//Adicionar arquivo texto
            char novo_arquivo[20], **ptr2;
            int pos=0;

            system("cls");
            printf("novo arquivo: ");
            scanf("%s", novo_arquivo);
            ptr2 = (char**)malloc(tamanho_lista_palavras(lista_arquivos) * sizeof(char*));
            
            lista_arquivos = inserir_lst(lista_arquivos, novo_arquivo);
            quant_arq++;
            for(lst arq=lista_arquivos; arq!=NULL; arq=arq->prox){
                ptr2[pos] = arq->palavra;
                pos++;
            }
            free(vetor_tup);
            vetor_tup = NULL;
            vetor_tup = gera_tupla(vetor_tup, vetor, tam, ptr2, &tam_tup, quant_arq);
            if(!remove("registros.bin")){
                criar_arq_bin_tup(vetor_tup, tam_tup);
                vetor_tup = ler_arquivo_bin_tup("registros.bin", &tam_tup);
            }
        }
        else{//Sair
            break;
        }
        system("pause");    
    }


}

int menu(){
    int opc;
    printf("1)buscar um termo\n2)busca mais de uma termo\n3)adicionar arquivo texto\n0)sair\n");
    scanf("%d", &opc);
    return opc;
}

int sub_menu(){
    int opc;
    printf("\t1)busca tipo 'E'\n\t2)busca tipo 'OU'\n");
    scanf("%d", &opc);
    return opc;
}
