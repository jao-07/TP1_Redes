#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

int main(int argc, char *argv[]){

    int client_socket;
    if(!strcmp(argv[1], "v4")){
        client_socket = socket(AF_INET, SOCK_STREAM, 0);
        printf("Criado v4 com sucesso\n");
    }
    else if(!strcmp(argv[1], "v6")){
        client_socket = socket(AF_INET6, SOCK_STREAM, 0);
        printf("Criado v6 com sucesso\n");
    }
    else{
        printf("Parâmetro de versão de IP inválido!\n");
        exit(-1);
    }

    int port = atoi(argv[2]);
    if(port > 1024 && port <= 65535){
        printf("Porta válida\n");
    }
    else{
        printf("Parâmetro de porta inválido!\n");
        exit(-1);
    }

}