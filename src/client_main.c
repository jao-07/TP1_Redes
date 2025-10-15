#include "client.h"
#include "message.h"

int main(int argc, char *argv[]){

    struct sockaddr *client_addr;
    socklen_t client_addrlen;
    int is_v4;
    BattleMessage server_message;
    BattleMessage client_message;
    int option = -1;
    client_able_to_play = 1;

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

    client_message.type = MSG_ACTION_REQ;

    while(1){
        memset(input_buffer, 0, sizeof(input_buffer));
        ssize_t received_bytes = recv(client_socket, &server_message, sizeof(server_message), 0);
        if(received_bytes <= 0){
            printf("Conexão encerrada.");
            break;
        }

        if(server_message.client_hp == 0 || server_message.server_hp == 0 || server_message.type == MSG_ESCAPE)
            client_able_to_play = 0;

        print_message(server_message);
        
        if(client_able_to_play){
            while(option < 0 || option > 4){
                printf("Escolha uma opção:\n0: Laser Attack\n1: Photon Torpedo\n2: Shields Up\n3: Cloaking \n4: Hyper Jump\n\n");

                scanf("%d", &option);

                if(option < 0 || option > 4)
                    printf("Opção escolhida inválida! Escolha uma opção entre 1 e 5.\n\n");
            }

            int option_net = htonl(option);
            send(client_socket, &option_net, sizeof(option_net), 0);
            option = -1;
        }
    }
}