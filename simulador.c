#include "bibliotecas_estruturas.h"

//configurações
int num_centrosTeste;
int num_pessoas_a_ser_testadas;
int num_medio_testes_por_pessoa;
int num_maximo_pessoas_fila;
int maximo_casos_em_estudo;
int num_pessoas_normal;
int num_pessoas_simulacao;
double temposimulacao;
int num_maximo_internados;

//Probalidades
int prob_desistiu_Risco;
int prob_desistiu_Normal;
int prob_criancas_efetados;
int prob_adultos_efetados;
int prob_idosos_efetados;
int prob_risco_infetado;
int prob_ser_de_risco;

char texto[500];

char mensagem[100];


//semaforos
//sem_t semafila0;              //semaforo da fila do centro 0
sem_t semafila;               //semaforo da fila do centro 1
sem_t semaAtendimento;        //tranca o atendimento do centro1
sem_t semainternadosCentros0; //numero de internados num centro
sem_t semainternadosCentros1;
sem_t trincoEnviamensagem;
sem_t trincoCriaPessoa;
sem_t trincoTarefaPessoa;
sem_t trincoFila;
sem_t trincoPessoaNaFila;
sem_t semamaximoCasosEstudo;
sem_t trincoAtendimentoPrioritario;
sem_t trincoCasosEmEstudo;
sem_t trincoDesistenciasTotais;
sem_t trincoTotalInternados;
sem_t trincoCasosPositivos;
sem_t trincoAtendimentos0;
sem_t trincoAtendimentos1;
sem_t trincoTotalTestes;
sem_t trincoVoltouTestar;
//tempo
clock_t tempo_inicial_fila, tempo_final_fila;

//Sockets
int sockfd = 0;

//variaveis globais
int total_internados = 0;
//int lugarFila = 0;
int horasIsolamento[] = {72, 120, 48};
int casosPositivos = 0;     //pessoas que ja testaram positivo
int casosEmEstudo = 0;      //quantidade de testes que estao a ser processados (a espera do resultado)
int desistenciasTotais = 0; //quantidade de pessoas que desistiram da fila
int id_pessoa = 0;
int pessoasAtendidasCentro1 = 0;
int pessoasAtendidasCentro0 = 0;
float tempoMedioNaFila = 0;
int Totaltestes = 0;
int VoltouTestar = 0;
//tarefa pessoa
pthread_t id_tarefa_pessoa[400];

