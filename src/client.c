#include "client.h"

struct sockaddr *create_client_addr(char* ip, int port, socklen_t *len, int* is_v4){

    struct sockaddr_in *address_v4 = malloc(sizeof(struct sockaddr_in));
    struct sockaddr_in6 *address_v6 = malloc(sizeof(struct sockaddr_in6));

    if (inet_pton(AF_INET, ip, &address_v4->sin_addr) == 1){
        *is_v4 = 1;
        address_v4->sin_family = AF_INET;
        address_v4->sin_port = htons(port);
        *len = sizeof(*address_v4);
        free(address_v6);
        return (struct sockaddr*)address_v4;
    }
    
    if (inet_pton(AF_INET6, ip, &address_v6->sin6_addr) == 1){
        *is_v4 = 0;
        address_v6->sin6_family = AF_INET6;
        address_v6->sin6_port = htons(port);
        address_v6->sin6_scope_id = 0;
        address_v6->sin6_flowinfo = 0;
        *len = sizeof(*address_v6);
        free(address_v4);
        return (struct sockaddr*)address_v6;
    }

    free(address_v4);
    free(address_v6);

    return NULL;
}

void print_message(BattleMessage server_message){
    char* actions[] = {"Laser Attack", "Photon Torpedo", "Shields Up", "Cloaking", "Hyper Jump"};

    if(server_message.type == MSG_INIT || server_message.type == MSG_BATTLE_RESULT || server_message.type == MSG_GAME_OVER || server_message.type == MSG_ESCAPE){
        printf("%s\n", server_message.message);
        return;
    }

    if(server_message.type == MSG_ACTION_RES){
        printf("Você usou um %s\n", actions[server_message.client_action]);
        printf("Servidor usou um %s\n", actions[server_message.server_action]);
        printf("%s\n", server_message.message);
        return;
    }

    if(server_message.type == MSG_INVENTORY){
        printf("Inventário final:\n");
        printf("- HP restante: %d\n", server_message.client_hp);
        printf("- Torpedos usados: %d\n", server_message.client_torpedoes);
        printf("- Escudos usados: %d\n", server_message.client_shields);
        printf("%s\n", server_message.message);
        return;
    }

    printf("Tipo de mensagem inválido!");
    exit(EXIT_FAILURE);
}