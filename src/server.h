#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

int create_server_socket(char* protocol);
struct sockaddr *create_server_addr(int port, char* protocol, socklen_t *len);
char * process_option(int option, int server_hp, int client_hp);