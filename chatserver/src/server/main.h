#ifndef MAIN_H
#define MAIN_H

// chat node file
#include "chat_node.h"

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

// global variables
extern pthread_mutex_t mutex_client_socket;
extern pthread_mutex_t mutex_chat_node_list;

// declare chat node linked list
extern ChatNodes* chatNodes;

#endif /* main.h */