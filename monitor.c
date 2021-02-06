#include "bibliotecas_estruturas.h"
int casosEmEstudo = 0;
int casosPOsitivos = 0;
int desistenciasTotais = 0;
float tempoMedioEsperaFila = 0;
int numTestesFeitos = 0;
int quantidadeDeInternados = 0;
int pessoasRiscoCentro0 = 0;
int pessoasRiscoCentro1 = 0;
int pessoasNormaisCentro0 = 0;
int pessoasNormaisCentro1 = 0;

bool acabou = false;


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


//------------------------leitura das mensagens---------------------------------
void LerMensagemSimulador(int sockfd){
    char buffer[TamLinha]; //cria um buffer com tamanho 1024
    read(sockfd,buffer,TamLinha); //le a mensagem do socket e guarda
    
    if(buffer){
        printf(buffer);
    }
    else{
        printf("erro!");
    }
    
}

void trataInformacao(char mensagem[]){

    //manda a mensagem para o buffer
    char buffer[TamLinha];
    strcpy(buffer,mensagem);
    int i = 0;
    int j;
    //utilizar um token "=" para dividir as frases, do numero que queremos guardar
    char *token = strtok(buffer, "-"); 
   
    if(buffer[0] == 'T')
    {
        j = 0;
        while (token != NULL)
        {
            token = strtok(NULL,buffer);
            j++;

            if(j==1)
            {
                tempoMedioEsperaFila= atof(token);
                break;
            }
        }     
    }
    else if(buffer[0] == 'E')
    {
        j = 0;
        while (token != NULL)
        {
            token = strtok(NULL,buffer);
            j++;

            if(j==1)
            {
                numTestesFeitos= atoi(token);
                break;
            }
        }   
    }
    else if(buffer[0] == 'I')
    {
        j = 0;
        while (token != NULL)
        {
            token = strtok(NULL,buffer);
            j++;

            if(j==1)
            {
                quantidadeDeInternados = atoi(token);
                break;
            }
        }   
    }
    else if(buffer[0] == 'P')
    {
        j = 0;
        while (token != NULL)
        {
            token = strtok(NULL,buffer);
            j++;

            if(j==1)
            {
                casosPOsitivos= atoi(token);
                break;
            }
        }   
    }
    else if(buffer[0] == 'D')
    {
        j = 0;
        while (token != NULL)
        {
            token = strtok(NULL,buffer);
            j++;

            if(j==1)
            {
                desistenciasTotais= atoi(token);
                break;
            }
        }   
    }
    //quando recebe o X é para acabar 
    else if (buffer[0] == 'X')
    {
        acabou = true;
        printf("A simulacão acabou!\n");
    }

    else if(buffer[0] == 'R' && buffer[1]=='0')
    {
        j = 0;
        while (token != NULL)
        {
            token = strtok(NULL,buffer);
            j++;

            if(j==1)
            {
                pessoasRiscoCentro0= atoi(token);
                break;
            }
        }   
    }
    else if(buffer[0] == 'R' && buffer[1]=='1')
    {
        j = 0;
        while (token != NULL)
        {
            token = strtok(NULL,buffer);
            j++;

            if(j==1)
            {
                pessoasRiscoCentro1= atoi(token);
                break;
            }
        }   
    }
    else if(buffer[0] == 'N' && buffer[1]=='0')
    {
        j = 0;
        while (token != NULL)
        {
            token = strtok(NULL,buffer);
            j++;

            if(j==1)
            {
                pessoasNormaisCentro0= atoi(token);
                break;
            }
        }   
    }
    else if(buffer[0] == 'N' && buffer[1]=='1')
    {
        j = 0;
        while (token != NULL)
        {
            token = strtok(NULL,buffer);
            j++;
            if(j==1)
            {
                pessoasNormaisCentro1= atoi(token);
                break;
            }
        }   
    } 
    else if(buffer[0] == 'C' && buffer[1]=='E')
    {
        j = 0;
        while (token != NULL)
        {
            token = strtok(NULL,buffer);
            j++;
            if(j==1)
            {
                casosEmEstudo= atoi(token);
                break;
            }
        }   
    }

}