//atende uma pessoa de risco (sao prints)
void AtendimentoPrioridade(struct pessoa *paciente)
{
        //escreve na consola e tambem no ficheiro que atendeu uma pessoa com prioridade
        //escrevendo o paciente ID faz o teste

        sprintf(texto, "O paciente com o número %i foi atendido com prioridade \n", paciente->id);
        escreve_ficheiro(texto);
        sprintf(texto, "O paciente com o número %i fez o teste \n", paciente->id); //o resultado do teste sera posto no centro nao?
        escreve_ficheiro(texto);

        if (paciente->centroTeste == 0)
        {

                sem_wait(&trincoAtendimentos0);
                pessoasAtendidasCentro0++;
                sem_post(&trincoAtendimentos0);
                sprintf(mensagem,"K-%i",pessoasAtendidasCentro0);
                EnviarMensagens(mensagem,sockfd);
        }
        if (paciente->centroTeste == 1)
        {

                sem_wait(&trincoAtendimentos1);
                pessoasAtendidasCentro1++;
                sem_post(&trincoAtendimentos1);
                sprintf(mensagem,"L-%i",pessoasAtendidasCentro1);
                EnviarMensagens(mensagem,sockfd);
        }
        sem_wait(&trincoTotalTestes);
        Totaltestes++;
        sem_post(&trincoTotalTestes);
        sprintf(mensagem, "E-%i",Totaltestes);
        EnviarMensagens(mensagem,sockfd);
        sem_wait(&trincoCasosEmEstudo);
        casosEmEstudo++;
        sem_post(&trincoCasosEmEstudo);
        sprintf(mensagem, "CE-%i", casosEmEstudo);
        EnviarMensagens(mensagem, sockfd);

        sem_wait(&semamaximoCasosEstudo);
}
//atende uma pessoa normal
void AtendimentoNormal(struct pessoa *paciente)
{
        /*escreve na consola e tambem no ficheiro que atendeu uma pessoa
        escrevendo o paciente ID faz o teste */

        sprintf(texto, "O paciente com o número %i foi atendido \n", paciente->id);
        escreve_ficheiro(texto);
        sprintf(texto, "O paciente com o número %i fez o teste \n", paciente->id);
        escreve_ficheiro(texto);

        if (paciente->centroTeste == 0)
        {
                sem_wait(&trincoAtendimentos0);
                pessoasAtendidasCentro0++;
                sem_post(&trincoAtendimentos0);
                sprintf(mensagem, "K-%i", pessoasAtendidasCentro0);
                EnviarMensagens(mensagem, sockfd);
        }
        if (paciente->centroTeste == 1)
        {

                sem_wait(&trincoAtendimentos1);
                pessoasAtendidasCentro1++;
                sem_post(&trincoAtendimentos1);
                sprintf(mensagem, "L-%i", pessoasAtendidasCentro1);
                EnviarMensagens(mensagem, sockfd);
        }
        sem_wait(&trincoTotalTestes);
        Totaltestes++;
        sem_post(&trincoTotalTestes);
        sprintf(mensagem, "E-%i",Totaltestes);
        EnviarMensagens(mensagem,sockfd);
        sem_wait(&trincoCasosEmEstudo);
        casosEmEstudo++;
        sem_post(&trincoCasosEmEstudo);
        sprintf(mensagem, "CE-%i", casosEmEstudo);
        EnviarMensagens(mensagem, sockfd);

        sem_wait(&semamaximoCasosEstudo);
}

void TestaPessoa(struct pessoa *paciente)
{
        //da o resultado do teste da pessoa sendo que é um random
        int infetado_crianca = rand() % 101;
        int infetado_adulto = rand() % 101;
        int infetado_idoso = rand() % 101;

        if (paciente->idade < 18) //caso seja uma criança
        {
                //verifica atraves da probabilidade se a pessoa esta infetada ou nao
                if (infetado_crianca > prob_criancas_efetados)
                {
                        paciente->resultadoTeste = false;
                }
                if (infetado_crianca <= prob_criancas_efetados)
                {
                        paciente->resultadoTeste = true;
                }
        }
        if (paciente->idade < 60 && paciente->idade >= 18) //caso seja um adulto
        {
                //verifica atraves da probabilidade se a pessoa esta infetada ou nao
                if (infetado_adulto > prob_adultos_efetados)
                {
                        paciente->resultadoTeste = false;
                }
                if (infetado_adulto <= prob_adultos_efetados)
                {
                        paciente->resultadoTeste = true;
                }
        }
        if (paciente->idade >= 60) //caso seja um idoso
        {
                //verifica atraves da probabilidade se a pessoa esta infetada ou nao
                if (infetado_idoso > prob_idosos_efetados)
                {
                        paciente->resultadoTeste = false;
                }
                if (infetado_idoso <= prob_idosos_efetados)
                {
                        paciente->resultadoTeste = true;
                }
        }
        if (!paciente->resultadoTeste) //o teste deu negativo
        {
                paciente->num_testes++;
                sprintf(texto, "O paciente com o número %i testou negativo e foi encaminhado para casa \n", paciente->id);
                printf(texto);
                escreve_ficheiro(texto);
        }
        if (paciente->resultadoTeste) //caso teste positivo
        {
                paciente->num_testes++;
                sem_wait(&trincoCasosPositivos);
                casosPositivos++;
                sem_post(&trincoCasosPositivos);
                sprintf(mensagem, "P-%i", casosPositivos);
                EnviarMensagens(mensagem, sockfd);

                if (paciente->prioridade == 1) //caso tenha prioridade (for um caso de risco)
                {
                        sprintf(texto, "O paciente com o número %i testou positivo para Covid-19 e vai ser internado \n", paciente->id);
                        printf(texto);
                        escreve_ficheiro(texto);
                        sem_wait(&trincoTotalInternados);
                        total_internados++;
                        sem_post(&trincoTotalInternados);
                        sprintf(mensagem, "I-%i", total_internados);
                        EnviarMensagens(mensagem, sockfd);
                        if (paciente->centroTeste == 0)
                        {
                                sem_wait(&semainternadosCentros0); //ocupa um lugar no internamento
                        }
                        else
                        {
                                sem_wait(&semainternadosCentros1); //ocupa um lugar no internamento
                        }

                        //casosEmEstudo--;
                }
                if (paciente->prioridade == 0) //caso nao tenha prioridade
                {
                        sprintf(texto, "O paciente com o número %i testou positivo para Covid-19 e vai para isolamento durante 72h \n", paciente->id);
                        printf(texto);
                        escreve_ficheiro(texto);
                        //casosEmEstudo--;
                }
        }
        sem_wait(&trincoCasosEmEstudo);
        casosEmEstudo--;
        sem_post(&trincoCasosEmEstudo);
        sprintf(mensagem, "CE-%i", casosEmEstudo);
        EnviarMensagens(mensagem, sockfd);
        sem_post(&semamaximoCasosEstudo);

        //para a pessoa fazer o numero teste desejados
        if (paciente->num_testes != paciente->testesDesejados)
        {
                usleep(1000000);
                sprintf(texto, "O paciente com o número %i vai voltar a fazer outro teste \n", paciente->id);
                printf(texto);
                escreve_ficheiro(texto);
                sem_wait(&trincoVoltouTestar);
                VoltouTestar++;
                sem_post(&trincoVoltouTestar);
                sprintf(mensagem,"R-%i",VoltouTestar);
                EnviarMensagens(mensagem,sockfd);
                trataPessoa(paciente);
        }
}

