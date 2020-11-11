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

        FILE *apontador;
        apontador = fopen("ficheiro_teste.txt", "r");
        char linha[MAXSIZE];
        while(!feof(apontador)){

                fgets(linha, MAXSIZE , apontador);
                printf(linha);
                }
                fclose(apontador);
        
        }
int main ( int argc , char const * argv[] ) {
        //escreve_monitor_ficheiro("hello");
        le();
    return 0;
}