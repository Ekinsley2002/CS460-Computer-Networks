#ifndef MAIN_H
#define MAIN_H

// other header files
#include "properties.h"
#include "sender_handler.h"
#include "receiver_handler.h"
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

// client struct
typedef struct
{
	// store server ip
	unsigned int ip;

	// store port
	uint16_t port;

	// store client name
	char name[16];
} ClientProperties;

#endif /* main.h */