#include "bibliotecas.h"

/* Escrever no ficheiro E no monitor */
int escreve_monitor_ficheiro(char texto[]){
                FILE *fp;
                fp = fopen("ficheiro_teste.txt","a"); //Abre o ficheiro no modo de write para escrever no fim
                if(fp == NULL){ // Verifica se o ficheiro existe
                        printf("Erro ao abrir o ficheiro");
                }
                else{
                        fprintf(fp,texto); //Escreve o text no ficheiro
                        printf(texto); //Imprime para o monitor o que foi escrito no ficheiro
                        fclose(fp); //fecha o ficheiro
        }     
}
int le(){
        #define MAXSIZE 512

        FILE *apontador; //cria apontador para abrir o ficheiro
        apontador = fopen("ficheiro_teste.txt", "r"); // abre o ficheiro para leitura
        char linha[MAXSIZE]; 
        while(!feof(apontador)){ // enquanto não chegar ao fim do ficheiro

                fgets(linha, MAXSIZE , apontador); // lê apenas uma linha do ficheiro
                printf(linha); // dá o print dessa mesma linha
                }
                fclose(apontador); // fecha o ficheiro
        
        }
int main ( int argc , char const * argv[] ) {
        //escreve_monitor_ficheiro("hello");
        le();
    return 0;
}