//pessoa realiza o teste
void trataPessoa(struct pessoa *paciente)
{
        sem_wait(&semafila);

        //começa a contar o tempo dessa pessoa na fila
        tempo_inicial_fila = clock();

        //usar semaforos para tratar da fila(uma fila é um semaforo)
        //dentro desta funçao verificamos se o paciente tem prioridade, caso tenha entao este é logo atendido
        //caso nao tenha prioridade atendemos um paciente na fila (libertamos um espaço do semaforo)
        //smp que atendemos um paciente damos o output que este foi atendido/fez o teste (atençao de usar trincos aqi pois so podemos atender 1 pessoa de cada vez por centro)
        //apos ser atendido temos que chamar uma funçao TestaPessoa que trata do resultado do teste
        if (paciente->desistiuFila == false) //se o paciente nao desistiu da fila
        {

                if (paciente->prioridade == 1) //se o paciente tiver prioridade(for caso de risco) é logo atenddido
                {
                        sem_wait(&trincoAtendimentoPrioritario);
                        sprintf(texto, "Pessoa com risco está na fila de espera, no centro %i.\n", paciente->centroTeste);
                        printf(texto);
                        escreve_ficheiro(texto);
                        AtendimentoPrioridade(paciente);
                        sem_post(&trincoAtendimentoPrioritario);
                        tempo_final_fila = clock();

                        //tempo na fila
                        float tempoNaFila;

                        tempoNaFila = ((float)(tempo_final_fila - tempo_inicial_fila) / CLOCKS_PER_SEC) * 10000;
                        tempoMedioNaFila += tempoNaFila / (pessoasAtendidasCentro1 + pessoasAtendidasCentro0);

                        //############################
                        //passa o tempo na fila para ser calculado o tempo medio no monitor
                        sprintf(mensagem, "T-%f", tempoMedioNaFila);
                        //envia para o monitor
                        EnviarMensagens(mensagem, sockfd);
                        sprintf(texto, "A paciente com o número %i esperou %f\n", paciente->id, tempoNaFila);
                        printf(texto);
                        escreve_ficheiro(texto);
                        //com prioridade
                        /**if (paciente->centroTeste == 0)
                        {
                                sem_wait(&semafila0);
                                /**sem_wait(&trincoPessoasRiscoFila0);
                                num_pessoas_risco_fila_centro0--;
                                sem_post(&trincoPessoasRiscoFila0);
                                sprintf(mensagem, "R0-%i", num_pessoas_risco_fila_centro0);
                                EnviarMensagens(mensagem, sockfd);
                        }
                        else if (paciente->centroTeste == 1)
                        {
                                sem_wait(&semafila1);
                                /**sem_wait(&trincoPessoasRiscoFila1);
                                num_pessoas_risco_fila_centro1--;
                                sem_post(&trincoPessoasRiscoFila1);
                                sprintf(mensagem, "R1-%i", num_pessoas_risco_fila_centro1);
                                EnviarMensagens(mensagem, sockfd);
                        }*/

                        usleep(1000000);
                        TestaPessoa(paciente);
                }
                else //caso nao seja paciente de risco (nao tem prioridade)
                {

                        sem_wait(&semaAtendimento);
                        sprintf(texto, "Pessoa normal está na fila de espera, no centro %i.\n", paciente->centroTeste);
                        printf(texto);
                        escreve_ficheiro(texto);
                        AtendimentoNormal(paciente); //atende a pessoa e faz o seu teste
                        sem_post(&semaAtendimento);
                        tempo_final_fila = clock();

                        //tempo na fila
                        float tempoNaFila;
                        tempoNaFila = ((float)(tempo_final_fila - tempo_inicial_fila) / CLOCKS_PER_SEC) * 10000;
                        tempoMedioNaFila += tempoNaFila / (pessoasAtendidasCentro1 + pessoasAtendidasCentro0);
                        ////passa o tempo na fila para ser calculado o tempo medio no monitor
                        sprintf(mensagem, "T-%f", tempoMedioNaFila);
                        //envia para o monitor
                        EnviarMensagens(mensagem, sockfd);
                        sprintf(texto, "A paciente com o número %i esperou %f\n", paciente->id, tempoNaFila);
                        printf(texto);
                        escreve_ficheiro(texto);
                        //sem prioridade
                        /**if (paciente->centroTeste == 0)
                        {
                                sem_wait(&semafila0);
                                /**sem_wait(&trincoPessoasNormaisFila0);
                                num_pessoas_normais_fila_centro0--;
                                sem_post(&trincoPessoasNormaisFila0);
                                sprintf(mensagem, "N0-%i", num_pessoas_normais_fila_centro0);
                                EnviarMensagens(mensagem, sockfd);/
                        }
                        else if (paciente->centroTeste == 1)
                        {
                                sem_wait(&semafila1);
                                /**sem_wait(&trincoPessoasNormaisFila1);
                                num_pessoas_normais_fila_centro1--;
                                sem_post(&trincoPessoasNormaisFila1);
                                sprintf(mensagem, "N1-%i", num_pessoas_normais_fila_centro1);
                                EnviarMensagens(mensagem, sockfd);/
                        }*/
                        usleep(1000000);
                        TestaPessoa(paciente);
                }
                //}
        }
        else //remove alguem da fila caso o paciente tenha desistido
        {
                sem_wait(&trincoDesistenciasTotais);
                desistenciasTotais++;
                sem_post(&trincoDesistenciasTotais);
                sprintf(mensagem, "D-%i", desistenciasTotais);
                EnviarMensagens(mensagem, sockfd);

                /**if (paciente->prioridade == 1)
                {
                        if (paciente->centroTeste == 0)
                        {
                                sem_wait(&semafila0);
                                /**sem_wait(&trincoPessoasRiscoFila0);
                                num_pessoas_risco_fila_centro0--;
                                sem_post(&trincoPessoasRiscoFila0);
                                sprintf(mensagem, "R0-%i", num_pessoas_risco_fila_centro0);
                                EnviarMensagens(mensagem, sockfd);/
                        }
                        else if (paciente->centroTeste == 1)
                        {
                                sem_wait(&semafila1);
                                /**sem_wait(&trincoPessoasRiscoFila1);
                                num_pessoas_risco_fila_centro1--;
                                sem_post(&trincoPessoasRiscoFila1);
                                sprintf(mensagem, "R1-%i", num_pessoas_risco_fila_centro1);
                                EnviarMensagens(mensagem, sockfd);*
                        }
                }
                else if (paciente->prioridade == 0)
                {
                        if (paciente->centroTeste == 0)
                        {
                                sem_wait(&semafila0);
                                /**sem_wait(&trincoPessoasNormaisFila0);
                                num_pessoas_normais_fila_centro0--;
                                sem_post(&trincoPessoasNormaisFila0);
                                sprintf(mensagem, "N0-%i", num_pessoas_normais_fila_centro0);
                                EnviarMensagens(mensagem, sockfd);/
                        }
                        else if (paciente->centroTeste == 1)
                        {
                                sem_wait(&semafila1);
                                /**sem_wait(&trincoPessoasNormaisFila1);
                                num_pessoas_normais_fila_centro1--;
                                sem_post(&trincoPessoasNormaisFila1);
                                sprintf(mensagem, "N1-%i", num_pessoas_normais_fila_centro1);
                                EnviarMensagens(mensagem, sockfd);/
                        }
                }*/
                sprintf(texto, "O paciente com o número %i desistiu da fila \n", paciente->id);
                printf(texto);
                escreve_ficheiro(texto);
        }
        sem_post(&semafila);
}

