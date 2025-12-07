#include "reciever_handler.h"

pthread_mutex_t mutex_reciever_socket = PTHREAD_MUTEX_INITIALIZER;

void *receive_from_server(void* arg)
{
    // handles right after connection
    int receiver_socket;
    int client_socket;
    struct sockaddr_in server_address;
    int yes = 1;

    Properties* properties = (Properties*) arg;
    int my_port = atoi(property_get_property(properties, "MY_PORT"));

    signal(SIGPIPE, SIG_IGN);

    if ((receiver_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("[RECEIVER] Socket creation failed. Exiting...");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(receiver_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
    {
        perror("[RECEIVER] Setting socket options failed. Exiting...");
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(my_port);

    if(bind(receiver_socket, (struct sockaddr*)&server_address, sizeof(server_address)) != 0)
    {
        perror("[RECEIVER] Socket binding failed. Exiting...");
        exit(EXIT_FAILURE);
    }

    if(listen(receiver_socket, NUM_CONNECTIONS) != 0)
    {
        perror("[RECEIVER] Socket listening failed. Exiting...");
        exit(EXIT_FAILURE);
    }

    debug("[RECEIVER] Receiver socket is now listening on the port...");

    while(true)
    {
        pthread_mutex_lock(&mutex_reciever_socket);

        client_socket = accept(receiver_socket, NULL, NULL);
        debug(stderr,"Server connection established!");

        pthread_t thread;
        if (pthread_create(&thread, NULL, connection_to_server, (void*)&client_socket))
        {
            debug("[RECEIVER] Thread creation failed. Exiting...");
            exit(EXIT_FAILURE);
        }

        if(pthread_detach(thread))
        {
            debug("[RECEIVER] Thread detaching failed. Exiting...");
            exit(EXIT_FAILURE);
        }
    }
}

void *connection_to_server(void* arg)
{
    int client_socket = *((int*)arg);
    Message message;

    pthread_mutex_unlock(&mutex_reciever_socket);

    receiveMessage(client_socket, &message);

    switch(message.type)
    {
        case NOTE:
        {
            printf("%s%s:%s %s", NOTE_COLOR, message.chatNode.name, RESET_COLOR, message.note);

            if(close(client_socket) == -1 )
            {
                perror("[RECEIVER] Error encounterd when closing socket.");
            }
            else
            {
                debug("[RECEIVER] Socket closed: %d", client_socket);
            }
            
            break;
        }
        case SHUTDOWN:
        {
            printf("Shutting down...\n");

            if (close(client_socket) == -1)
            {
                perror("[RECEIVER] Error encountered closing socket.");
            }
            else
            {
                debug("[RECEIVER] Socket closed: %d", client_socket);
            }

            exit(EXIT_SUCCESS);
        }
        case JOIN:
        {
            printf("%s%s joined the chat%s\n", JOINED_COLOR, message.chatNode.name, RESET_COLOR);

            if (close(client_socket) == -1)
            {
                perror("[RECEIVER] Error encountered closing socket.");
            }
            else
            {
                debug("[RECEIVER] Socket closed: %d", client_socket);
            }
            break;
        }
        case LEAVE:
        {
            printf("%s%s left the chat%s\n", LEFT_COLOR, message.chatNode.name, RESET_COLOR);

            if (close(client_socket) == -1)
            {
                perror("[RECEIVER] error closing socket");
            }
            else
            {
                debug("[RECEIVER] socket closed: %d", client_socket);
            }
            break;
        }
    }
    pthread_exit(NULL);
}