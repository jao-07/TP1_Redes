#include "server.h"

int main(int argc, char *argv[]){

    int server_socket, client_socket;
    char* protocol = argv[1];
    int port = atoi(argv[2]);

    struct sockaddr *server_addr;
    socklen_t server_addrlen;

    char input_buffer[1024] = { 0 };
    char output_buffer[1024] = { 0 };

    // struct sockadd_in client_addr4;
    // struct sockadd_in6 client_addr6;
    // struct sockaddr *client_addr;
    // socklen_t client_addrlen;

    server_socket = create_server_socket(argv[1]);

    if(server_socket < 0){
        perror("Erro ao criar o socket de servidor");
        exit(EXIT_FAILURE);
    }
    
    server_addr = create_server_addr(port, protocol, &server_addrlen);
    if(!server_addr){
        perror("Erro ao criar o endereço");
        exit(EXIT_FAILURE);
    }

    if(bind(server_socket, server_addr, server_addrlen) < 0){
        perror("Erro no bind");
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 1) < 0) {
        perror("Erro no listen");
        exit(EXIT_FAILURE);
    }

    // if(!strcmp(protocol, "v4")){
    //     client_addr = (struct sockaddr *)&client_addr4
    //     client_addrlen = sizeof(client_addr4)
    // }
    // else{
    //     client_addr = (struct sockaddr *)&client_addr6
    //     client_addrlen = sizeof(client_addr6)
    // }

    client_socket = accept(server_socket, NULL, NULL);

    if(client_socket < 0){
        perror("Erro no accept");
        exit(EXIT_FAILURE);
    }

    while(1){
        memset(input_buffer, 0, sizeof(input_buffer));
        ssize_t received_bytes = recv(client_socket, input_buffer, sizeof(input_buffer) - 1, 0);
        if(received_bytes <= 0){
            printf("Conexão encerrada.");
            break;
        }

        input_buffer[received_bytes] = '\0';
        printf("Mensagem do cliente: %s\n", input_buffer);

        memset(output_buffer, 0, sizeof(output_buffer));
        send(client_socket, buffer)
    }

    close(client_socket);
    close(server_socket);

}