void leConfigura()
{
        int erro = 1;
        #define MAXSIZE 512
        char linha[MAXSIZE];
        char configura[] = "server.config";
        int valores_configura[11];
        while (erro == 1)
        {
                erro = 0;
                FILE *ficheiro_configura;
                ficheiro_configura = fopen(configura, "r"); //Abre o ficheiro no modo de read
                if (ficheiro_configura == NULL)
                {
                        printf("Erro ao abrir o ficheiro");
                }
                else
                {
                        int j = 0;
                        while (!feof(ficheiro_configura))
                        {                                                  // enquanto não chegar ao fim do ficheiro
                                fgets(linha, MAXSIZE, ficheiro_configura); // lê apenas uma linha do ficheiro

                                for (int i = 0; i < 4; i++)
                                { //le os primeiros 4 carateres da linha
                                        if (linha[i] == '=')
                                        { //caso seja "="
                                                break;
                                        }
                                        else
                                        {
                                                if (isalpha(linha[i]))
                                                {                                                                                            //verfica se é carater alfabeto A a Z
                                                        printf("ERRO! Inseriu um valor com o carater '%c' na linha %i \n", linha[i], j + 1); //j+1 é numero da linha  e linha[i] é o carater
                                                        erro = 1;
                                                }
                                        }
                                }
                                char *token = strtok(linha, "=");   //utilizar um token "=" para dividir as frases, do numero que queremos guardar
                                valores_configura[j] = atoi(token); //passa o valor para o array
                                j++;
                        } //adicionar os casos erro (verifica configura.txt)
                        /*
                        if (valores_configura[0] < 2)
                        {
                                printf("Introduziu um valor incorreto na linha 1, o minimo de centros de teste é 2");
                                erro = 1;
                        }*/
                        if (valores_configura[0] < 2)
                        {
                                printf("Introduziu um valor incorreto na linha 2, o minimo de pessoas que podem estar a ser testadas é >= 2");
                                erro = 1;
                        }
                        // else if (valores_configura[2] < 0)
                        // {
                        //         printf("Introduziu um valor negativo na linha 3, o numero medio de testes por pessoa tem que ser maior que 0");
                        //         erro = 1;
                        // }
                        else if (valores_configura[1] < 0)
                        {
                                printf("Introduziu um valor negativo na linha 3, o máximo de pessoas na fila de espera tem de se maior que 0");
                                erro = 1;
                        }
                        else if (valores_configura[2] < 0)
                        {
                                printf("Introduziu um valor negativo na linha 4, o número maximo de casos em estudo tem que ser >= 0");
                                erro = 1;
                        }
                        else if (valores_configura[3] < 0)
                        {
                                printf("Introduziu um valor negativo na linha 5, o número máximo de internados num centro tem que ser >= 0");
                                erro = 1;
                        }
                        else if (valores_configura[4] <= 0) //maximo por defenir
                        {
                                printf("Introduziu um valor invalido na linha 6, por favor volte a tentar");
                                erro = 1;
                        }
                        else if (valores_configura[5] < 0 || valores_configura[5] > 100)
                        {
                                printf("Introduziu um valor incorreto na linha 7, a probablidade tem que variar entre 0 e 100");
                                erro = 1;
                        }
                        else if (valores_configura[6] < 0 || valores_configura[6] > 100)
                        {
                                printf("Introduziu um valor incorreto na linha 8, a probablidade tem que variar entre 0 e 100");
                                erro = 1;
                        }
                        else if (valores_configura[7] < 1 || valores_configura[7] > 100)
                        {
                                printf("Introduziu um valor incorreto na linha 9, a probablidade tem que variar entre 1 e 100");
                                erro = 1;
                        }
                        else if (valores_configura[8] < 1 || valores_configura[8] > 100)
                        {
                                printf("Introduziu um valor incorreto na linha 10, a probablidade tem que variar entre 1 e 100");
                                erro = 1;
                        }
                        else if (valores_configura[9] < 1 || valores_configura[9] > 100)
                        {
                                printf("Introduziu um valor incorreto na linha 11, a probablidade tem que variar entre 1 e 100");
                                erro = 1;
                        }
                        else if (valores_configura[10] < 1 || valores_configura[10] > 100)
                        {
                                printf("Introduziu um valor incorreto na linha 12, a probablidade tem que variar entre 1 e 100");
                                erro = 1;
                        }
                }
                if (erro == 1)
                {
                        int opcao = 0;
                        printf("Faça as alteraçoes necessarias! Prima 1 para continuar: \n"); //Pede ao utilizador para introduzir uma opcao
                        while (opcao != 1)
                        {
                                scanf("%d", &opcao); //Le valor introduzido pelo utilizador
                        }
                }
                fclose(ficheiro_configura); //fecha o ficheiro
        }
        
        num_pessoas_a_ser_testadas = valores_configura[0];
        num_maximo_pessoas_fila = valores_configura[1];
        maximo_casos_em_estudo = valores_configura[2];
        num_maximo_internados = valores_configura[3];
        num_pessoas_simulacao = valores_configura[4];
        prob_desistiu_Risco = valores_configura[5];
        prob_desistiu_Normal = valores_configura[6];
        prob_criancas_efetados = valores_configura[7];
        prob_adultos_efetados = valores_configura[8];
        prob_idosos_efetados = valores_configura[9];
        prob_ser_de_risco = valores_configura[10];
        
}

