#include "bibliotecas_estruturas.h"


int fimSimulacao = 0;
/* Escrever no ficheiro E no monitor */
int escreve_monitor_ficheiro(char texto[]){
                FILE *fp;
                fp = fopen("monitor.txt","a"); //Abre o ficheiro no modo de write para escrever no fim
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
        if(apontador == NULL)
                printf("Erro ao abrir o ficheiro");
        else{
                while(!feof(apontador)){ // enquanto não chegar ao fim do ficheiro

                        fgets(linha, MAXSIZE , apontador); // lê apenas uma linha do ficheiro
                        printf(linha); // dá o print dessa mesma linha
                }
                fclose(apontador); // fecha o ficheiro
        }

}

//mostra a informacao no monitor
void mostraInformacao(){
    char textoMonitor[TamLinha];

    //centro de Teste
    escreve_monitor_ficheiro("#####################################\n");
    escreve_monitor_ficheiro("########## Centro de Teste ##########\n");
    escreve_monitor_ficheiro("#####################################\n");
    sprintf(textoMonitor,"Número de casos em estudo: %i\n" ,casosEmEstudo);
    escreve_monitor_ficheiro(textoMonitor);
    sprintf(textoMonitor,"Número de casos positivos: %i\n" ,casosPositivos);
    escreve_monitor_ficheiro(textoMonitor);
    sprintf(textoMonitor,"Número de desistências: %i\n" ,desistenciasTotais);
    escreve_monitor_ficheiro(textoMonitor);
    
    //quantidade de internados
    //tempos médios
    //quantidade de atendidos em cada fila
    //numero de pessoas de risco no centro ##falta comunicar##
    //numero de pessoas normais no centro ##falta comunicar##
    

        
}

//leitura das mensagens
void LerMensagemSimulador(int sockfd){
    char buffer[TamLinha]; //cria um buffer com tamanho 1024
    read(sockfd,buffer,TamLinha); //le a mensagem do socket e guarda
    printf(buffer);
}





//Cria o servidor 
void criaServidor () {
    //sockfd -> criacao para a primeira comunicacao
    //novoSocket -> criacao para a segunda comunicacao
    //tamCLiente-> guarda o tamanho do endereco do cliente
    //tamanhoServidor -> guarda o tamanho do servidor
    int sockfd, novoSocket, tamCliente, tamanhoServidor;
    struct sockaddr_un end_cli , serv_addr;             
	
    //Verifica a criacao do socket
    if ((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0 ){
        printf ( "Erro ao criar o Socket \n");
    }
    
    //Incializa os valores do buffer a zero
    bzero((char*) & serv_addr , sizeof(serv_addr));   
    serv_addr.sun_family = AF_UNIX;                       
    strcpy(serv_addr.sun_path, UNIXSTR_PATH);
    tamanhoServidor = strlen(serv_addr.sun_path) + sizeof(serv_addr.sun_family);    
    unlink(UNIXSTR_PATH);

    //Liga o socket a um endereco
    if( bind(sockfd, (struct sockaddr *) & serv_addr, tamanhoServidor) < 0){
        printf("Erro a ligar o socket a um endereco\n");
    }

    //Espera a conexao com o simulador
    printf("Esperando pelo simulador: \n");
    listen( sockfd , 1 );

    //Criacao de um novo scoket
    tamCliente= sizeof(end_cli);
    novoSocket = accept (sockfd, (struct sockaddr *) &end_cli, &tamCliente);
    if (novoSocket < 0) {                                        //Verifica o erro na aceitacao da ligacao
        printf ("Erro na aceitacao \n");
    }

    //Criação do processo filho
    int pid;
    if ((pid = fork()) < 0) {
        printf ("Erro na criação do processo filho! \n");         //Erro na criacao do processo filho
    }else if (pid == 0) {                                       //Processo filho irá tratar das sucessivas leituras e fecha o socket do processo pai
        close(sockfd);
        LerMensagemSimulador(novoSocket);                       //lê e imprime a mensagem do simulador 
    }

    close(novoSocket);
}

/*                      Main                         */
/*###################################################*/
int main(int argc, char const * argv[]){
    printf ( "########### Bem vindo ########### \n" );              //Menu
    printf ( "       Comecar simulacao          \n" );                  //Menu
    printf ( "################################# \n" );              //Menu

    
    

    int opcao = 0; //opcao escolhida pelo utilzador 
    bool termina = false; 
    while (!termina) 
    {
        //while (opcao !=1)
        //{
            //printf("Escolha uma opção: \n");
            //scanf("%d",&opcao);
        //}
        criaServidor();  //Cria o servidor
        mostraInformacao();  
    }
    
    return 0;
}

/*Adicionar funçoes que imprima a informaçao de tempo medio de estepera;pessoas na fila;pessoas que ja desistiram etc*/