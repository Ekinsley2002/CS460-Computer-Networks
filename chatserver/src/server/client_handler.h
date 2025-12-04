#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

// standard libraries 
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include "chat_node.h"
#include "message.h"
#include "dbg.h"
#include "main.h"
#include <netdb.h>

// function to talk to client
void* talk_to_client(void* arg);

// function prototypes for handling message cases
void join(Message* message);
void leave(Message* message);
void note(Message* message);
void shutdownAll(Message* message);

// ******THIS MAY BE WRONG PLEASE CHECK*****
const char* ip_ntop(unsigned int ip);

#endif /* CLIENT_HANDLER.H */