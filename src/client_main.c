#include "client.h"

int main(int argc, char *argv[]){

    struct sockaddr *client_addr;
    socklen_t client_addrlen;
    int is_v4;
    BattleMessage *battle_message = malloc(sizeof(BattleMessage));
    int option = -1;

    if(argc != 3){
        printf("Parâmetros passados inválidos!");
        exit(EXIT_FAILURE);
    }

    char* ip = argv[1];
    int port = atoi(argv[2]);

    client_addr = create_client_addr(ip, port, &client_addrlen, &is_v4);

    int client_socket = socket(is_v4 ? AF_INET : AF_INET6, SOCK_STREAM, 0);

    if (connect(client_socket, client_addr, client_addrlen) < 0) {
        perror("Erro ao conectar");
        exit(EXIT_FAILURE);
    }

    memset(battle_message, 0, sizeof(BattleMessage));
    while(1){
        ssize_t received_bytes = recv(client_socket, battle_message, sizeof(BattleMessage), 0);
        if(received_bytes <= 0){
            printf("Conexão encerrada.\n");
            break;
        }

        if(battle_message->type == MSG_INIT || battle_message->type == MSG_ESCAPE || battle_message->type == MSG_BATTLE_RESULT || battle_message->type == MSG_GAME_OVER){
            printf("%s", battle_message->message);
        }
        
        if(battle_message->type == MSG_ACTION_REQ){
            printf("%s", battle_message->message);
            scanf("%d", &battle_message->client_action);
            battle_message->type = MSG_ACTION_RES;
            send(client_socket, battle_message, sizeof(BattleMessage), 0);
        }

        if(battle_message->type == MSG_INVENTORY){
            printf("Inventário Final:\n- HP restante: %d\n- Torpedos usados: %d\n- Escudos usados: %d\n%s", battle_message->client_hp, battle_message->client_torpedoes, battle_message->client_shields, battle_message->message);
        }
    }
}