int escreve_ficheiro(char texto2[])
{
        FILE *fp;
        fp = fopen("simulador.txt", "a"); //Abre o ficheiro no modo de write para escrever no fim
        if (fp == NULL)
        {
                printf("Erro ao abrir o ficheiro");
        }
        else
        {
                fprintf(fp, texto2); //Escreve o texto no ficheiro
                fclose(fp);          //fecha o ficheiro
                return 0;            //retorna 0 para indicar que funcionou corretamente
        }
}

//Cria Pessoa
struct pessoa criaPessoa()
{

        sem_wait(&trincoCriaPessoa); //tranca o trinco
        //printf("entrou no cria pessoa \n");
        int desistiu = rand() % 101;
        struct pessoa paciente;
        paciente.id = id_pessoa;
        id_pessoa++;
        paciente.centroTeste = (rand() % 2);
        paciente.idade = (rand() % 100) + 1;
        int ser_risco = rand() % 101;
        paciente.testesDesejados = (rand() % 2) + 1;
        //tratar da prioridade da pessoas, ex: ser atendida primeiro, passar à frente da fila etc
        if (ser_risco > prob_ser_de_risco)
        {
                paciente.prioridade = 1;
        }
        if (ser_risco <= prob_ser_de_risco)
        {
                paciente.prioridade = 0;
        }
        if (paciente.prioridade == 0) // caso nao tenha prioridade
        {
                /**if (paciente.centroTeste == 0)
                {
                        sem_wait(&semafila0);
                        /**sem_wait(&trincoPessoasNormaisFila0);
                        num_pessoas_normais_fila_centro0++;
                        sem_post(&trincoPessoasNormaisFila0);
                        sprintf(mensagem, "N0-%i", num_pessoas_normais_fila_centro0);
                        EnviarMensagens(mensagem, sockfd);/
                }
                if (paciente.centroTeste == 1)
                {
                        sem_wait(&semafila1);
                        /**sem_wait(&trincoPessoasNormaisFila1);
                        num_pessoas_normais_fila_centro1++;
                        sem_post(&trincoPessoasNormaisFila1);
                        sprintf(mensagem, "N1-%i", num_pessoas_normais_fila_centro1);
                        EnviarMensagens(mensagem, sockfd);/
                }*/

                if (desistiu > prob_desistiu_Normal)
                {
                        paciente.desistiuFila = false;
                }
                if (desistiu <= prob_desistiu_Normal)
                {
                        paciente.desistiuFila = true;
                }
        }
        if (paciente.prioridade == 1) //caso tenha prioridade
        {

                /**if (paciente.centroTeste == 0)
                {
                        sem_wait(&semafila0);
                        /**sem_wait(&trincoPessoasRiscoFila0);
                        num_pessoas_risco_fila_centro0++;
                        sem_post(&trincoPessoasRiscoFila0);
                        sprintf(mensagem, "R0-%i", num_pessoas_risco_fila_centro0);
                        EnviarMensagens(mensagem, sockfd);/
                }
                if (paciente.centroTeste == 1)
                {
                        sem_wait(&semafila1);
                        /**sem_wait(&trincoPessoasRiscoFila1);
                        num_pessoas_risco_fila_centro1++;
                        sem_post(&trincoPessoasRiscoFila1);
                        sprintf(mensagem, "R1-%i", num_pessoas_risco_fila_centro1);
                        EnviarMensagens(mensagem, sockfd);/
                }*/

                if (desistiu > prob_desistiu_Risco)
                {
                        paciente.desistiuFila = false;
                }
                if (desistiu <= prob_desistiu_Risco)
                {
                        paciente.desistiuFila = true;
                }
        }
        paciente.num_testes = 0;
        paciente.isolamento = false;
        paciente.resultadoTeste = false;

