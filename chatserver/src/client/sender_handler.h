#ifndef SENDER_HANDLER_H
#define SENDER_HANDLER_H

// standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>

// foreign file inclusion
#include "properties.h"
#include "message.h"
#include "chat_node.h"
#include "dbg.h"

// function prototype
void *send_to_server(void* arg);

#endif /* SENDER_HANDLER_H */