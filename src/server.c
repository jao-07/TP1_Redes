#include "server.h"

int create_server_socket(char* protocol){
    int server_socket = 0;
    if(!strcmp(protocol, "v4"))
        server_socket = socket(AF_INET, SOCK_STREAM, 0);
    else if(!strcmp(protocol,"v6"))
        server_socket = socket(AF_INET6, SOCK_STREAM, 0);
    else
        server_socket = -1;

    return server_socket;
}

struct sockaddr *create_server_addr(int port, char* protocol, socklen_t *len){
    if(!strcmp(protocol, "v4")){
        struct sockaddr_in *address_v4 = malloc(sizeof(struct sockaddr_in));
        address_v4->sin_family = AF_INET;
        address_v4->sin_port = htons(port);
        address_v4->sin_addr.s_addr = INADDR_ANY;
        *len = sizeof(*address_v4);

        return (struct sockaddr *)address_v4;
    }

    if(!strcmp(protocol,"v6")){
        struct sockaddr_in6 *address_v6 = malloc(sizeof(struct sockaddr_in6));
        address_v6->sin6_family = AF_INET6;
        address_v6->sin6_port = htons(port);
        address_v6->sin6_flowinfo = 0; 
        address_v6->sin6_addr = in6addr_any;
        address_v6->sin6_scope_id = 0;
        *len = sizeof(*address_v6);

        return (struct sockaddr *)address_v6;
    }
    
    return NULL;
}

void actions_results(struct BattleMessage* msg){

    char * client_phrases[4] = {"Você disparou um Laser!\n", "Você disparou um Photon Torpedo!\n", "Você ativou Cloaking\n", "Você ativou Escudos\n"};

    char * server_phrases[4] = {"Inimigo disparou um Laser!\n", "Inimigo disparou um Photon Torpedo!\n", "Inimigo ativou Cloaking\n", "Inimigo ativou Escudos\n"};

    char* client_option_selected = client_phrases[msg->client_action];
    char* server_option_selected = server_phrases[msg->server_action];

    char* blocked_attack_client = "Resultado: Ataque aliado bloqueado!\n";
    char* blocked_attack_server = "Resultado: Ataque inimigo bloqueado!\n";

    char* failed_attack_client = "Resultado: Ataque aliado falhou!\n";
    char* failed_attack_server = "Resultado: Ataque inimigo falhou!\n";

    char* success_attack_server = "Resultado: Você recebeu 20 de dano\n";
    char* success_attack_client = "Resultado: Inimigo recebeu 20 de dano\n";
    char* draw_attack = "Resultado: Ambos receberam 20 de dano\n";
    char* draw_fail = "Resultado: Nenhum ataque foi desferido. Ninguém recebeu dano\n";

    snprintf(msg->message, MSG_SIZE, "%s%s", client_option_selected, server_option_selected);

    if(msg->client_action == 0){
        if(msg->server_action == 0){
            snprintf(msg->message, MSG_SIZE, draw_attack);
            msg->client_hp -= 20;
            msg->server_hp -= 20;
        }
        if(msg->server_action == 1){
            snprintf(msg->message, MSG_SIZE, success_attack_server);
            msg->client_hp -= 20;
        }  
        if(msg->server_action == 2){
            snprintf(msg->message, MSG_SIZE, blocked_attack_client);
        }
        if(msg->server_action == 3){
            snprintf(msg->message, MSG_SIZE, success_attack_client);
            msg->server_hp -= 20;
        }    
    }

    if(msg->client_action == 1){
        if(msg->server_action == 0){
            snprintf(msg->message, MSG_SIZE, success_attack_client);
            msg->server_hp -= 20;
            msg->client_torpedoes++;
        }
        if(msg->server_action == 1){
            snprintf(msg->message, MSG_SIZE, draw_attack);
            msg->client_hp -= 20;
            msg->server_hp -= 20;
            msg->client_torpedoes++;
        }
        if(msg->server_action == 2){
            snprintf(msg->message, MSG_SIZE, blocked_attack_client);
            msg->client_torpedoes++;
        }
        if(msg->server_action == 3){
            snprintf(msg->message, MSG_SIZE, failed_attack_client);
        }
    }

    if(msg->client_action == 2){
        msg->client_shields++;
        if(msg->server_action == 0 || msg->server_action == 1){
            snprintf(msg->message, MSG_SIZE, blocked_attack_server);
        }
        if(msg->server_action == 2 || msg->server_action == 3){
            snprintf(msg->message, MSG_SIZE, draw_fail);
        }
    }

    if(msg->client_action == 3){
        if(msg->server_action == 0){
            snprintf(msg->message, MSG_SIZE, success_attack_server);
            msg->client_hp -= 20;
        }
        if(msg->server_action == 1){
            snprintf(msg->message, MSG_SIZE, failed_attack_server);
        }
        if(msg->server_action == 2 || msg->server_action == 3){
            snprintf(msg->message, MSG_SIZE, draw_fail);
        }
    }

    if(msg->client_action > 3 || msg->server_action > 3){
        printf("Valor de ação de cliente ou servidor inválido!");
        exit(EXIT_FAILURE);
    }

    snprintf(msg->message, MSG_SIZE, "Placar: Você %d x %d Inimigo\n\n");
        
}

struct  BattleMessage build_message(struct BattleMessage msg){
    if(msg.type != MSG_ACTION_REQ){
        printf("Tipo da mensagem do cliente inválida");
        exit(EXIT_FAILURE);
    }

    

    int server_choice = rand() % 5;

    if(msg.client_action == 4 && server_choice == 4){
        msg.type = MSG_ESCAPE;
        snprintf(msg->message, MSG_SIZE, "Fuga mútua! Você e o inimigo acionaram o Hyper Jump!\nSuas naves escaparam para o hiperespaço.");
        return msg;
    }

    if(msg.client_action == 4){
        msg.type = MSG_ESCAPE;
        snprintf(msg->message, MSG_SIZE, "Você acionou o Hyper Jump!\nSua nave escapou para o hiperespaço.");
        return msg;
    }

    if(server_choice == 4){
        msg.type = MSG_ESCAPE;
        snprintf(msg->message, MSG_SIZE, "O inimigo acionou o Hyper Jump!\nA nave dele escapou para o hiperespaço.");
        return msg;
    }

    msg.type = MSG_ACTION_RES;
    
    char* actions_result = actions_results(msg.client_action, server_action);

    snprintf(msg->message, MSG_SIZE, "%s%s%sPlacar: Você %d x %d Inimigo\n", client_option_selected, server_option_selected, actions_result, msg.client_hp, msg.server_hp);

    msg.server_action = server_choice;
}

void process_actions(struct BattleMessage message, int client_socket){
    struct  BattleMessage message;
    



}