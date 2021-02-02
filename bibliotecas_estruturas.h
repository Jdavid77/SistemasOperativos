#include <stdio.h>          //Biblioteca geral de input e outputs da linguagem C
#include <stdlib.h>         //Biblioteca padrão do C
#include <sys/types.h>      //Contem definicoes de data types usados nos system calls
#include <sys/stat.h>
#include <fcntl.h>       
#include <sys/socket.h>     //Inclui definicoes necessarias para trabalhar com sockets
#include <sys/un.h>         //contem estruturas e constantes necessarias para o dominio do endereco
#include <time.h>            //biblioteca para trabalhar com  o tempo
#include <stdbool.h>        //Para trabalhar com boleanos
#include <pthread.h>
#include <semaphore.h>

#define TamLinha 1024       //tamanho maximo do buffer
#define UNIXSTR_PATH  "/tmp/SistemasOperativos"

//variaveis globais
int lugarFila = 0;
int horasIsolamento[] = {72,120,48};
int casosPositivos = 0; //pessoas que ja testaram positivo
int casosEmEstudo = 0; //quantidade de testes que estao a ser processados (a espera do resultado)
int desistenciasTotais = 0; //quantidade de pessoas que desistiram da fila
int id_pessoa = 0;
int pessoasAtendidasCentro1 = 0;
int pessoasAtendidasCentro0 = 0;


struct pessoa  
{
    int id;
    int num_testes;
    int idade; 
    int prioridade;
    bool resultadoTeste;
    int posicaoFila;
    float tempoEntradaInternamento;
    float tempoSaidaInternamento;
    float tempoDeInternamento;
    bool desistiuFila;
    bool isolamento;
    int centroTeste;

};

struct centroDeTeste
{
    int id;
    int capacidadeMaxima; //total de pessoas fila + internados
    int capacidaMaximaInternamento; //total de pessoas internadas
    float tempoMaximoInternamento;
    float tempoMedioInternamento;
    float tempoMaximoIsolamento;
    float tempoMedioIsolamento;
    
};

struct fila
{   
    int idCentro;
    float tempoMedioEspera;
    int capacidadeMaximaFila;
    int tamanhoAtual; //tamanho atual da fila
    int numDesistenciasRisco;
    int numDesistenciasNormal;
};
