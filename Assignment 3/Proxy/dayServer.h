// standard libararies
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// socket/bind/listen/accept
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

// close/recv
#include <sys/uio.h>
#include <sys/types.h>
#include <unistd.h>

#include <signal.h>
#include <netdb.h>
#include <time.h>
#include <pthread.h>

// This header file is responsible for evey library used and global constants for:
// datClient.c, dayClientPersonal.c, and DayServer.c

// function prototype
void* handle_client(void* arg);

// preprocessor directives
#define SERVER_ADDR "time.nist.gov"  // nist.gov IP, for client

#define SERVER_ADDR_PERSONAL "REPLACE_WITH_YOUR_HOST_NAME" // Personal server replace with yours
#define PORT "13"                     // port the server will listen on
#define PORT_PERSONAL "1300" // Custome port for our custom server

// I cannot let go of the old-fashioned way :) - for readability ... (from Dr. Otte)
#define FALSE false
#define TRUE !false

// number of pending connections in the connection queue (from Dr. Otte)
#define NUM_CONNECTIONS 5