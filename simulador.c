#include "bibliotecas.h"


main(void){

}

int escreve_ficheiro(char texto2[]){
        FILE *fp;
        fp = fopen("ficheiro_simulador.txt","a"); //Abre o ficheiro no modo de write para escrever no fim
        if(fp == NULL){ 
                printf("Erro ao abrir o ficheiro");
        }
        else{
                fprintf(fp,texto2); //Escreve o texto no ficheiro
                fclose(fp); //fecha o ficheiro
                return 0; //retorna 0 para indicar que funcionou corretamente
        }
}