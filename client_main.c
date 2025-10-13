#include "client.h"

int main(int argc, char *argv[]){

    if(argc != 2){
        printf("Parâmetros passados inválidos!");
        exit(EXIT_FAILURE);
    }

    struct sockaddr *client_addr
    socklen_t client_addrlen;
    int is_v4;

    client_addr = create_client_addr(ip, port, &client_addrlen, &is_v4);

    int client_socket = socket(is_v4 ? AF_INET : AF_INET6, SOCK_STREAM, 0);

    if (connect(client_socket, client_addr, client_addrlen) < 0) {
        perror("Erro ao conectar");
        exit(EXIT_FAILURE);
    }
}