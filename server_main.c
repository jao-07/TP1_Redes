#include "server.h"

int main(int argc, char *argv[]){

    int server_socket = create_server_socket(argv[1]);

    int port = atoi(argv[2]);
    if(port > 1024 && port <= 65535){
        printf("Porta válida\n");
    }
    else{
        printf("Parâmetro de porta inválido!\n");
        exit(-1);
    }

    if(!strcmp(protocol, "v4")){
}