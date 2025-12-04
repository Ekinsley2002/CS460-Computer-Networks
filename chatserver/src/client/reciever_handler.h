#ifndef RECEIVER_HANDLER_H
#define RECEIVER_HANDLER_H

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
#include "message.h"
#include "properties.h"
#include "main.h"
#include "dbg.h"
#include "text_color.h"

void *connection_to_server(void* arg);
void *receive_from_server(void* arg);

#endif /* RECEIVER_HANDLER_H */