        usleep(100000);
        sprintf(texto, "Chegou o paciente com o número %i ao centro %i\n", paciente.id, paciente.centroTeste);
        printf(texto);
        escreve_ficheiro(texto);
        sem_post(&trincoCriaPessoa); //destranca o trinco apos criar uma pessoa para poder criar a proxima
        return paciente;
}

//Cria Centro de Teste
struct centroDeTeste criaCentroDeTeste(int idCentro)
{
        struct centroDeTeste centro;

        centro.capacidadeMaxima = 100;
        centro.id = idCentro;
        centro.capacidaMaximaInternamento = 60;
        centro.tempoMaximoInternamento = 40;
        centro.tempoMaximoIsolamento = 30;
        
};

//Tarefa Pessoa
void Pessoa(void *ptr)
{
        
        sem_wait(&trincoTarefaPessoa);
        struct pessoa person = criaPessoa();
        sem_post(&trincoTarefaPessoa);
        //########### necessario colocar a pessoa na fila de espera
        //fila(&person);
        //sem_wait(&semafila0);
        usleep(1000000);
        trataPessoa(&person);
        
}

//Socket
int criarSocket()
{
        //Variaveis
        struct sockaddr_un end_serv;
        int tamanhoServidor;

        //#############Cria o socket##################
        int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

        if (sockfd < 0)
        {
                printf("Erro: socket nao foi criado \n");
        }

        //Colocar o socket a zero
        bzero((char *)&end_serv, sizeof(end_serv));

        //Dominio do socket
        end_serv.sun_family = AF_UNIX;
        strcpy(end_serv.sun_path, UNIXSTR_PATH);
        tamanhoServidor = strlen(end_serv.sun_path) + sizeof(end_serv.sun_family);

        // Estabelece a ligacao com o socket
        int varprint = 0;
        while (connect(sockfd, (struct sockaddr *)&end_serv, tamanhoServidor) < 0)
        {
                if (varprint == 0)
                {
                        printf("Espera pelo monitor.\n");
                        varprint = 1;
                }
        }
        printf("Simulador pronto.\n");
        return sockfd;
}

