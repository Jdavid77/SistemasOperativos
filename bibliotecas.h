#include <stdio.h>          //Biblioteca geral de input e outputs da linguagem C
#include <stdlib.h>         //Biblioteca padrão do C
#include <sys/types.h>      //Contem definicoes de data types usados nos system calls. E necessario para os dois procimos imports
#include <sys/socket.h>     //Inclui definicoes necessarias para trabalhar com sockets
#include <sys/un.h>         //contem estruturas e constantes necessarias para o dominio do endereco
#include <time.h>            //biblioteca para trabalhar com  o tempo
#include <stdbool.h>        //Para trabalhar com boleanos
#include <pthread.h>
#include <semaphore.h>
#include <ctype.h>