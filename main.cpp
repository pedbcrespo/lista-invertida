#include<iostream>
#include<string>
#include"Tabela.hpp"
using namespace std;

// para executar:
// abra o terminal, acesse a pasta por ele e digite:
// g++ main.cpp Tabela.cpp -o main.exe


// essa funcao é por conta de um trabalho da faculdade no qual eu tinha que ler arquivos textos guardados numa pasta
// como os nomes ja sao de conhecimento, entao criei essa funcao especificamente para esse caso
// para executar o programa de forma generica, apenas remova essa funcao da main
void ler_arquivos_docs(Tabela*tab){
    /*considero que essa pastar estara dentro do projeto*/
    char arquivo[50];
    for(int i=1; i<=20; i++){
        snprintf(arquivo, 50, "%s%d%s", ".\\docs\\doc", i, ".txt");
        tab->adiciona_arquivo(arquivo);
    }
}

int menu(string texto){
    int op;
    cout<<texto<<endl;
    cin>>op;
    return op;
}

int main(){
    int op = -1;
    char arq_stop_words[] = ".\\stop_words.txt";
    char termo1[60],termo2[60];
    Tabela *t = new Tabela();
    t->insere_stop_words(arq_stop_words);
    ler_arquivos_docs(t); //para funcionar de forma generica, remova essa funcao
    t->gerar();
    system("cls");
    while(op != 0){
        op = menu("1)busca simples\n2)busca dupla\n3)adicionar arquivo\n0)sair");
        system("cls");
        if(op == 1){
            cout<<"termo: ";
            cin>>termo1;
            t->busca_simples(termo1);
        }
        else if(op == 2){
            int op2 = menu("1)busca eh\n2)busca ou");
            cout<<"termo 1: ";
            cin>>termo1;
            cout<<"termo 2: ";
            cin>>termo2;

            if(op2 == 1){
                t->busca_eh(termo1, termo2);
            }
            else if(op2 == 2){
                t->busca_ou(termo1, termo2);
            }
            else{
                cout<<"opcao invalida"<<endl;
            }
        }
        else if(op == 3){
            char novo_arquivo[100];
            cout<<"digite o caminho do arquivo: "<<endl;
            cin>>novo_arquivo;
            t->adiciona_arquivo(novo_arquivo);
            cout<<"arquivo adicionado\n"<<"atualizando"<<endl;
            t->imprimir_lista_arquivos();
            system("pause");
            t->atualizar_tabela();
        }
        else{
            break;
        }

        system("pause");
        system("cls");
    }

    cout<<"terminou"<<endl;
}