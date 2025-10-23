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
#include "message.h"

int create_client_socket(char* ip, int port);
struct sockaddr *create_client_addr(char* ip, int port, socklen_t *len, int* is_v4);
void print_message(BattleMessage server_message);