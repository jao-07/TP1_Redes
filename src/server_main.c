#include "server.h"

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

    BattleMessage *battle_message = malloc(sizeof(BattleMessage));

    server_socket = create_server_socket(argv[1]);

    srand(time(NULL));

    if(server_socket < 0){
        perror("Erro ao criar o socket de servidor");
        exit(EXIT_FAILURE);
    }

    printf("Socket criado");
    
    server_addr = create_server_addr(port, protocol, &server_addrlen);
    if(!server_addr){
        perror("Erro ao criar o endereço\n");
        exit(EXIT_FAILURE);
    }

    printf("Endereço criado\n");

    if(bind(server_socket, server_addr, server_addrlen) < 0){
        perror("Erro no bind");
        exit(EXIT_FAILURE);
    }

    printf("Bind\n");

    if (listen(server_socket, 1) < 0) {
        perror("Erro no listen");
        exit(EXIT_FAILURE);
    }

    printf("Listen\n");

    client_socket = accept(server_socket, NULL, NULL);

    if(client_socket < 0){
        perror("Erro no accept");
        exit(EXIT_FAILURE);
    }

    memset(battle_message, 0, sizeof(BattleMessage));
    battle_message->type = MSG_INIT;
    
    
    snprintf(battle_message->message, MSG_SIZE, "Conectado ao servidor.\nSua nave : SS-42 Voyager (HP: 100 )\n");
    send(client_socket, battle_message, sizeof(BattleMessage), 0);
    printf("Mensagem enviada");

    while(1){

        memset(battle_message->message, 0, MSG_SIZE);
        if(battle_message->client_action < 0 || battle_message->client_action > 4){
            snprintf(battle_message->message, MSG_SIZE, "Erro: Escolha inválida!\nPor favor selecione um valor entre 0 e 4.\n");
        }

        battle_message->type = MSG_ACTION_REQ;
        snprintf(battle_message->message + strlen(battle_message->message), MSG_SIZE - strlen(battle_message->message), "\nEscolha sua ação: 0=Laser, 1=Torpedo, 2=Escudo, 3=Cloaking, 4=Escape\n");
        send(client_socket, battle_message, sizeof(BattleMessage), 0);

        ssize_t received_bytes = recv(client_socket, &battle_message, sizeof(BattleMessage), 0);
        if(received_bytes <= 0){
            printf("Conexão encerrada.\n");
            break;
        }

        if(battle_message->type != MSG_ACTION_RES){
            printf("Tipo da mensagem do cliente inválida");
            exit(EXIT_FAILURE);
        }

        if(battle_message->client_action < 0 || battle_message->client_action > 4){
            continue;
        }

        handle_message(battle_message);
        battle_message->type = MSG_BATTLE_RESULT;    
        send(client_socket, battle_message, sizeof(BattleMessage), 0);

        if(battle_message->type == MSG_ESCAPE || !battle_message->client_hp || !battle_message->server_hp){
            battle_message->type = MSG_GAME_OVER;
            snprintf(battle_message->message, sizeof(battle_message->message), "Fim de jogo!\n");
            send(client_socket, battle_message, sizeof(BattleMessage), 0);

            if(battle_message->type == MSG_ESCAPE) snprintf(battle_message->message, sizeof(battle_message->message), "Obrigado por jogar!\n");
            if(battle_message->client_hp && !battle_message->server_hp) snprintf(battle_message->message, sizeof(battle_message->message), "Você derrotou a frota inimiga!\n");
            if(!battle_message->client_hp && battle_message->server_hp) snprintf(battle_message->message, sizeof(battle_message->message), "Sua nave foi destruída!\n");
            if(!battle_message->client_hp && !battle_message->server_hp) snprintf(battle_message->message, sizeof(battle_message->message), "Ambas as naves foram destruídas!\n");
            battle_message->type = MSG_INVENTORY;
            send(client_socket, battle_message, sizeof(BattleMessage), 0);

            break;
        }
    }

    close(client_socket);
    close(server_socket);

}