//-----------------------mostra a informacao no monitor-----------------------------
void mostraInformacao(){
    char textoMonitor[TamLinha];

    //centro de Teste
    escreve_monitor_ficheiro("-------------------------------------\n");
    escreve_monitor_ficheiro("------------ INFORMAÇÕES ------------\n");
    escreve_monitor_ficheiro("-------------------------------------\n");
    //CE-%i
    sprintf(textoMonitor,"Número de casos em estudo: %i\n" ,casosEmEstudo); 
    escreve_monitor_ficheiro(textoMonitor);
    //P-%i
    sprintf(textoMonitor,"Número de casos positivos: %i\n" ,casosPOsitivos); 
    escreve_monitor_ficheiro(textoMonitor);
    // D-%i
    sprintf(textoMonitor,"Número de desistências: %i\n" ,desistenciasTotais); 
    escreve_monitor_ficheiro(textoMonitor);
    //tempos médios T-%f 
    sprintf(textoMonitor,"Tempo médio das filas de espera: %f\n" ,tempoMedioEsperaFila); 
    escreve_monitor_ficheiro(textoMonitor);
    //numero de teste feitos E-%i 
    sprintf(textoMonitor,"Número de testes de covid-19 feitos: %i\n" ,numTestesFeitos); 
    escreve_monitor_ficheiro(textoMonitor);
    //quantidade de internados I-%i 
    sprintf(textoMonitor,"Número de pessoas internadas: %i\n" ,quantidadeDeInternados); 
    escreve_monitor_ficheiro(textoMonitor);
    //numero de pessoas de risco no centro 0 -> R0-%i 
    sprintf(textoMonitor,"Número de pessoas de risco no centro 0: %i\n" ,pessoasRiscoCentro0); 
    escreve_monitor_ficheiro(textoMonitor);
    // no centro 1 -> R1-%i(ler ate ao - )
    sprintf(textoMonitor,"Número de pessoas de risco no centro 1: %i\n" ,pessoasRiscoCentro1); 
    escreve_monitor_ficheiro(textoMonitor);
    //numero de pessoas normais no centro 0 -> N0-%i  
    sprintf(textoMonitor,"Número de pessoas normais no centro 0: %i\n" ,pessoasRiscoCentro0); 
    escreve_monitor_ficheiro(textoMonitor);
    //no centro 1 -> N1-%i
    sprintf(textoMonitor,"Número de pessoas normais no centro 1: %i\n" ,pessoasRiscoCentro1); 
    escreve_monitor_ficheiro(textoMonitor);
    escreve_monitor_ficheiro("-------------------------------------\n");
}





void ReceberMensagens(int sockfd){
    char buffer[TamLinha];
    int leitura = 0;
    //enquanto nao acabar a simulação nao terminar
    while (!acabou)
    {
        //le a mensagem do socket
        leitura = read(sockfd,buffer,TamLinha);
        if(leitura==0) //verifica se chegou ao fim
        {
            break;
        }
        else if (leitura<0) //erro
        {
            printf("erro ao ler o socket\n");
        }
        else
        {
            trataInformacao(buffer);
        }
        mostraInformacao();
    }
    
}





//-----------------------Cria o servidor -------------------------------
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
        ReceberMensagens(novoSocket);                       //lê e imprime a mensagem do simulador 
    }

    close(novoSocket);
}

/* -----------------------Main ---------------------------*/
int main(int argc, char const * argv[]){
    printf ( "---------- Bem vindo ----------\n" );              //Menu
    printf ( "       Começar simulação       \n" );                  //Menu
    printf ( "-------------------------------\n" );              //Menu

    bool termina = false; 
    while (!termina) 
    {
        if(acabou)
        {
            termina = true;
        }
        
        criaServidor();  //Cria o servidor   

    }
    
    return 0;
}