//envia mensagens entre servidor e cliente
void EnviarMensagens(char *t, int sockfd)
{

        //###########fecha o trinco###########
        sem_wait(&trincoEnviamensagem);
        char mensagem[TamLinha];
        //copiar para dentro do buffer a mensagem
        //strcpy(mensagem, t);                       //recebe o array e mensagem
        //write(sockfd, mensagem, strlen(mensagem)); //clienteSOcket, mensagem, tamanho do array mensagem

        int var;

        if (strcpy(mensagem, t) != 0)
        {
                var = strlen(mensagem) + 1;
                if (write(sockfd, mensagem, var) != var)
                {
                        printf("Erro cliente no write! \n");
                }
        }

        usleep(2000);

        //###########abre o trinco ###########
        sem_post(&trincoEnviamensagem);
}

void simula(int sockfd)
{

        srand(time(NULL));
        inicializa();
        //cria as tarefas pessoa
        for (int i = 0; i < num_pessoas_simulacao; i++)
        {
                pthread_create(&id_tarefa_pessoa[i], NULL, Pessoa, NULL);

        }

        for (int i = 0; i < num_pessoas_simulacao; i++)
        {
                pthread_join(id_tarefa_pessoa[i], NULL);
        }

        //acabou a simulação
        EnviarMensagens("X", sockfd);
}
void inicializa()
{
        //printf("entrou no inicializa");
        leConfigura();
        sem_init(&semafila, 0, num_maximo_pessoas_fila);             //inicializa a fila do centro1 para ser partilhada entre threads(pessoas) com 40 lugares
        sem_init(&semaAtendimento, 0, num_pessoas_a_ser_testadas);   //podemos atender no minimo duas pessoas de cada vez
        sem_init(&semainternadosCentros0, 0, num_maximo_internados); //so podemos ter 60 pesssoas internadas nos centros (total)
        sem_init(&semainternadosCentros1, 0, num_maximo_internados);
        sem_init(&trincoCriaPessoa, 0, 1);
        sem_init(&trincoEnviamensagem, 0, 1);
        sem_init(&trincoTarefaPessoa, 0, 1);
        sem_init(&semamaximoCasosEstudo, 0, maximo_casos_em_estudo);
        sem_init(&trincoAtendimentoPrioritario, 0, 1);
        sem_init(&trincoCasosEmEstudo, 0, 1);
        sem_init(&trincoDesistenciasTotais, 0,1);
        sem_init(&trincoTotalInternados,0,1);
        sem_init(&trincoCasosPositivos,0,1);
        sem_init(&trincoTotalTestes,0,1);
        sem_init(&trincoVoltouTestar,0,1);
        /**sem_init(&trincoPessoasNormaisFila0,0,1);
        sem_init(&trincoPessoasRiscoFila0,0,1);
        sem_init(&trincoPessoasNormaisFila1,0,1);
        sem_init(&trincoPessoasRiscoFila1,0,1);*/
        sem_init(&trincoAtendimentos1,0,1);
        sem_init(&trincoAtendimentos0,0,1);
        //sem_init(&trincoPessoaNaFila,0,1);
        criaCentroDeTeste(0);
        criaCentroDeTeste(1);
}

int main(int argc, char const *argv[])
{
        sockfd = criarSocket();
        simula(sockfd);
        close(sockfd);
        return 1;
}