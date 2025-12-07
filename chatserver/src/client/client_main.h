#ifndef CLIENT_MAIN_H
#define CLIENT_MAIN_H

// other header files
#include "properties.h"
#include "sender_handler.h"
#include "reciever_handler.h"
#include "message.h"

// standard libraries
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

// global constants
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 9000
#define CLIENT_NAME 32

// number of client connections allowed
#define NUM_CONNECTIONS 5

#endif /* CLIENT_MAIN_H */