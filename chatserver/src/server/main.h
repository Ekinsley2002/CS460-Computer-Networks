#ifndef MAIN_H
#define MAIN_H

// chat node file
#include "chat_node.h"
#include "chat_node.c"

// standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>

// global constants

// port for server
#define PORT 9000

// number of client connections allowed
#define NUM_CONNECTIONS 5

// sever loop condition
#define TRUE !false

// shared mutexes
extern pthread_mutex_t mutex_client_socket;
extern pthread_mutex_t mutex_chat_node_list;

// shared chat nodes list
extern ChatNode* chatNodes;

#endif /* main.h */