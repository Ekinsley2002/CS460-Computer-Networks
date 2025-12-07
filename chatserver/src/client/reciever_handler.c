#include "reciever_handler.h"

// initialize the mutex (avoids race conditions)
pthread_mutex_t mutex_reciever_socket = PTHREAD_MUTEX_INITIALIZER;

// This function is used 
void *receive_from_server(void* arg)
{
    // initialize variables
    int receiver_socket;
    int client_socket;
    struct sockaddr_in server_address;
    int yes = 1;
    Properties* properties = (Properties*) arg;
    int my_port = atoi(property_get_property(properties, "MY_PORT"));
    signal(SIGPIPE, SIG_IGN);

    // create socket for receiver
    if ((receiver_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("[RECEIVER] Socket creation failed. Exiting...");
        exit(EXIT_FAILURE);
    }

    // set socket options
    if (setsockopt(receiver_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
    {
        perror("[RECEIVER] Setting socket options failed. Exiting...");
        exit(EXIT_FAILURE);
    }

    // set server address
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(my_port);

    // bind socket to server address (using recievr socket)
    if(bind(receiver_socket, (struct sockaddr*)&server_address, sizeof(server_address)) != 0)
    {
        perror("[RECEIVER] Socket binding failed. Exiting...");
        exit(EXIT_FAILURE);
    }

    // listen for connections in the reciever socket
    if(listen(receiver_socket, NUM_CONNECTIONS) != 0)
    {
        perror("[RECEIVER] Socket listening failed. Exiting...");
        exit(EXIT_FAILURE);
    }

    debug("[RECEIVER] Receiver socket is now listening on the port...");

    // loop until SHUTDOWN message is received
    while(true)
    {
        // lock the mutex (to avoid race conditions)
        pthread_mutex_lock(&mutex_reciever_socket);

        // accept a connection from the server
        client_socket = accept(receiver_socket, NULL, NULL);

        debug(stderr,"Server connection established!");

        // create a thread to handle the connection to the server
        pthread_t thread;

        // check if the thread creation was a success
        if (pthread_create(&thread, NULL, connection_to_server, (void*)&client_socket))
        {
            debug("[RECEIVER] Thread creation failed. Exiting...");
            exit(EXIT_FAILURE);
        }

        // detach the thread (to avoid zombie processes
        if(pthread_detach(thread))
        {
            debug("[RECEIVER] Thread detaching failed. Exiting...");
            exit(EXIT_FAILURE);
        }
    }
}

// Handles a connection to the server with the argument given (properties)
void *connection_to_server(void* arg)
{
    // initialize variables
    int client_socket = *((int*)arg);
    Message message;
    pthread_mutex_unlock(&mutex_reciever_socket);
    receiveMessage(client_socket, &message);

    // switch on the message type
    switch(message.type)
    {
        // in the case the user is sending a message
        case NOTE:
        {
            printf("%s%s:%s %s", NOTE_COLOR, message.chatNode.name, RESET_COLOR, message.note);

            // close the socket
            if(close(client_socket) == -1 )
            {
                perror("[RECEIVER] Error encountered when trying to close socket.");
            }

            // check if the socket was closed successfully
            else
            {
                debug("[RECEIVER] Socket closed: %d", client_socket);
            }
            
            break;
        }

        // in the case the user wanst to shutdown there server
        case SHUTDOWN:
        {
            printf("Shutting down...\n");

            // close the socket
            if (close(client_socket) == -1)
            {
                perror("[RECEIVER] Error encountered closing socket.");
            }

            // check if the socket was closed successfully
            else
            {
                debug("[RECEIVER] Socket closed: %d", client_socket);
            }

            exit(EXIT_SUCCESS);
        }

        // in the case where the users wants to join in on the chat
        case JOIN:
        {
            printf("%s%s joined the chat%s\n", JOINED_COLOR, message.chatNode.name, RESET_COLOR);

            // close the socket
            if (close(client_socket) == -1)
            {
                perror("[RECEIVER] Error encountered closing socket.");
            }

            // check if the socket was closed successfully
            else
            {
                debug("[RECEIVER] Socket closed: %d", client_socket);
            }
            break;
        }

        // in the case where the user wants to leave the chat
        case LEAVE:
        {
            printf("%s%s left the chat%s\n", LEFT_COLOR, message.chatNode.name, RESET_COLOR);

            // close the socket
            if (close(client_socket) == -1)
            {
                perror("[RECEIVER] error closing socket");
            }

            // check if the socket was closed successfully
            else
            {
                debug("[RECEIVER] socket closed: %d", client_socket);
            }
            break;
        }
    }

    pthread_exit(NULL);
}