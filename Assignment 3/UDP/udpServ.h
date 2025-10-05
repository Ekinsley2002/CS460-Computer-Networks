// standard libraries for UDP connection
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <signal.h>
#include <netdb.h>
#include <stdbool.h>

// nist.gov address and port number
#define LOCAL_PORT 8080
#define SERVER_ADDR "time.nist.gov"
#define PORT 13

// readability (from Dr. Otte)
#define FALSE false
#define TRUE !false