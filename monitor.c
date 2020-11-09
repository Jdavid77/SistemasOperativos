#include "bibliotecas.h"
main (){
    
}

int escreve(){  
        int fd = open("monitor.log", O_WRONLY |O_CREAT, S_IRUSR | S_IWUSR);
        if(fd == 0){ // Se o ficheiro nao foi aberto ou criado
            printf("Erro ao abrir o ficheiro");
                return -1; // Retorna a indicação de erro

        }
        else{
            
        }
}
