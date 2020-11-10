#include "bibliotecas.h"
int main (){
    
}

/* Escrever no ficheiro E no monitor */
int escreve_monitor_ficheiro(char texto[]){
        FILE *fp;
        fp = fopen("ficheiro_teste.txt","a+"); //Abre o ficheiro no modo de read/write para escrever no fim
        if(fp == NULL){ //Caso o ficheiro nao exista ou nao seja aberto
                printf("Erro ao abrir o ficheiro");
                return -1; //Indicaçao do erro
        }
        else{
                fprintf(fp,texto); //Escreve o text no ficheiro
                printf(texto); //Imprime para o monitor o que foi escrito no ficheiro
                fclose(fp); //fecha o ficheiro
                if(fclose(fp) == EOF){ //Quando ocorre um erro a fechar o ficheiro fclose retorna EOF
                        printf("Erro ao fechar o ficheiro");
                        return 1; //Indicacao do erro
                }
                return 0; //retorna 0 para indicar que funcionou corretamente
        }
}
