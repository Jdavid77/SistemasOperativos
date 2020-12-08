#include "bibliotecas_estruturas.h"


main(void){

}

void leConfigura(){
        int erro = 0;
        #define MAXSIZE 512
        char linha[MAXSIZE];
        int valores_configura [8];
        while (erro==0)
        {
                FILE *ficheiro_configura;
                ficheiro_configura = fopen("configura.txt","r"); //Abre o ficheiro no modo de read
                if(ficheiro_configura == NULL){ 
                        printf("Erro ao abrir o ficheiro");
                }
                else{
                        int j = 0;
                        while(!feof(ficheiro_configura)){ // enquanto não chegar ao fim do ficheiro
                                fgets(linha, MAXSIZE,ficheiro_configura); // lê apenas uma linha do ficheiro
                
                                for(int i = 0; i<4; i++){ //le os primeiros 4 carateres da linha 
                                        if (linha[i] == '='){ //caso seja "=" 
                                                break;
                                        }
                                        else{
                                                if (isalpha(linha[i])){ //verfica se é carater alfabeto A a Z
                                                        printf("ERRO! Inseriu um valor com o carater '%c' na linha %i \n", linha[i],j+1); //j+1 é numero da linha  e linha[i] é o carater
                                                        erro = 1;  
                                                }
                                        }
                                }              
                        char *token = strtok(linha, "=");//utilizar um token "=" para dividir as frases, do numero que queremos guardar 
                        valores_configura[j]=atoi(token);//passa o valor para o array       
                        j++;
                        } //adicionar os casos erro (verifica configura.txt)                
                }
                if (erro==1){
                        int opcao = 0;
                        printf ("Faça as alteraçoes necessarias! Prima 1 para continuar: \n");//Pede ao utilizador para introduzir uma opcao
                        while(opcao!= 1) {
                                scanf("%d",&opcao);//Le valor introduzido pelo utilizador
                        }
                }
                fclose(ficheiro_configura); //fecha o ficheiro 
        }
       
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


