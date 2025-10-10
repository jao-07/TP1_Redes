#include "server.h"

int create_server_socket(char* protocol){
    int server_socket = 0;
    if(!strcmp(protocol, "v4"))
        server_socket = socket(AF_INET, SOCK_STREAM, 0);
    else if(!strcmp(protocol,"v6"))
        server_socket = socket(AF_INET6, SOCK_STREAM, 0);
    else
        server_socket = -1;

    if(server_socket < 0){
        perror("Erro ao criar o socket de servidor");
        exit(-1);
    }

    return server_socket;
}

struct sockaddr_in *create_ipv4_addr(int port){  
    struct sockaddr_in *address_v4 = malloc(sizeof(struct sockaddr_in));
    address_v4->sin_family = AF_INET;
    address_v4->sin_port = htons(port);
    address_v4->sin_addr.s_addr = INADDR_ANY;

    return address_v4;
}

struct sockaddr_in6 *create_ipv6_addr(int port){  
    struct sockaddr_in6 *address_v6 = malloc(sizeof(struct sockaddr_in6));
    address_v6->sin6_family = AF_INET6;
    address_v6->sin6_port = htons(port);
    address_v6->sin6_flowinfo = 0; 
    address_v6->sin6_addr.s6_addr = in6addr_any;
    address_v6->sin6_scope_id = 0;

    return address_v6;
}