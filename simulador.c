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

//variaveis globais
char texto[500];
lugarFila = 0;
int horasIsolamento[] = {72,120,48};
int casosPositivos = 0; //pessoas que ja testaram positivo
int casosEmEstudo = 0; //quantidade de testes que estao a ser processados (a espera do resultado)
int desistenciasTotais = 0; //quantidade de pessoas que desistiram da fila

//trincos
pthread_mutex_t trincoCriaPessoa;
//pthread_mutex_init(&criarPessoa,NULL);

//semaforos
sem_t fila; //semaforo da fila do centro 1
sem_t trincoAtendimento; //tranca o atendimento do centro1
sem_t internadosCentros; //numero de internados num centro
sem_t enviamensagem;

//Sockets
int sockfd = 0;


int main(int argc, char const *argv[])
{
        if (pthread_mutex_init(&trincoCriaPessoa, NULL) != 0){
                printf("A inicialização do trinco falhou!");
                return 1;
        }
        sem_init(&fila, 0, 40); //inicializa a fila do centro1 para ser partilhada entre threads(pessoas) com 40 lugares
        sem_init(&trincoAtendimento, 1, 2); //podemos atender ate duas pessoas de cada vez
        sem_init(&internadosCentros, 1, 60); //so podemos ter 60 pesssoas internadas nos centros (total)
        //procurar a explicaçao disto
        sockfd = criarSocket();
        simulacao(sockfd);
        close(sockfd);
        return 1;
}
//atende uma pessoa de risco (sao prints)
void AtendimentoPrioridade(struct pessoa *paciente)
{
        //escreve na consola e tambem no ficheiro que atendeu uma pessoa com prioridade
        //escrevendo o paciente ID faz o teste
        sprintf(texto, "O paciente %i foi atendido com prioridade", paciente->id);
        escreve_ficheiro(texto);
        sprintf(texto, "O paciente %i fez o teste", paciente->id); //o resultado do teste sera posto no centro nao?
        escreve_ficheiro(texto);
}
//atende uma pessoa normal (sao prints)
void AtendimentoNormal(struct pessoa *paciente)
{
        /*escreve na consola e tambem no ficheiro que atendeu uma pessoa
        escrevendo o paciente ID faz o teste */
        sprintf(texto, "O paciente %i foi atendido", paciente->id);
        escreve_ficheiro(texto);
        sprintf(texto, "O paciente %i fez o teste", paciente->id); //o resultado do teste sera posto no centro nao?
        escreve_ficheiro(texto); 
}

void TestaPessoa(struct pessoa *paciente, struct centroDeTeste *centro)
{
        //criar um semaforo que guarda o numero maximo de casos que podem estar em estudo de cada vez
        //da o resultado do teste da pessoa sendo que é um random
        int resTeste = rand() % 1;
        int isolamento = rand() % 1;
        if(resTeste == 0) //o teste deu negativo
        {
                if(isolamento == 1) //se a pessoa  precisar de isolamento (esteve em contacto com infetados)
                {
                        paciente->isolamento = true;
                        //vai ser preciso depois usar isto para o tempo medio de isolamento e tals
                        //escolhe randomly entre se precisa de 72h,48h ou 120h de isolamento
                        int tempoIsolamento = horasIsolamento[rand() % 2];
                        sprintf(texto,"O utilizador %i testou negativo e devera fazer %i horas de isolamento", paciente->id, tempoIsolamento);
                        escreve_ficheiro(texto);
                        casosEmEstudo--;
                }
                else //caso a pessoa nao precise de isolamento
                {
                        sprint(texto,"O utilizador %i testou negativo e foi encaminhado para casa", paciente->id);
                        escreve_ficheiro(texto);
                        casosEmEstudo--;
                }
        }
        else //caso teste positivo
        {
                casosPositivos++;
                paciente->resultadoTeste = true;
                sprinf(texto,"O utilizador %i testou positivo para Covid-19 e vai ser internado", paciente->id);
                escreve_ficheiro(texto);
                sem_wait(&internadosCentros);
                casosEmEstudo--;
        }
}

void trataPessoa(struct pessoa *paciente, struct centroDeTeste *centro)
{
        //usar semaforos para tratar da fila(uma fila é um semaforo)
        //dentro desta funçao verificamos se o paciente tem prioridade, caso tenha entao este é logo atendido
        //caso nao tenha prioridade atendemos um paciente na fila (libertamos um espaço do semaforo)
        //smp que atendemos um paciente damos o output que este foi atendido/fez o teste (atençao de usar trincos aqi pois so podemos atender 1 pessoa de cada vez por centro)
        //apos ser atendido temos que chamar uma funçao que trata do resultado do teste
        int desistiu = rand() % 1; //a pessoa desistiu da fila?
        if(paciente->desistiuFila == false) //se o paciente nao desistiu da fila
        {
                if(paciente->centroTeste == centro->id) //caso a pessoa esteja neste centro de teste
                {
                        if(paciente->prioridade == 1) //se o paciente tiver prioridade(for caso de risco) é logo atenddido
                        {
                                sem_wait(&trincoAtendimento); 
                                casosEmEstudo++;
                                AtendimentoPrioridade(&paciente);
                                sem_post(&trincoAtendimento);
                                sem_post(&fila);
                        }
                        else //caso nao seja paciente de risco (nao tem prioridade)
                        {
                                casosEmEstudo++;
                                sem_wait(&trincoAtendimento);
                                AtendimentoNormal(&paciente); //atende a pessoa e faz o seu teste
                                sem_post(&trincoAtendimento);
                                sem_post(&fila);
                        }
                }
        }
        else //remove alguem da fila caso o paciente tenha desistido
        {
                desistenciasTotais++;
                sprint(texto,"O utilizador %i desistiu da fila", paciente->id);
                escreve_ficheiro(texto);
                sem_post(&fila);
        }        
}

