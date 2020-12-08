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

#define TamLinha 1024       //tamanho maximo do buffer

struct pessoa  
{
    int id;
    int idade; 
    int prioridade;
    int lugarFila;
    bool resultadoTeste;
    float tempoEntradaInternamento;
    float tempoSaidaInternamento;
    int tempoDeInternamento;
    bool desistiuFila;
    bool isolamento;
};

struct centroDeTeste
{
    int id;
    int capacidadeMaxima;
    int capacidaMaximaInternamento;
    int capacidaMaximaPessoasTestadas;
};

struct fila
{   
    int idCentro;
    float tempoMedioEspera;
    int capacidadeMaximaFila;
    int casosRisco;
    int casosNormais;
    int desistenciaRisco;
    int desistenciaNormal;
};
