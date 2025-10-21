#include "server.h"
#include "message.h"

int main(int argc, char *argv[]){

    if(argc != 3){
        printf("Parâmetros passados inválidos!");
        exit(EXIT_FAILURE);
    }

    int server_socket, client_socket;
    char* protocol = argv[1];
    int port = atoi(argv[2]);

    struct sockaddr *server_addr;
    socklen_t server_addrlen;

    int client_input;
    char output_buffer[1024] = { 0 };

    int server_hp, client_hp = 100;
    struct BattleMessage server_message;
    struct BattleMessage client_message;

    server_socket = create_server_socket(argv[1]);

    srand(time(NULL));

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

    client_socket = accept(server_socket, NULL, NULL);

    if(client_socket < 0){
        perror("Erro no accept");
        exit(EXIT_FAILURE);
    }

    server_message->type = MSG_INIT;
    snprintf(server_message->message, MSG_SIZE, "Conectado ao servidor.\nSua nave : SS-42 Voyager (HP: 100 )");
    send(client_socket, output_buffer, strlen(output_buffer), 0);

    while(1){
        ssize_t received_bytes = recv(client_socket, &client_message, sizeof(client_message), 0);
        if(received_bytes <= 0){
            printf("Conexão encerrada.");
            break;
        }

        build_message(client_message);     
        send(client_socket, &server_message, strlen(server_message), 0);
    }

    close(client_socket);
    close(server_socket);

}