void leConfigura() //é preciso colocar o valores_configura como global?Iremos necessitar desses dados para outras funçoes?
{
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

int escreve_ficheiro(char texto2[])
{
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

//Cria Pessoa
struct pessoa criaPessoa()
{       
        //pthread_mutex_lock(&criarPessoa); //tranca o trinco no inicio da funçao de criar pessoa pois so podemos criar 1 pessoa de cada vez
        int desistiu = rand() % 1;
        struct pessoa paciente;
        paciente.id = rand() % 1000;
        paciente.centroTeste = (rand() % 1 ) + 1;
        paciente.idade = (rand() % 100) + 1;
        if(desistiu == 0)
        {
                paciente.desistiuFila = false;
        }
        else if (desistiu == 1)
        {
                paciente.desistiuFila = true;
        }
        paciente.isolamento = false;
        paciente.prioridade = rand() % 1; //tratar da prioridade da pessoas, ex: ser atendida primeiro, passar à frente da fila etc
        paciente.resultadoTeste = false;
        //pthread_mutext_unlock(&criarPessoa); //destranca o trinco apos criar uma pessoa para poder criar a proxima
        sprintf(texto,"Chegou o utilizador %i", paciente.id);
        escreve_ficheiro(texto);
        //paciente.tempoDeInternamento TEM DE SER QUANDO A PESSOA É TESTADA
        //paciente.tempoEntradaInternamento;
        //paciente.tempoSaidaInternamento
        //criar probabilidades para tratar dos booleanos
}


//cria fila
/*struct fila criaFila(pessoa *paciente)
{
        bool atendidaJa; //variavel para verificar se o paciente deve ser atendido ja ou nao
        struct fila f;
        //f.capacidadeMaximaFila = 40;
        f.tamanhoAtual = f.tamanhoAtual + 1;
        paciente->posicaoAtual = (lugarFila++);
        if(paciente->prioridade == 1){ //se o paciente tiver prioridade(for caso de risco) é logo atenddido
                //escreve na consola e tambem no ficheiro que atendeu uma pessoa com prioridade
                //escrevendo o paciente ID faz o teste
                sprintf(texto, "O paciente %i foi atendido com prioridade", paciente->id);
                escreve_ficheiro(texto);
                sprintf(texto, "O paciente %i fez o teste", paciente->id); //o resultado do teste sera posto no centro nao?
                escreve_ficheiro(texto);
                f.tamanhoAtual--; //tira uma pessoa da fila
        }
        else
        {
                if(paciente->posicaoAtual == 1)
                {
                    AtendimentoNormal(&paciente);
                    f.tamanhoAtual--;    
                }
        }
        if(atendidaJa == true) //o que acontece quando a pessoa é logo atendida?
        {
                

        }
        f.numDesistenciasNormal = 0;
        f.numDesistenciasRisco = 0; //vai incrementando durante a simulação à medida que vao saindo
        //f.tempoMedioEspera criar funcao para tratar dos tempos de espera
}*/

//Cria Centro de Teste
struct centroDeTeste criaCentroDeTeste()
{
        struct centroDeTeste centro;
        centro.capacidadeMaxima = 100;
        centro.id = (rand() % 1)+1;
        centro.capacidaMaximaInternamento = 60;
        //centro.tempoMedioInternamento tem de ser calculada a media (pelo tempo de entrada e saida do paciente)
        centro.tempoMaximoInternamento = 40;
        centro.tempoMaximoIsolamento = 30;
        //centro.tempoMedioIsolamento é calculado    
};


//Tarefa Pessoa
void *Pessoa(void *ptr)
{
        pthread_mutex_lock(&trincoCriaPessoa); //criar um pessoa de cada vez 
        struct pessoa person = criaPessoa();
        //fila(&person);    
        sem_wait(&fila);
        pthread_mutex_unlock(&trincoCriaPessoa); 
        return NULL;                                                                                                              

}

//Socket
int criarSocket(){
    //Variaveis
    struct sockaddr_un end_serv;
    int tamanhoServidor;

    //#############Cria o socket##################
    int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

    if(sockfd < 0) {                                                                         
        printf("Erro: socket nao foi criado \n");
    }

    //Colocar o socket a zero
    bzero((char *) &end_serv, sizeof(end_serv));
    
    //Dominio do socket
    end_serv.sun_family = AF_UNIX;
    strcpy(end_serv.sun_path, UNIXSTR_PATH);
    tamanhoServidor = strlen(end_serv.sun_path) + sizeof( end_serv.sun_family);

    // Estabelece a ligacao com o socket
    int varprint = 0;
    while( connect( sockfd, (struct sockaddr *) &end_serv, tamanhoServidor) < 0) {
        if (varprint==0){
            printf("Espera pelo monitor.\n"); 
            varprint = 1;
        }
    }
    printf("Simulador pronto.\n"); 
    return sockfd;
}

//envia mensagens entre servidor e cliente
void EnviarMensagens(char * t, int sockfd){
        char mensagem[TamLinha];
        //copiar para dentro do buffer a mensagem
        strcpy(mensagem,t); //recebe o array e mensagem
        write(sockfd,mensagem,strlen(mensagem)); //clienteSOcket, mensagem, tamanho do array mensagem
}





