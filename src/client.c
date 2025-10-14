#include "client.h"

int create_client_socket(char* ip, int port){
    int client_socket = 0;
    struct sockaddr_in address_v4;
    struct sockaddr_in6 address_v6;

    if (inet_pton(AF_INET, ip, &address_v4.sin_addr) == 1){
        client_socket = socket(AF_INET, SOCK_STREAM, 0);
        // address_v4.sin_family = AF_INET;
        // address_v4.sin_port = htons(port);
        // client_addr = (struct sockaddr*)address_v4;
    }
    else if (inet_pton(AF_INET6, ip, &address_v6.sin6_addr) == 1){
        client_socket = socket(AF_INET6, SOCK_STREAM, 0);
        // address_v6.sin6_family = AF_INET6;
        // address_v6.sin6_port = htons(port);
        // address_v6->sin6_scope_id = 0;
        // address_v6->sin6_flowinfo = 0;
        // client_addr = (struct sockaddr*)address_v6;
    }
    else
        client_socket = -1;

    return client_socket;
}

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