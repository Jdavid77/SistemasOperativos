#include "bibliotecas_estruturas.h"

//configurações
int num_centrosTeste;
int num_pessoas_a_ser_testadas;
int num_medio_testes_por_pessoa;
int idade_media_casos_positivos;
int num_pessoas_risco;
int num_pessoas_normal;
int num_pessoas_simulacao; 


//Probalidades
int prob_desistiu_Risco;
int prob_desistiu_Normal;
int prob_criancas_efetados;
int prob_adultos_efetados;
int prob_idosos_efetados;


main(void){

}

void leConfigura(){
        int erro = 0;
        #define MAXSIZE 512
        char linha[MAXSIZE];
        int valores_configura [12];
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
                        if(valores_configura[0] < 2){
                                printf("Introduziu um valor incorreto na linha 1, o minimo de centros de teste é 2");
                                erro = 1;
                        }
                        else if (valores_configura[1] > 10)
                        {
                                printf("Introduziu um valor incorreto na linha 2, o maximo de pessoas que podem estar a ser testadas é 10");
                                erro = 1;
                        }
                        else if (valores_configura[2] < 0)
                        {
                                printf("Introduziu um valor negativo na linha 3, o numero medio de testes por pessoa tem que ser maior que 0");
                                erro = 1;
                        }
                        else if (valores_configura[3] < 0)
                        {
                                printf("Introduziu um valor negativo na linha 4, a idade media para casos positivos tem que ser maior que 0");
                                erro = 1;
                        }
                        else if (valores_configura[4] < 0)
                        {
                                printf("Introduziu um valor negativo na linha 5, o numero de pessoas de risco na fila tem que ser >= 0");
                                erro = 1;
                        }
                        else if (valores_configura[5] < 0)
                        {
                                printf("Introduziu um valor negativo na linha 6, o numero de pessoas normais na fila tem que ser >= 0");
                                erro = 1;
                        }
                        else if (valores_configura[6] <= 0)//maximo por defenir
                        {
                                printf("Introduziu um valor invalido na linha 7, por favor volte a tentar");
                                erro = 1;
                        }
                        else if (valores_configura[7] < 0 || valores_configura[7] > 100)
                        {
                                printf("Introduziu um valor incorreto na linha 8, a probablidade tem que variar entre 0 e 100");
                                erro = 1;
                        }
                        else if (valores_configura[8] < 0 || valores_configura[8] > 100)
                        {
                                printf("Introduziu um valor incorreto na linha 9, a probablidade tem que variar entre 0 e 100");
                                erro = 1;
                        }
                        else if (valores_configura[9] < 1 || valores_configura[9] > 100)
                        {
                                printf("Introduziu um valor incorreto na linha 10, a probablidade tem que variar entre 1 e 100");
                                erro = 1;
                        }
                        else if (valores_configura[10] < 1 || valores_configura[10] > 100)
                        {
                                printf("Introduziu um valor incorreto na linha 11, a probablidade tem que variar entre 1 e 100");
                                erro = 1;
                        }
                        else if (valores_configura[11] < 1 || valores_configura[11] > 100)
                        {
                                printf("Introduziu um valor incorreto na linha 12, a probablidade tem que variar entre 1 e 100");
                                erro = 1;
                        } 
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
        num_centrosTeste = valores_configura [0];
        num_pessoas_a_ser_testadas = valores_configura [1];
        num_medio_testes_por_pessoa = valores_configura [2];
        idade_media_casos_positivos = valores_configura [3];
        num_pessoas_risco = valores_configura [4];
        num_pessoas_normal = valores_configura [5];
        num_pessoas_simulacao = valores_configura [6];
        prob_desistiu_Risco = valores_configura [7];
        prob_desistiu_Normal = valores_configura [8];
        prob_criancas_efetados = valores_configura [9];
        prob_adultos_efetados = valores_configura [10];
        prob_idosos_efetados = valores_configura [11];
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


