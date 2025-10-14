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

char * process_option(int option, int server_hp, int client_hp){

}